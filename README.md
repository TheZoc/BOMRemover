# BOMRemover
Simple command-line application to remove BOM from UTF-8 files encoded with BOM

## How to use

`
BOMRemover <input file> <output file>
`

It will read the `input file`, check if it has the UTF-8 BOM header and, if it has, remove it and save as `output file`.
Simple, hassle-free.

It will warn the user and stop execution if it doesn't have the BOM header.
