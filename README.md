[![Build Status](https://ahmad88me.semaphoreci.com/badges/tada-hdt-entity/branches/master.svg?style=shields&key=8f64e452-9983-4d20-85a4-94e1e74028aa)](https://ahmad88me.semaphoreci.com/projects/tada-hdt-entity)
[![codecov](https://codecov.io/gh/oeg-upm/tada-hdt-entity/branch/master/graph/badge.svg)](https://codecov.io/gh/oeg-upm/tada-hdt-entity)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.3732626.svg)](https://doi.org/10.5281/zenodo.3732626)

**Annotate entity and textual columns using HDT**

# TADA HDT Entity

This library offers several techniques to annotate entity and textual properties. It user HDT-compressed knowledge base as the training set to annotated a given table. 


# requirements
* [easy-logger](https://github.com/ahmad88me/easy-logger)
* [tabular-parser](https://github.com/ahmad88me/tabular-parser)
* [hdt-cpp](https://github.com/rdfhdt/hdt-cpp)
* [getopt](https://github.com/ahmad88me/getoptpp)

# Compile
We offer two options to compile:

## Make
```
make
```
## CMake
```
mkdir build
cd build
cmake ..
make
```

## Tests
inside the `build` directory
```
./tests
```

# Technical documentation
The technical documentation can be found on [https://oeg-upm.github.io/tada-hdt-entity](https://oeg-upm.github.io/tada-hdt-entity)
