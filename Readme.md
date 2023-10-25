# OpenFHEPy

OpenFHEPy is a library that implements machine learning operators for
homomorphically encrypted inference. It uses the [OpenFHE](https://github.com/pybind/pybind11) library in order to
implement these operators in C++ and [Pybind11](https://github.com/openfheorg/openfhe-development) in order to create convinient
Python wrappers around that. It was mainly implemented to 

## Dependencies

- [Pybind11](https://github.com/pybind/pybind11)
- [OpenFHE](https://github.com/openfheorg/openfhe-development)
- [CMake 3.12 or newer](https://cmake.org/)

## Installation

The project is set up as using CMake, so the project can be installed by executing the following commands

```
mkdir build && cd build
cmake ..
make && make install
```

It is recommended to do this with an active virtual environment.

## Directory structure
