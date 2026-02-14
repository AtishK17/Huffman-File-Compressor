#ifndef HUFFMAN_H
#define HUFFMAN_H

#include<unordered_map>
#include<string>
#include<vector>
#include<bitset>
#include <iostream>
#include <fstream>

using namespace std;

struct Node {
    char ch;
    int freq;
    Node* left;
    Node* right;
    
    Node(char c, int f);
};

typedef unsigned char byte;

struct CompressedData {
    vector<byte> bytes;
    int padding;
};

unordered_map<char, int> buildFrequency(const string& text);
Node* buildHuffmanTree(const unordered_map<char, int>& freq);
void buildCodes(Node* root, string code,
                unordered_map<char, string>& huffmanCode);

string encode(const string& text, unordered_map<char, string>& huffmanCode);

CompressedData pack_bits(const string& bitString);
    
string unpack_bits(const CompressedData& data);

string decode(const string& bitString, Node* root);

void encodeFile(const string& inputFile, const string& outputFile);

void decodeFile(const string& inputFile, const string& outputFile);

#endif