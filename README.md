# text-compression
Lossless Text Compression

## Usage
Run `make` to build the program. This will create an executable named `exe`. 

Call `./exe [filename] [-d/-c] [output filename]` to run program.<br />

**filename** is the name of the ASCII text file you want to compress or decompress<br />
**-c** to compress, **-d** to decompress (must be a previously compressed file)<br />
**output filename** is the name you want to give to the output file<br />

Eg.<br />
`./exe harrypotter.txt -c compressedharrypotter` <br />
`./exe compressedharrypotter.txt -d harrypotter` <br /> 
