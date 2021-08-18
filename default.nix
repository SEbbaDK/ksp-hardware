{ pkgs ? import <nixpkgs> {}
, mkDerivation ? pkgs.stdenv.mkDerivation
, python3 ? pkgs.python3
}:

let
	packages = pypkgs: [
    	pypkgs.pyserial
    	(import ./krpc.nix { inherit pkgs; })
    	pypkgs.tkinter
    	(import ./pysimplegui.nix { inherit pkgs; })
	];
	python = python3.withPackages packages;
in

mkDerivation {
    name = "ksp-controller";
    src = ./.;
    buildInputs = [ python ];
}
