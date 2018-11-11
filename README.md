# lala [![Build Status](https://travis-ci.org/benvenutti/lala.svg?branch=master)](https://travis-ci.org/benvenutti/lala)

This is a simple cmake template project to link *dynamically* against [libpd](https://github.com/libpd). By running the script `hmb` (**H**elp **M**e **B**uild), *libpd* will be downloaded to a local `3rd-party` folder and built. Then, the `Findlibpd.cmake` module will find the library and link against the main project when configuring the build.

## How to build

```sh
$ python3 hmb/run.py
$ mkdir build && cd build
$ cmake ..
$ cmake --build .
```