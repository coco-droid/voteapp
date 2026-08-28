set_project("app")
set_version("1.0.0")
set_warnings("all")
add_rules("mode.debug", "mode.release")
add_defines("_CRT_SECURE_NO_WARNINGS")
includes("@builtin/xpack")

if is_plat("windows") then
    -- Wrappers de casse (Windows.h -> windows.h) pour la cross-compilation MinGW
    -- (applique a toutes les cibles, y compris vendor/LCUI)
    add_includedirs("wincompat")
    -- Cible Windows 10 pour exposer GetDpiForWindow / GetDpiForSystem
    add_defines("_WIN32_WINNT=0x0A00", "WINVER=0x0A00")
    -- Le code Win32 de LCUI est ecrit pour MSVC : clang rejette certains
    -- transtypages de pointeurs de fonctions par defaut. On les degrade en warnings.
    add_cxflags("-Wno-incompatible-function-pointer-types",
                "-Wno-incompatible-pointer-types",
                "-Wno-unknown-pragmas", {tools = {"clang", "gcc"}})
end
includes("vendor/LCUI/xmake.lua")

target("app")
    if is_plat("windows") then
        -- NB : pas de add_rules("win.sdk.application") : cette regle ajoute
        -- -lstrsafe, inexistant sous MinGW. On force le sous-systeme GUI.
        add_ldflags("-mwindows", {force = true})
        add_files("version.rc")
        -- Libs Win32 liees via #pragma comment (MSVC) dans LCUI :
        -- MinGW les ignore, il faut les declarer explicitement.
        add_syslinks("user32", "gdi32", "imm32")
    end
    set_configdir("include")
    set_rundir("dist")
    add_deps("lcui")
    set_kind("binary")
    add_files("app/**.c")
    -- Couche metier electorale (logique pure, partagee avec l'app console)
    add_files("src/core/*.c")
    add_includedirs("src")

xpack("app")
    set_title("LCUI Quick Start ($(arch))")
    set_description("A minimal LCUI application")
    set_author("i@lc-soft.io")
    set_maintainer("i@lc-soft.io")
    set_copyright("Copyright (C) 2014-present, Liu Chao <i@lc-soft.io>")
    set_licensefile("./LICENSE.txt")
    set_formats("nsis", "zip")
    set_basename("lcui-quick-start-v$(version)")
    add_installfiles("dist/(**)")
    add_targets("app")
    set_bindir("./")
    set_iconfile("logo.ico")
