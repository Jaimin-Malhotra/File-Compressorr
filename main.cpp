#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <queue>
#include <map>
#include <iterator>

// --- 1. Define the Huffman Tree Node ---
struct MinHeapNode {
    char data;
    unsigned freq;
    MinHeapNode *left, *right;

    MinHeapNode(char data, unsigned freq) {
        left = right = nullptr;
        this->data = data;
        this->freq = freq;
    }
};

// --- 2. Comparison Object for the Priority Queue ---
struct compare {
    bool operator()(MinHeapNode* l, MinHeapNode* r) {
        return (l->freq > r->freq);
    }
};

// --- 3. Function to Generate Huffman Codes (for compression) ---
void generateCodes(struct MinHeapNode* root, std::string str, std::map<char, std::string>& huffmanCodes) {
    if (!root) return;
    if (root->data != '$') {
        huffmanCodes[root->data] = str;
    }
    generateCodes(root->left, str + "0", huffmanCodes);
    generateCodes(root->right, str + "1", huffmanCodes);
}

// --- 4. Function to Build the Huffman Tree (used by both) ---
MinHeapNode* buildHuffmanTree(const std::map<char, int>& freq) {
    std::priority_queue<MinHeapNode*, std::vector<MinHeapNode*>, compare> minHeap;
    for (auto pair : freq) {
        minHeap.push(new MinHeapNode(pair.first, pair.second));
    }

    while (minHeap.size() != 1) {
        MinHeapNode* left = minHeap.top(); minHeap.pop();
        MinHeapNode* right = minHeap.top(); minHeap.pop();

        MinHeapNode* top = new MinHeapNode('$', left->freq + right->freq);
        top->left = left;
        top->right = right;
        minHeap.push(top);
    }
    return minHeap.top();
}


// --- 5. Compression Function ---
void huffmanCompress(const std::string& inputFilename, const std::string& outputFilename) {
    // Step A: Count frequencies
    std::map<char, int> freq;
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl; return;
    }
    char c;
    while (inputFile.get(c)) {
        freq[c]++;
    }
    inputFile.close();
    
    // Step B: Build the tree
    MinHeapNode* root = buildHuffmanTree(freq);

    // Step C: Generate codes
    std::map<char, std::string> huffmanCodes;
    generateCodes(root, "", huffmanCodes);

    // Step D: Write the compressed file
    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl; return;
    }

    // D.1: Write header (frequency table)
    int freqMapSize = freq.size();
    outputFile.write(reinterpret_cast<const char*>(&freqMapSize), sizeof(freqMapSize));
    for (auto const& [key, val] : freq) {
        outputFile.write(reinterpret_cast<const char*>(&key), sizeof(key));
        outputFile.write(reinterpret_cast<const char*>(&val), sizeof(val));
    }

    // D.2: Re-read input and write encoded data
    inputFile.open(inputFilename, std::ios::binary);
    std::string encodedString = "";
    while (inputFile.get(c)) {
        encodedString += huffmanCodes[c];
    }
    inputFile.close();

    // D.3: Pack bit string into bytes
    char buffer = 0;
    int count = 0;
    for (char bit : encodedString) {
        buffer = buffer | (bit - '0') << (7 - count);
        count++;
        if (count == 8) {
            outputFile.write(&buffer, 1);
            buffer = 0;
            count = 0;
        }
    }
    if (count > 0) {
        outputFile.write(&buffer, 1);
    }

    outputFile.close();
    std::cout << "File compressed successfully!" << std::endl;
}

// --- 6. Decompression Function ---
void huffmanDecompress(const std::string& inputFilename, const std::string& outputFilename) {
    std::ifstream inputFile(inputFilename, std::ios::binary);
    if (!inputFile.is_open()) {
        std::cerr << "Error: Could not open input file." << std::endl; return;
    }

    // Step A: Read the header and rebuild the frequency map
    std::map<char, int> freq;
    int freqMapSize;
    inputFile.read(reinterpret_cast<char*>(&freqMapSize), sizeof(freqMapSize));
    for (int i = 0; i < freqMapSize; ++i) {
        char key;
        int val;
        inputFile.read(reinterpret_cast<char*>(&key), sizeof(key));
        inputFile.read(reinterpret_cast<char*>(&val), sizeof(val));
        freq[key] = val;
    }

    // Step B: Rebuild the Huffman Tree from the frequency map
    MinHeapNode* root = buildHuffmanTree(freq);

    // Step C: Read the rest of the file and decode
    std::ofstream outputFile(outputFilename, std::ios::binary);
    if (!outputFile.is_open()) {
        std::cerr << "Error: Could not open output file." << std::endl; return;
    }

    MinHeapNode* curr = root;
    char byte;
    long long totalChars = 0;
    for (auto const& [key, val] : freq) {
        totalChars += val;
    }

    long long charsWritten = 0;
    while (inputFile.get(byte) && charsWritten < totalChars) {
        for (int i = 7; i >= 0; --i) {
            if (charsWritten >= totalChars) break;
            
            int bit = (byte >> i) & 1;
            if (bit == 0) {
                curr = curr->left;
            } else {
                curr = curr->right;
            }

            // If a leaf node is reached
            if (curr->left == nullptr && curr->right == nullptr) {
                outputFile.put(curr->data);
                charsWritten++;
                curr = root; // Go back to the root for the next character
            }
        }
    }

    inputFile.close();
    outputFile.close();
    std::cout << "File decompressed successfully!" << std::endl;
}


// --- 7. The Main Function (Entry point of the program) ---
int main(int argc, char* argv[]) {
    if (argc != 4) {
        std::cerr << "Usage: " << argv[0] << " <-c|-d> <input_file> <output_file>" << std::endl;
        std::cerr << "  -c: Compress" << std::endl;
        std::cerr << "  -d: Decompress" << std::endl;
        return 1;
    }

    std::string flag = argv[1];
    std::string inputFilename = argv[2];
    std::string outputFilename = argv[3];

    if (flag == "-c") {
        huffmanCompress(inputFilename, outputFilename);
    } else if (flag == "-d") {
        huffmanDecompress(inputFilename, outputFilename);
    } else {
        std::cerr << "Invalid flag. Use -c to compress or -d to decompress." << std::endl;
        return 1;
    }

    return 0;
}
