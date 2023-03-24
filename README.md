# PHREESQL

PHREESQL is a toolkit for efficiently storing the huge amount of data from geochemical speciation simulations run by PHREEQC in an
SQL database. It includes both a C++ library, namely PHREESQLib, and a command-line interface, namely PHREESQLexe, to ease the usability of PHREESQLib by technical experts
with low programming knowledge. Thanks to its relational data structure, PHREESQL allows to be exploited in third-party packages based on and to re-use them for other analyses by external pack-
ages of environmental interest.

## Authors

- Daniela Cabiddu (CNR-IMATI Genova, Italy)
- Marino Vetuschi Zuccolini (DISTAV Genova, Italy)

## Dependences 
- CMake (https://cmake.org/download);
- SQlite (https://www.sqlite.org/download.html);
- IPhreeQC (included as a submodule in *external*);
- PROJ (included as a submodule in *external*);

## How to build (and test)

PHREESQL provides a *build_and_test.sh* script. 
Simply run it to build the toolkit (together with the dependencies), generate the *PhreeSQLexe* tool and run a preliminary test.

## Usage

**PHREESQLib** is an header-only C++ library having IPhreeQC and PROJ as mandatory dependences. To exploit PHREESQLib in your C++ project:
- build the external dependence IPhreeQC (*external/iphreeqc*);
- build the external dependence PROJ (*external/proj-osgeo*);
- include PHREESQLib (*phreesqlib folder*) in your project;
- include and link IPhreeQC and PROJ in your project;
- link SQlite in your project.

**PhreeSQLExe** is an example of usage, and provides the possibility to automatically perform speciation calculation and storage starting from a given dataset. 

## Citing Us

If you use PhreeSQLib in your academic projects, please consider citing the library using the following BibTeX entry:

[BIBTEX REFERENCE AFTER PUBLICATION]

## Acknowledgments
