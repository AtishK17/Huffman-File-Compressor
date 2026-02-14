#include<iostream>
#include "huffman.h"
using namespace std;

int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cout << "Usage:\n";
        std::cout << "./huffman encode <input_file> <output_file>\n";
        std::cout << "./huffman decode <input_file> <output_file>\n";
        return 1;
    }

    string mode = argv[1];
    string inputFile = argv[2];
    string outputFile = argv[3];

    if(mode == "encode") {
        encodeFile(inputFile, outputFile);
    }
    else if(mode == "decode") {
        decodeFile(inputFile, outputFile);
    }

    return 0;
}