#ifndef INVENTORY_COMPRESSOR_TEST_H
#define INVENTORY_COMPRESSOR_TEST_H
#include "../include/app/inventory_compressor.h"
#include <cassert>
#include <iostream>

// Test the HuffmanTree class independently
void testHuffmanTree() {
    std::cout << "\n=== Testing HuffmanTree Class ===" << std::endl;
    
    // Create and initialize frequency data
    XArrayList<std::pair<char, int>> symbolsFreqs;
    symbolsFreqs.add(std::make_pair('a', 10));
    symbolsFreqs.add(std::make_pair('b', 5));
    symbolsFreqs.add(std::make_pair('c', 2));
    symbolsFreqs.add(std::make_pair('d', 8));
    symbolsFreqs.add(std::make_pair('e', 15));
    
    // 1. Test constructor - create tree with order 3
    HuffmanTree<3> tree;
    
    // 2. Test build method
    std::cout << "Testing build method..." << std::endl;
    tree.build(symbolsFreqs);
    
    // 3. Test generateCodes method
    std::cout << "Testing generateCodes method..." << std::endl;
    xMap<char, std::string> codeTable([](char &key, int tableSize) { 
        return (int)key % tableSize; 
    });
    tree.generateCodes(codeTable);
    
    // Print the generated codes
    std::cout << "Generated Huffman codes:" << std::endl;
    DLinkedList<char> keys = codeTable.keys();
    for (char key : keys) {
        std::cout << "'" << key << "' : " << codeTable.get(key) << std::endl;
    }
    
    // 4. Test decode method
    std::cout << "Testing decode method..." << std::endl;
    // Create a test string by encoding some characters
    std::string encodedStr = "";
    std::string testStr = "abcde";
    for (char c : testStr) {
        encodedStr += codeTable.get(c);
    }
    
    std::cout << "Original: " << testStr << std::endl;
    std::cout << "Encoded: " << encodedStr << std::endl;
    
    std::string decodedStr = tree.decode(encodedStr);
    std::cout << "Decoded: " << decodedStr << std::endl;
    
    // Verify decoding worked correctly
    assert(decodedStr == testStr);
    std::cout << "Huffman tree decoding test passed." << std::endl;
}
#endif /* INVENTORY_COMPRESSOR_TEST_H */