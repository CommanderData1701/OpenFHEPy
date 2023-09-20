# OpenFHEPy
OpenFHEPy is a library that implements machine learning operators for 
homomorphically encrypted inference. It uses the [OpenFHE]() library in order to 
implement these operators in C++ and [Pybind11]() in order to create convinient 
Python wrappers around that.

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
If this is not done in a virtual environment the `make install` command must be 
run with sudo privileges.

## 
