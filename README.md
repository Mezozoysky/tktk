# tktk - triko toolkit
tktk is a couple of libraries designed to help in solving common game development tasks; it is written in c++.

Status
------
The project is in early stages of development.
So currently tktk provides the entity-component system implementation (tktk-ecs library) and couple of useful types of tktk-util library.

License
-------
tktk is released under the terms of zlib/png license; see full license text in LICENSE.md file or at https://opensource.org/licenses/Zlib

Build
-----
To build libraries:

    cd <path/to/tktk/dir>
    mkdir build
    cd build
    cmake ..
    make

Documentation
-------------
To build documentation:

    cd <path/to/tktk/dir>
    doxygen
