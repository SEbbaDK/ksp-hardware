{ pkgs ? import <nixpkgs> {}
, buildPythonPackage ? pkgs.python3Packages.buildPythonPackage
, fetchPypi ? pkgs.python3Packages.fetchPypi
, fetchzip ? pkgs.fetchzip
}:
buildPythonPackage rec {
    pname = "krpc";
    version = "0.4.8";

    src = fetchzip {
        url = "https://files.pythonhosted.org/packages/b3/21/a2040aac284c04221acb31942d2f78281ce0b01a184762b0e9548803ac60/krpc-0.4.8.zip";
        sha256 = "1a08i59fj2hraabakh812sgdm70kvafv6w6yvw5ffg9yanw88xbq";
    };

    #buildInputs = [ pkgs.python3Packages.protobuf ];
    propagatedBuildInputs = [ pkgs.python3Packages.protobuf ];

    doCheck = false;
}

