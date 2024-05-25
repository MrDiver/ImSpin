{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
        pkgs.libGL
        pkgs.gcc
        pkgs.gtk3
        pkgs.wayland
        pkgs.libxkbcommon
    ];
    buildInputs = with pkgs; [
        glfw
        glfw-wayland
        xorg.libX11
        xorg.libXcursor
        xorg.libXi
        xorg.libXinerama
        xorg.libXrandr
    ];
    nativeBuildInputs = with pkgs.buildPackages; [
        dbus
        clang
        byacc
        cacert
        cmake
        cmakeCurses
        git
        libffi
        pkg-config
        wayland-scanner
        python3
        (python3.withPackages (python-pkgs: [
            python-pkgs.jinja2
        ]))
    ];
}
