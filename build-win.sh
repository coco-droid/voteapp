#!/usr/bin/env bash
#
# build-win.sh — Génère automatiquement l'exécutable Windows (app.exe)
# dans le sous-dossier ./win/ par cross-compilation MinGW depuis Linux.
#
# Usage :
#   ./build-win.sh            # build release complet
#   ./build-win.sh --clean    # reconfigure + rebuild from scratch
#
# Ce que fait le script :
#   1. Vérifie la présence de xmake et d'une toolchain MinGW.
#      - Si x86_64-w64-mingw32-gcc est dans le PATH, il est utilisé.
#      - Sinon, si tools/llvm-mingw existe, il est utilisé.
#      - Sinon, llvm-mingw est téléchargé automatiquement dans tools/
#        (aucun sudo nécessaire).
#   2. Configure xmake : plateforme windows, toolchain mingw, x86_64, release.
#   3. Compile la cible "app".
#   4. Assemble ./win/ : app.exe + assets/.
#
# Note : après ce script, le projet reste configuré pour Windows.
# Pour revenir à un build Linux :  xmake f -p linux -m release -y
#
set -euo pipefail

# --- Paramètres --------------------------------------------------------------
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
MINGW_DIR="$ROOT/tools/llvm-mingw"
OUT_DIR="$ROOT/win"
BUILD_SUBDIR="build/windows/x86_64/release"
LLVM_MINGW_VERSION="20260826"   # fallback si l'API GitHub est injoignable

cd "$ROOT"

log()  { printf '\033[1;34m==>\033[0m %s\n' "$*"; }
err()  { printf '\033[1;31mErreur :\033[0m %s\n' "$*" >&2; }

# --- 1. Prérequis : xmake ----------------------------------------------------
if ! command -v xmake >/dev/null 2>&1; then
    err "xmake introuvable. Installez-le : https://xmake.io/#/guide/installation"
    exit 1
fi

# --- 2. Prérequis : toolchain MinGW ------------------------------------------
MINGW_SDK=""

if [ -x "$MINGW_DIR/bin/x86_64-w64-mingw32-gcc" ]; then
    MINGW_SDK="$MINGW_DIR"
elif command -v x86_64-w64-mingw32-gcc >/dev/null 2>&1; then
    # Toolchain système (ex. paquet mingw-w64) : xmake cherche dans <sdk>/bin
    MINGW_SDK="/usr"
else
    log "Aucune toolchain MinGW détectée : téléchargement de llvm-mingw…"
    mkdir -p "$ROOT/tools"

    # Récupère l'URL de la dernière release ubuntu/x86_64, sinon version figée
    URL="$(curl -fsSL https://api.github.com/repos/mstorsjo/llvm-mingw/releases/latest \
        | grep -oE '"browser_download_url": *"[^"]*ucrt-ubuntu-[0-9.]+-x86_64\.tar\.xz"' \
        | head -1 | cut -d'"' -f4 || true)"
    if [ -z "${URL:-}" ]; then
        URL="https://github.com/mstorsjo/llvm-mingw/releases/download/${LLVM_MINGW_VERSION}/llvm-mingw-${LLVM_MINGW_VERSION}-ucrt-ubuntu-22.04-x86_64.tar.xz"
    fi

    TMP="$ROOT/tools/llvm-mingw.tar.xz"
    log "Téléchargement : $URL"
    curl -fSL --retry 3 -o "$TMP" "$URL"
    log "Extraction…"
    tar -C "$ROOT/tools" -xf "$TMP"
    rm -f "$TMP"
    # Le tarball contient un dossier versionné : on le normalise en llvm-mingw
    EXTRACTED="$(find "$ROOT/tools" -maxdepth 1 -type d -name 'llvm-mingw-*' | head -1)"
    [ -n "$EXTRACTED" ] && mv "$EXTRACTED" "$MINGW_DIR"
    MINGW_SDK="$MINGW_DIR"
fi
log "Toolchain MinGW : $MINGW_SDK"

# --- 2b. Patchs de portage MinGW sur LCUI -------------------------------------
# LCUI est écrit pour MSVC ; ces patchs (conservés dans patches/) corrigent
# la cross-compilation MinGW. Appliqués de façon idempotente.
apply_patch() {  # apply_patch <repo> <patch>
    local repo="$1" patch="$2"
    [ -d "$repo/.git" ] || [ -f "$repo/.git" ] || return 0
    if git -C "$repo" apply --check "$patch" 2>/dev/null; then
        git -C "$repo" apply "$patch"
        log "Patch appliqué : $(basename "$patch")"
    else
        : # déjà appliqué ou non applicable : on continue
    fi
}
apply_patch "$ROOT/vendor/LCUI"           "$ROOT/patches/lcui-mingw-windows.patch"
apply_patch "$ROOT/vendor/LCUI/lib/yutil" "$ROOT/patches/lcui-yutil-errno.patch"

# --- 3. Configuration xmake ---------------------------------------------------
# --require=n : ne pas tenter d'installer les dépendances optionnelles de LCUI
# (libpng, freetype, cmake, python…) depuis le dépôt xmake.
CLEAN_FLAG=""
[ "${1:-}" = "--clean" ] && CLEAN_FLAG="-c"

log "Configuration xmake (windows / mingw / x86_64 / release)…"
xmake f -p windows --toolchain=mingw --mingw="$MINGW_SDK" \
        -a x86_64 -m release --require=n -y $CLEAN_FLAG

# --- 4. Compilation -----------------------------------------------------------
log "Compilation…"
xmake build app

EXE="$ROOT/$BUILD_SUBDIR/app.exe"
if [ ! -f "$EXE" ]; then
    err "app.exe introuvable après le build ($EXE)"
    exit 1
fi

# --- 5. Assemblage du dossier win/ --------------------------------------------
log "Assemblage de $OUT_DIR/"
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"
cp "$EXE" "$OUT_DIR/"
if [ -d "$ROOT/dist" ]; then
    cp -r "$ROOT/dist/." "$OUT_DIR/"
fi

log "Terminé ✔  Exécutable Windows généré :"
ls -lh "$OUT_DIR/app.exe"
printf '\nLe dossier \033[1mwin/\033[0m est autonome : copiez-le tel quel sur Windows\net lancez \033[1mapp.exe\033[0m (sous-système GUI, aucune DLL à ajouter).\n'
