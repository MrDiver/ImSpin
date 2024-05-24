{ pkgs ? import <nixpkgs> {} }:
  pkgs.mkShell {
    # nativeBuildInputs is usually what you want -- tools you need to run
    LD_LIBRARY_PATH = pkgs.lib.makeLibraryPath [
        pkgs.libGL
    ];
    buildInputs = with pkgs; [
        glfw
        glfw-wayland
        xorg.libX11
        xorg.libXrandr
        xorg.libXi
        xorg.libXcursor
        xorg.libXinerama
    ];
    nativeBuildInputs = with pkgs.buildPackages; [
        git
        byacc
        cacert
        gcc
        cmake
        wayland
        libffi
        libxkbcommon
        pkg-config
        wayland-scanner
        python3
        (python3.withPackages (python-pkgs: [
            python-pkgs.jinja2
        ]))
    ];
}
