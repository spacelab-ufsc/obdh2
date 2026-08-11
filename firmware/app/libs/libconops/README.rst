libconops
=========

A simple library to facilitate the mission control and planning.

.. _building-sec:

Building
--------

    Currently, the library supports both CMake and Meson. How to compile the library using them can be seen below.

Meson
~~~~~
    To build the library with Meson, run the following command in the repo's root::

        meson setup build
        meson compile -C build

CMake
~~~~~
    To build the library with CMake, run the following command in the repo's root::

        mkdir build && cd build
        cmake ..
        make

Tests
-----

    To run the library unit tests, assuming that the commands from `Building <#building>`_ were used, run the following commands.

CMake builds
~~~~~~~~~~~~
     Run in the build directory::

        ctest --output-on-failure

Meson builds
~~~~~~~~~~~~
    Run in the repo's root::

        meson test -C build --print-errorlogs
