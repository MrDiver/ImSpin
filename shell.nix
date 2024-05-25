{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
        pkgs.libGL
        pkgs.gcc
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
        clang
        byacc
        cacert
        cmake
        git
        libffi
        libxkbcommon
        pkg-config
        wayland
        wayland-scanner
        python3
        (python3.withPackages (python-pkgs: [
            python-pkgs.jinja2
        ]))
    ];
}
