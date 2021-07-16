# PHREESQLib
An in-situ real-time speciation calculation and storage tool by means of PHREEQC code (USGS).
It allows water speciation calculations and storage by means of a SQL database. Thanks to the relational data structure and query capabilities, it is possible to efficiently browse, export a large amount of data deriving from speciation computation, and to reuse them for further analyses and plot generation.

## Authors

- Daniela Cabiddu (CNR-IMATI Genova, Italy)
- Marino Vetuschi Zuccolini (DISTAV Genova, Italy)

## Dependences 
- CMake (https://cmake.org/download);
- SQlite (https://www.sqlite.org/download.html);
- IPhreeQC (version 3.7.0 included in *external*).

## Installation
**PHREESQLib** is an header-only C++ library having IPhreeQC as a mandatory dependence. To exploit PHREESQLib in your C++ project:
- build the external dependence IPhreeQC (*external/iphreeqc-3.7.0-15749*);
- include PHREESQLib (*phreesqlib folder*) in your project;
- include and link IPhreeQc in your project;
- link SQlite in your project.

**PhreeSQLTool** is an example of usage, and provides the possibility to automatically perform speciation calculation and storage starting from a given dataset. To build PhreeSQLTool:
```
cd phreesqltool
mkdir build
cd build
cmake ..
make
```
The corresponding executable will be available in *external/phreesqltool/build*.

## Usage

[...]

## Citing Us

If you use PhreeSQLib in your academic projects, please consider citing the library using the following BibTeX entry:

https://www.overleaf.com/5119426148dhjrjfrcqdvj (Draft Paper to be submitted)

[BIBTEX REFERENCE AFTER PUBLICATION]

## Acknowledgments
