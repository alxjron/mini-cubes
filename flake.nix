{
  description = "A Nix-flake-based C SDL2 OpenGL development";

  inputs.nixpkgs.url = "github:nixos/nixpkgs/nixos-24.11";

  outputs = { self, nixpkgs }:
    let
      supportedSystems = [ "x86_64-linux" "aarch64-linux" "x86_64-darwin" "aarch64-darwin" ];
      forEachSupportedSystem = f: nixpkgs.lib.genAttrs supportedSystems (system: f {
        pkgs = import nixpkgs { inherit system; };
      });
    in
    {
      devShells = forEachSupportedSystem ({ pkgs }: {
        default = pkgs.mkShell.override
          {
            # Override stdenv in order to change compiler:
            stdenv = pkgs.gccStdenv;
          }
          {
            packages = with pkgs; [
              clang-tools
              codespell
              gnumake
              gcc
              valgrind
              gtest
              lcov

              libGL
              glfw
              cglm
              #glew.dev
              SDL2
              SDL2.dev
              #SDL2_image
            ] ++ (if system == "aarch64-darwin" then [ ] else [ gdb ]);
            
            LIBCLANG_PATH = "${pkgs.libclang.lib}/lib";
          };
      });
    };
}

