# stellsim - Modulation format simulator

A program that can modulate binary data to an array of complex double structs
to simulate or indeed implement modulation in software.

can: read - "will be able to"

While a toy dog compared to an ASIC, it is interesting to code a simulator for
this. There are libraries in Python and MATLAB that allow one to do something
similar, but I decided that it would be nice to try making this in C.

### Synopsis

Console under development.

### Commands

`[foo]` denotes an optional argument.

`exit` cleanly exits the program.

`generate dumb M` where M is the modulation order, will generate a modulation
format with no Gray coding. For debugging uses mainly.

`print lookup|sample` prints a table of lookup or sample symbols in memory.

`draw lookup|sample [v]` will draw an 80x24 diagram of the lookup table values
and if `sample`, it will draw the symbols represented as dots. As the diagram is
the same size as the terminal, press `ENTER` to get back to the command line.

`noise amplitude|phase [g]` destructively applies amplitude or phase noise to
symbols stored in memory. `g` is the gain, a float, unadjusted (for now).

`concat n` is a debug tool which makes n copies of the lookup table and stores
them in memory.

### Notes

Can add flags to makefile to vectorise math operations to use MMX, SSE.

