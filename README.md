# stellsim - Modulation format simulator

A program that can modulate binary data to an array of complex double structs
to simulate or indeed implement modulation in software.

While a toy dog compared to an ASIC, it is interesting to code a simulator for
this. There are libraries in Python and MATLAB that allow one to do something
similar, but I decided that it would be nice to try making this in C.

### Synopsis

Usage: `stellsim -g d -M 16 -c`
- generates a non-gray coded constellation diagram for 16-QAM and drops to
console
- `-c` may be replaced with `-t` to get a table of lookup table constellation
points, or with `-a` to plot them on a graph (`-a[rgand]`)

### Commands

TBC

### Notes

Can add flags to makefile to vectorise math operations to use MMX, SSE.

