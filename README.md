# Configuration

## TL;DR

A C++ configuration library CCI compliant

## Build

```bash
mkdir build
cd build
```

If you want to use Conan:

```bash
cmake [...] -DENABLE_CONAN=ON
make -j8
```

Otherwise:

```bash
cmake [...]
make -j8
```

## 

## Gcov support

Gcov support is available by activating the option ENABLE_GCOV in cmake (ENABLE_GCOV=ON).
Only compatible with GCC and debug mode must be enabled (forced when cmake is called with ENABLE_GCOV=ON).

## Unit tests

To build tests, enable `BUILD_TESTS` option with cmake:

```bash
cmake [...] -DBUILD_TESTS=ON
```

Then run tests with `make test`

## Doxygen

To build Doxygen doc, enable `BUILD_DOXYGEN` option with cmake:

```bash
cmake [...] -DBUILD_DOXYGEN=ON
```

Then run tests with `make` or `make doxygen`
