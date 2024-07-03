{
  inputs = {
    nixpkgs.url = "github:nixos/nixpkgs/nixos-unstable";
    OpenSiv3D.url = "github:yadokani389/OpenSiv3D";
  };
  outputs = { nixpkgs, OpenSiv3D, ... }:
    let
      pkgs = import nixpkgs { system = "x86_64-linux"; };
      mandelbrotSet = with pkgs;
        stdenv.mkDerivation {
          pname = "mandelbrotSet";

          version = "0.0.1";

          src = ./.;

          nativeBuildInputs = [ cmake ninja ];

          installPhase = ''
            runHook preInstall

            mkdir -p $out/bin
            cd ..
            cp mandelbrotSet $out/bin
            cp -r resources $out/bin

            runHook postInstall
          '';

          cmakeFlags = [ "-GNinja" "-DCMAKE_BUILD_TYPE=RelWithDebInfo" ];

          buildInputs = [
            OpenSiv3D.packages."x86_64-linux".default
            pkgconf
            zlib
            alsaLib
            ffmpeg
            boost175
            giflib
            mesa
            libmpg123
            opencv
            opusfile
            soundtouch
            libtiff
            libwebp
            util-linux
            xorg.libXft
            xorg.libXdmcp
            xorg.libXtst
            libGLU
            libvorbis
            harfbuzz
            glib
            gtk3
            curl
            pcre
            pcre2
            libselinux
            libthai
            libsepol
            libdatrie
            libxkbcommon
            epoxy
          ];
        };
    in { defaultPackage."x86_64-linux" = mandelbrotSet; };
}
