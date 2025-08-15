C++ Huffman File Compressor
A simple, efficient command-line tool written in C++ to compress and decompress files using the Huffman coding algorithm. This project provides a from-scratch implementation of the Huffman algorithm, focusing on clarity and correctness.

🚀 Live Web Version
Experience the Huffman algorithm in action without any setup! This project includes a fully interactive web-based version that runs entirely in your browser.

💻 Runs Entirely in JavaScript: No installation needed.

📁 Drag & Drop Interface: Easily upload your files to compress.

📊 See Instant Results: Watch the compression ratio and size reduction in real-time.

➡️ Try the Live Compressor Here!

Features
Lossless Compression: Reduces file size without any loss of data.

Header-based Decompression: The compressed file includes a frequency table header, allowing it to be self-contained and easily decompressed.

Command-Line Interface: Simple and intuitive commands to either compress or decompress files.

Cross-Platform: Written in standard C++, it can be compiled and run on Windows, macOS, and Linux.

Getting Started (C++ Command-Line Version)
Prerequisites
You need a C++ compiler installed on your system. The g++ compiler is recommended.

Windows: Install MinGW-w64.

macOS: Run xcode-select --install in your terminal.

Linux: Run sudo apt-get install build-essential (for Debian/Ubuntu).

Step 1: Compile the Program
First, compile the main.cpp source code to create an executable file. Open your terminal in the project directory and run the following command:

g++ main.cpp -o huffman_tool

This will create an executable file named huffman_tool (or huffman_tool.exe on Windows).

Step 2: Compress a File
To compress a file, use the -c flag followed by the input and output filenames.

Syntax:

./huffman_tool -c <input_file> <output_compressed_file>

Example:

./huffman_tool -c my_document.txt compressed.huff

This command reads my_document.txt and creates a smaller, compressed file named compressed.huff.

Step 3: Decompress a File
To decompress a file and restore it to its original state, use the -d flag.

Syntax:

./huffman_tool -d <compressed_file> <output_restored_file>

Example:

./huffman_tool -d compressed.huff restored_document.txt

This command reads compressed.huff and creates restored_document.txt, which will be identical to the original file.
