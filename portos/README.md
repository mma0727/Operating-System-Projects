PortOS Educational Operating System
___

This is the PortOS user-level threading library project.  Instructions for completing the project can be found on the [CS4411 
course website](https://www.cs.cornell.edu/courses/cs4410/2016sp/CS4411/index.html).

Compiling
=========

To compile an application with the library, you can type `make application`.
This will compile the file `application.c` as well as the PortOS library, and
produce an executable `application`.

You can edit the [`Makefile`](/p1/src/Makefile) file to change what programs are compiled by default
and also what programs are compiled when you type `make all`.

Source Code Overview
====================

This release contains three groups of files:

1. low level system primitives
    - `defs.h`
    - `start.c`
    - `interrupts.*`
    - `machine_primitives.*`
    - `random.*`

2. the threading library itself
    - `minithread.*`
    - `queue.*`
    - `semaphore.*`

3. sample applications
    - `buffer.c`
    - `sieve.c`
    - `test*.c`

_You should not need to edit the system primitives, though you may want to read
the header files!_
