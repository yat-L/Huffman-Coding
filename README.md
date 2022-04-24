# Huffman Coding

* This is an implementation of [Huffman Coding](https://en.wikipedia.org/wiki/Huffman_coding).
* Execute `make` to compile, this will compile both static and dynamic Huffman coding.
* For Static Huffman Coding:
  * `./huff-static c inputFile compressedFile` to compress inputFile into compressedFile.
  * `./huff-static d compressedFile decompressedFile` to decompress compressedFile into decompressedFile.
* For Adaptive Huffman Coding:
  * `./huff-adap c inputFile compressedFile` to compress inputFile into compressedFile.
  * `./huff-adap d compressedFile decompressedFile` to decompress compressedFile into decompressedFile.
* execute `make clean` to delete the executable
