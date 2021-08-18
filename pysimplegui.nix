{ pkgs ? import <nixpkgs> {}
, buildPythonPackage ? pkgs.python3Packages.buildPythonPackage
, fetchPypi ? pkgs.python3Packages.fetchPypi
, fetchzip ? pkgs.fetchzip
}:
buildPythonPackage rec {
    pname = "PySimpleGUI";
    version = "4.46.0";

    src = fetchTarball {
        url = "https://files.pythonhosted.org/packages/32/7f/6ad17686dcab93b88a0a1eb02ca3c3feceac4c98b5c3e02ebd7ddbd73b77/PySimpleGUI-4.46.0.tar.gz";
        sha256 = "035fsgvrlrx2ndsjpfw1s30j27rcr2w0gwipz1crv4jxx3iz5hap";
    };

    #buildInputs = [ pkgs.python3Packages.protobuf ];
    #propagatedBuildInputs = [ pkgs.python3Packages.protobuf ];

    doCheck = false;
}

