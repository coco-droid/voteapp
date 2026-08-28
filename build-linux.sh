#!/usr/bin/env bash
#
# build-linux.sh — Génère automatiquement l'exécutable Linux (app)
# dans le sous-dossier ./linux/ par compilation native.
#
# Usage :
#   ./build-linux.sh            # build release complet
#   ./build-linux.sh --clean    # reconfigure + rebuild from scratch
#
# Ce que fait le script :
#   1. Vérifie la présence de xmake, gcc et des en-têtes de développement
#      (X11, png, jpeg, freetype — dépendances optionnelles de LCUI).
#   2. Configure xmake : plateforme linux, x86_64, release.
#   3. Compile la cible "app".
#   4. Assemble ./linux/ : app + assets/.
#
# Note : après ce script, le projet reste configuré pour Linux.
# Pour un build Windows :  ./build-win.sh
#
set -euo pipefail

# --- Paramètres --------------------------------------------------------------
ROOT="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
OUT_DIR="$ROOT/linux"
BUILD_SUBDIR="build/linux/x86_64/release"

cd "$ROOT"

log()  { printf '\033[1;34m==>\033[0m %s\n' "$*"; }
warn() { printf '\033[1;33mAttention :\033[0m %s\n' "$*" >&2; }
err()  { printf '\033[1;31mErreur :\033[0m %s\n' "$*" >&2; }

# --- 1. Prérequis -------------------------------------------------------------
if ! command -v xmake >/dev/null 2>&1; then
    err "xmake introuvable. Installez-le : https://xmake.io/#/guide/installation"
    exit 1
fi
if ! command -v cc >/dev/null 2>&1 && ! command -v gcc >/dev/null 2>&1; then
    err "Aucun compilateur C (gcc/clang). Installez-le : sudo apt install build-essential"
    exit 1
fi

# En-têtes optionnels de LCUI : absents = fonctionnalités réduites (pas de X11,
# pas de png/jpeg/freetype). On avertit sans bloquer.
MISSING=""
for pkg in x11 libpng libjpeg freetype2 fontconfig; do
    pkg-config --exists "$pkg" 2>/dev/null || MISSING="$MISSING $pkg"
done
if [ -n "$MISSING" ]; then
    warn "Paquets de développement introuvables :$MISSING"
    warn "LCUI sera compilé sans ces fonctionnalités. Pour les installer :"
    warn "  sudo apt install libx11-dev libpng-dev libjpeg-dev libfreetype-dev libfontconfig-dev"
fi

# --- 2. Configuration xmake ---------------------------------------------------
CLEAN_FLAG=""
[ "${1:-}" = "--clean" ] && CLEAN_FLAG="-c"

log "Configuration xmake (linux / x86_64 / release)…"
xmake f -p linux -a x86_64 -m release -y $CLEAN_FLAG

# --- 3. Compilation -----------------------------------------------------------
log "Compilation…"
xmake build app

BIN="$ROOT/$BUILD_SUBDIR/app"
if [ ! -f "$BIN" ]; then
    err "Binaire introuvable après le build ($BIN)"
    exit 1
fi

# --- 4. Assemblage du dossier linux/ ------------------------------------------
log "Assemblage de $OUT_DIR/"
rm -rf "$OUT_DIR"
mkdir -p "$OUT_DIR"
cp "$BIN" "$OUT_DIR/"
if [ -d "$ROOT/dist" ]; then
    cp -r "$ROOT/dist/." "$OUT_DIR/"
fi

log "Terminé ✔  Exécutable Linux généré :"
ls -lh "$OUT_DIR/app"
printf '\nLe dossier \033[1mlinux/\033[0m est prêt : \033[1mcd linux && ./app\033[0m\n'
printf 'Dépendances système requises à l'\''exécution : libX11, libpng, libjpeg, libfreetype.\n'
