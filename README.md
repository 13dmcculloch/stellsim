# stellsim - Symbol mapper

A program that can map binary data to an array of coherent symbols 
to simulate or indeed implement modulation in software. This project is a
hobby, but it may have its uses in SDR when it is finished. It should be an
easily accessible way for arbitrary constellation diagrams to be loaded in
and mapped in software.

#### Current abilities

- load arbitrary constellation diagrams
- map data stream/string to stream of sybmols (`%f,%f`)
- print and draw
- apply amplitude and phase noise (arbitrary units) to symbol sample

#### Benchmark

I measured this with 
`./main -s [setup] | pv --line-mode --rate -lb > /dev/null`.

- with formatted float output: 7.8 Mbps 8PSK
- with `puts("")` output: 75 Mbps 8PSK

Possible improvements include stack lookup table, normalised float output,
less control flow in the mapper function, fewer dereferencing operations
during mapping, and static variables within mapper functions.

#### Larger goals

- write demapper and hence EVM calculator
- write mapper to map binary data from stdin to symbols on output stream
- benchmark tool to measure symbols/sec
- optimisation problems in mapper
- axis scale in `draw`

There are libraries in Python and MATLAB that allow one to do something
similar, but I decided that it would be nice to try making this in C.
And it would be more performant to implement a mapper/modulator in embedded
systems using this language.

### Synopsis

`[main]` enters console.

`[main] [-s filename]` runs the script in `filename` and drops to console.
See script examples in `scripts/`. The syntax is `[command]\n`. There is no
control flow. Scripts are quite useful for setting up the modulation format
and options quickly after executing.

There are two tables of symbols in memory, `lookup`, and `sample`. `lookup`
is the lookup table of the constellation diagram of the working modulation
scheme, while `sample` is a sample of symbols which would have been received.

### Console commands

`exit` cleanly exits the program.

`generate ref BPSK|QPSK|8PSK` constructs reference constellation using values
predefined in .rodata.

`load [type] [filename]` will load a constellation into file where type is:

- `integer` where each value is component squared
- `double`
- `polar` where the polar coords are doubles

Files are CSVs. In `constellations/`, see an exmple of an
`integer` constellation file.

`map [filename]` will map the stream at `filename` onto the modulation format
in memory and currently prints the symbols to stdout.

`maps [string]` is a wrapper for `map` taking a string input. Creates a temp
string in the working directory.

`print(a|c|p) lookup|sample` prints a table of symbol values for the lookup
reference constellation or for the sample in memory.

`draw lookup|sample` will draw an 80x24 diagram of the lookup reference
constellation, as hex values, or the sample in memory, as dots. As the
screen may be only 24 cols high, the cursor will pause at the end of line
24, and pressing ENTER will return to console.

`noise amplitude|phase [g]` destructively applies amplitude or phase noise to
symbols stored in memory. `g` is the gain, a float, unadjusted (for now).
The function is in radians and will be shortly changed to degrees.

`concat [n]` is a debug tool which makes `n` copies of the lookup table and 
stores them in memory. To "restore" a noise-affected sample, rerun concat.

### Notes

There is a problem in `draw` where there is garbage on either side of the
diagram, but only on some systems. For example, it outputs cleanly on the
desktop, but on the laptop, which incidentally uses WSL and, curiously, the
same `$TERM`, there are a few garbage characters on the side. The garbage
characters also show when run in a terminal in `vi` on the desktop in a
similar way. It is likely something to do with the newline in `puts`, or it
would have something to do with the line buffer.

Additionally, `draw` will put one dot for any number of samples. This way,
`concat 1000` would yield more dots than `concat 5` and the effect of the
distribution would not be present visually. To mitigate, I will add to the
`draw` algorithm, but this is not a priority.

Can add flags to makefile to vectorise math operations to use SIMD. 
Makefile currently does not optimise. There should be no mathematical
operations performed during the mapping process, only during setup, so
performance should not be an issue unless there were a continuous EVM
calculation needing to be performed. Demapping will likely be more expensive
than mapping.

