# Huffman File Compressor (C++)

A command-line based file compressor and decompressor implemented in C++ using the Huffman Coding algorithm.

This project performs real lossless compression by:
- Generating Huffman codes based on character frequencies
- Packing bits into bytes
- Storing metadata for accurate decompression
- Reconstructing the original file perfectly

---

## Features

- Lossless compression
- Binary-safe (works with text and binary files)
- Bit-level packing
- Deterministic Huffman tree construction
- Padding handling
- Command-line interface
- Compression ratio measurement (optional)

---

## How It Works

1. Reads file in binary mode
2. Builds frequency table
3. Constructs Huffman tree
4. Generates prefix codes
5. Encodes data into bitstream
6. Packs bits into bytes
7. Stores:
   - Padding information
   - Frequency table
   - Compressed bytes

During decompression:
1. Reads metadata
2. Rebuilds Huffman tree
3. Unpacks bits
4. Decodes original file

---

## Build Instructions

### Windows (MinGW)

```
g++ main.cpp huffman.cpp -o huffman.exe
```

### Linux / macOS

```
g++ main.cpp huffman.cpp -o huffman
```

---

## Usage

### Compress

```
huffman.exe encode input.txt compressed.huff
```

### Decompress

```
huffman.exe decode compressed.huff restored.txt
```

---

## Verify Correctness (Windows)

```
fc input.txt restored.txt
```

If no differences are shown, decompression is successful.

---

## Example Compression Ratio

For typical English text files, compression ratio ranges between:
- < 50% reduction

Results depend on:
- File size
- Character distribution
- Redundancy in data

---

## Limitations

- Does not implement canonical Huffman coding
- Metadata size may impact very small files
- No GUI (CLI only)

---

## Concepts Used

- Greedy algorithms
- Min-heap (priority queue)
- Binary trees
- Bit manipulation
- File I/O in binary mode
- Data compression theory

---

## Author

- Atish Kundu
---
