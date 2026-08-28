# Environnement de développement

Ce document explique comment reproduire l'environnement de développement
complet à partir d'un clone frais du dépôt.

## 1. Vue d'ensemble de la chaîne de build

```
app/*.tsx ──lcui build──► app/*.tsx.h, app/*/page.c, dist/ ──xmake──► app / app.exe
src/core/*.c ────────────────────────────────────────────────┘
vendor/LCUI (sous-module git) ──► librairie UI compilée par xmake
```

Trois outils sont nécessaires :

| Outil | Rôle | Installation |
|---|---|---|
| **Node.js + npm** | Dépendances JS (`@lcui/react`, tailwind…) | https://nodejs.org |
| **`@lcui/cli`** (v1.3.0) | Compile les `.tsx`/`.css` en C, génère `dist/` | `npm install -g @lcui/cli` |
| **xmake** (v3.1+) | Compile le C et lie LCUI | https://xmake.io/#/guide/installation |

## 2. Mise en route après un clone

```bash
# 1. Cloner avec le sous-module LCUI (hébergé sur gitee, voir .gitmodules)
git clone --recurse-submodules <url-du-depot>
#   …ou après coup :
git submodule update --init --recursive

# 2. Dépendances JavaScript (recrée node_modules/)
npm install        # ou : yarn install

# 3. Générer les fichiers C à partir des sources TSX + le dossier dist/
lcui build

# 4a. Compiler et lancer sous Linux
./build-linux.sh   # produit linux/app

# 4b. Cross-compiler pour Windows (télécharge llvm-mingw automatiquement)
./build-win.sh     # produit win/app.exe
```

## 3. Contenu NON versionné et régénération

Ces éléments sont volontairement exclus du dépôt (voir `.gitignore`) :

| Chemin | Qu'est-ce ? | Comment le régénérer |
|---|---|---|
| `node_modules/` | Dépendances npm (`@lcui/react`, `@lcui/react-icons`, tailwindcss, postcss…) | `npm install` |
| `vendor.node_modules/` | Ancienne copie manuelle de `@lcui/react-icons`, **non référencée** par le build (les imports `@lcui/react-icons` sont résolus via `node_modules/`) | Inutile — supprimable |
| `vendor/LCUI/` | Bibliothèque UI, **sous-module git** (https://gitee.com/lc-soft/LCUI.git) | `git submodule update --init --recursive` |
| `patches/*.patch` | Patchs de portage MinGW appliqués **automatiquement** par `build-win.sh` sur `vendor/LCUI` (et son sous-module `lib/yutil`) — voir §4.2 | `./build-win.sh` |
| `dist/` | Assets compilés avec noms hachés (polices Fluent, logo) | `lcui build` |
| `.lcui/` | Cache et manifeste du compilateur lcui | `lcui build` |
| `build/`, `.xmake/` | Objets, binaires et configuration xmake | `./build-linux.sh` ou `./build-win.sh` |
| `include/config.h`, `include/version.h` | En-têtes générés par xmake | configuration xmake |
| `tools/llvm-mingw/` | Toolchain de cross-compilation Windows (~300 Mo) | `./build-win.sh` (téléchargement auto) ou `sudo apt install mingw-w64` |
| `win/`, `linux/` | Exécutables assemblés prêts à distribuer | `./build-win.sh` / `./build-linux.sh` |
| `src/obj/` | Fichiers objets de Code::Blocks (ancien IDE) | Inutile — supprimable |

> **Note :** les fichiers générés `app/**/*.tsx.h`, `app/**/*.css.h` et
> `app/*/page.c` **sont** versionnés, contrairement au template d'origine.
> Le dépôt reste donc compilable avec xmake seul ; `lcui build` n'est
> nécessaire que si vous modifiez les sources `.tsx` / `.css`.

## 4. Paramétrage détaillé

### 4.1 Linux (compilation native)

Prérequis système (Debian/Ubuntu) :

```bash
sudo apt install build-essential pkg-config \
    libx11-dev libpng-dev libjpeg-dev libfreetype-dev libfontconfig-dev
```

Ces paquets `-dev` sont détectés par xmake via `pkg-config` et activent les
fonctionnalités correspondantes de LCUI (rendu X11, images png/jpeg, polices).
Sans eux, la compilation aboutit mais l'application perd l'affichage graphique.

```bash
./build-linux.sh            # binaire dans linux/app
cd linux && ./app           # exécution
```

Dépendances à l'exécution : `libX11`, `libpng16`, `libjpeg`, `libfreetype`.

### 4.2 Windows (cross-compilation depuis Linux)

Aucune installation manuelle requise : `build-win.sh` télécharge
automatiquement **llvm-mingw** (toolchain clang → PE Windows) dans `tools/`
si aucun `x86_64-w64-mingw32-gcc` n'est trouvé. Alternative système :

```bash
sudo apt install mingw-w64
./build-win.sh              # exécutable dans win/app.exe
```

Spécificités du port Windows :

- `wincompat/` : wrappers `Windows.h` → `windows.h` (casse) pour MinGW ;
  référencés dans `xmake.lua` sous `is_plat("windows")`.
- `xmake.lua` cible Windows : `-mwindows` (sous-système GUI),
  `_WIN32_WINNT=0x0A00`, syslinks `user32 gdi32 imm32`.
- `app/backend.h` : réimplémentation portable de `strcasestr()`
  (extension GNU absente de Windows), active sous `#ifdef _WIN32`.
- `patches/lcui-mingw-windows.patch` et `patches/lcui-yutil-errno.patch` :
  correctifs de portabilité sur le code Win32 de LCUI (écrit pour MSVC) —
  `errno.h` manquant, pragmas MSVC, `__FUNCTIONW__`, `LIBUI_INLINE __inline`
  sans émission de symbole sous clang, casse `Shell32`→`shell32`, etc.
  **`build-win.sh` les applique automatiquement** (idempotent) après
  l'initialisation du sous-module ; ils ne sont pas commités dans le
  sous-module car celui-ci pointe vers le dépôt upstream gitee.

L'exécutable Windows ne dépend d'**aucune DLL tierce** (UCRT système
uniquement) : le dossier `win/` est autonome.

### 4.3 Couche TSX → C (interface graphique)

- Sources déclaratives : `app/**/page.tsx`, `app/layout.tsx`, styles
  `app/global.css` (+ Tailwind, voir `tailwind.config.js` et
  `postcss.config.js` — le plugin `@thedutchcoder/postcss-rem-to-px`
  convertit les `rem` en `px`, non supportés par le moteur CSS de LCUI).
- Après toute modification d'un `.tsx` ou `.css` :

```bash
lcui build        # régénère les .tsx.h / .css.h / page.c et dist/
```

- Icônes : `@lcui/react-icons` (Fluent System Icons), importé dans les
  `.tsx` ; les polices `.ttf` aboutissent dans `dist/assets/`.

## 5. Structure du dépôt

```
app/          Interface LCUI (pages TSX + code C généré + backend.c/.h)
src/          Couche métier électorale console d'origine + core/ partagé
vendor/LCUI   Bibliothèque UI (sous-module)
wincompat/    Wrappers d'en-têtes pour la cross-compilation MinGW
patches/      Patchs de portage MinGW appliqués à vendor/LCUI (auto)
build-linux.sh / build-win.sh   Scripts de génération des exécutables
xmake.lua     Configuration de build C
package.json  Dépendances JS (lcui react, tailwind, postcss)
```
