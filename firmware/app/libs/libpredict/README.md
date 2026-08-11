<h1 align='center'> libpredict </h1>

<h2 align='center'> <strong> A satellite orbit prediction library. </strong> </h2>

This is a fork of **[la1k/libpredict](https://github.com/la1k/libpredict/)** and is inspired by **[philcrump's](https://github.com/philcrump/libpredict/)** fork of the same library. 

## Major differences from la1k's library

This fork is intended to be used on embedded systems, therefore it supports static allocated
structs, instead of the dynamic allocated ones from the original library. Another major 
change is that this fork intends to make the library MISRA-C 2012 compliant, which is 
a requirement for many critical embedded applications, like spacecraft. Because of 
compliance, pretty much every single function of the original library will be modified. 

## MISRA-C 2012 Compliance Progress

The following list contains the status of the library files:

- [X] Headers
- [X] Orbit 
- [ ] Observer ( Partially Complaint )
- [X] SGP4
- [ ] SDP4
- [X] Sun
- [ ] Moon
- [X] Unsorted
- [X] Julian Date
- [ ] Refraction

## Building

To build the library it should be as simple as running:

``` bash
make
```

To specify a custom build directory you can do as follows:

``` bash
make BUILD_DIR=/your/build/dir/
```

To specify a custom toolchain prefix, like the arm-none-eabi, you can do as follows:

``` bash
make TOOLCHAIN_PREFIX=arm-none-eabi- CC_FLAGS_APPEND="-mthumb -mcpu=cortex-m3" 
```

Note that any compiler, linker and ar flags can be appended to the default ones using 
`CC_FLAGS_APPEND`, `LD_FLAGS_APPEND` and `AR_FLAGS_APPEND` variables. The previous 
toolchain example shows how its done aswell.


## Installation

```
make install
```

The install location is defined by `INSTALL_DIR`, which
defaults to `/usr/lib`. To relocate the whole installation (to make
usr/local etc. inside another directory, e.g., if you want to make a
tarball or package it afterwards), use `make INSTALL_DIR=/foo/bar install`.
