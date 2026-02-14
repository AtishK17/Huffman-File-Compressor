#include "huffman.h"
#include<queue>
#include<algorithm>
using namespace std;

Node :: Node(char c, int f) {
    ch = c;
    freq = f;
    left = right = nullptr;
};

struct Compare {
    bool operator()(Node* l, Node* r) {
        if (l->freq == r->freq)
            return l->ch > r->ch;
        return l->freq > r->freq;
    }
};

unordered_map<char, int> buildFrequency(const string& text) {
    unordered_map<char, int> freq;
    for(char ch : text)  {
        freq[ch]++;
    }
    return freq;
}

Node* buildHuffmanTree(const unordered_map<char, int>& freq) {
    priority_queue<Node*, vector<Node*>, Compare> pq;
    vector<pair<char,int>> freqVec(freq.begin(), freq.end());
    sort(freqVec.begin(), freqVec.end());

    for(auto& p : freqVec) {
        pq.push(new Node(p.first, p.second));
    }
    while(pq.size() != 1) {
        Node* left = pq.top();
        pq.pop();
        Node* right = pq.top();
        pq.pop();
        Node* parent = new Node('\0', left->freq + right->freq);
        parent->left = left;
        parent->right = right;
        pq.push(parent);
    }
    Node* root = pq.top();
    pq.pop();
    return root;
}

void buildCodes(Node* root, string code, unordered_map<char, string>& huffmanCode) {
    if(root == NULL) return;
    if(!root->left && !root->right) {
        huffmanCode[root->ch] = code.empty() ? "0" : code;
    }
    buildCodes(root->left, code+"0", huffmanCode);
    buildCodes(root->right, code+"1", huffmanCode);
}

string encode(const string& text, unordered_map<char, string>& huffmanCode) {
    string bitString;
    for(char ch : text) {
        bitString += huffmanCode[ch];
    }
    return bitString;
}

CompressedData pack_bits(const string& bitString) {
    vector<byte> bytes;
    long long n = bitString.length();
    int padding = (8 - (n%8)) % 8;
    byte currentByte = 0;
    int bitCount = 0;
    for(char ch : bitString) {
        currentByte = (currentByte << 1) | (ch - '0');
        bitCount++;
        if(bitCount == 8) {
            bytes.push_back(currentByte);
            currentByte = 0;
            bitCount = 0;
        }
    }
    if(padding > 0) {
        currentByte <<= padding;
        bytes.push_back(currentByte);
    }
    return {bytes, padding};
}

string unpack_bits(const CompressedData& data) {
    string bitString = "";
    for(size_t i = 0; i < data.bytes.size(); i++) {
        bitset<8> b(data.bytes[i]);
        bitString += b.to_string();
    }
    return bitString.substr(0, bitString.length() - data.padding);
}

void freeTree(Node* root) {
    if(!root) return;
    freeTree(root->left);
    freeTree(root->right);
    delete root;
}

string decode(const string& bitString, Node* root) {
    string decoded = "";
    Node* curr = root;
    for(char c : bitString) {
        if(c == '1') curr = curr->right;
        else curr = curr->left;

        if(!curr->left && !curr->right) {
            decoded += curr->ch;
            curr = root;
        }
    }
    return decoded;
}

void encodeFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if(!in) {
        cerr << "Error opening input file\n";
        return;
    }

    string text((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();
    if(text.empty()) {
        cerr << "Empty file\n";
        return;
    }

    auto freq = buildFrequency(text);
    Node* root = buildHuffmanTree(freq);
    unordered_map<char, string> codes;
    buildCodes(root, "", codes);

    string bitString = encode(text, codes);
    CompressedData packed = pack_bits(bitString);

    ofstream out(outputFile, ios::binary);
    if(!out) {
        cerr << "Error creating output file\n";
        return;
    }

    out.write(reinterpret_cast<char*>(&packed.padding), sizeof(packed.padding));

    int tableSize = freq.size();
    out.write(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

    for(auto& p : freq) {
        out.write(&p.first, sizeof(p.first));
        out.write(reinterpret_cast<char*>(&p.second), sizeof(p.second));
    }

    out.write(reinterpret_cast<char*>(packed.bytes.data()), packed.bytes.size());
    out.close();

    freeTree(root);

    cout << "Compression Successfull.\n";
}

void decodeFile(const string& inputFile, const string& outputFile) {
    ifstream in(inputFile, ios::binary);
    if(!in) {
        cerr << "Error opening compressed file\n";
        return;
    }

    int padding;
    in.read(reinterpret_cast<char*>(&padding), sizeof(padding));
    int tableSize;
    in.read(reinterpret_cast<char*>(&tableSize), sizeof(tableSize));

    unordered_map<char, int> freq;

    for(int i = 0; i < tableSize; i++) {
        char ch;
        int frequency;
        in.read(&ch, sizeof(ch));
        in.read(reinterpret_cast<char*>(&frequency), sizeof(frequency));
        freq[ch] = frequency;
    }

    Node* root = buildHuffmanTree(freq);

    vector<byte> bytes((istreambuf_iterator<char>(in)), istreambuf_iterator<char>());
    in.close();

    CompressedData data{bytes, padding};

    string bitString = unpack_bits(data);
    string decoded = decode(bitString, root);

    ofstream out(outputFile, ios::binary);
    out.write(decoded.c_str(), decoded.size());
    out.close();

    freeTree(root);

    cout << "Decompression successful.\n";
}