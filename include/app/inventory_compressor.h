#ifndef INVENTORY_COMPRESSOR_H
#define INVENTORY_COMPRESSOR_H

#include "hash/xMap.h"
#include "heap/Heap.h"
#include "inventory.h"
#include "list/XArrayList.h"
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <string>
#include <utility>

template <int treeOrder>
class HuffmanTree {
public:
    struct HuffmanNode {
        char symbol = '\0';
        int freq;
        int order = 0;
        XArrayList<HuffmanNode *> children;

        HuffmanNode(char s, int f, int o = 0) : symbol(s), freq(f), order(o) {}
        HuffmanNode(int f, const XArrayList<HuffmanNode *> &childs, int o = 0) : freq(f), children(childs), order(o) {}
    };

    HuffmanTree();
    ~HuffmanTree();

    void build(XArrayList<pair<char, int>> &symbolsFreqs);
    void generateCodes(xMap<char, std::string> &table);
    std::string decode(const std::string &huffmanCode);

private:
    HuffmanNode *root;
    //  Helper function to delete all nodes recursively
    void deleteNode(HuffmanNode *node) {
        if (node == nullptr)
            return;

        // Recursively delete all children
        for (int i = 0; i < node->children.size(); i++) {
            deleteNode(node->children.get(i));
        }

        // Delete this node
        delete node;
    }
    static int minHeapComparator(HuffmanNode *&a, HuffmanNode *&b) {
        if (a->freq < b->freq)
            return -1;
        else if (a->freq > b->freq)
            return +1;
        else if (a->order < b->order) {
            return -1;
        } else if (a->order < b->order) {
            return +1;
        } else {
            return 0;
        }
    }
};

template <int treeOrder>
class InventoryCompressor {
public:
    InventoryCompressor(InventoryManager *manager);
    ~InventoryCompressor();

    void buildHuffman();
    void printHuffmanTable();
    std::string productToString(const List1D<InventoryAttribute> &attributes, const std::string &name);
    std::string encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name);
    std::string decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput);

private:
    xMap<char, std::string> *huffmanTable;
    InventoryManager *invManager;
    HuffmanTree<treeOrder> *tree;
};

#endif // INVENTORY_COMPRESSOR_H

template <int treeOrder>
HuffmanTree<treeOrder>::HuffmanTree() {
    root = nullptr; // Initialize the root node to nullptr
}

template <int treeOrder>
HuffmanTree<treeOrder>::~HuffmanTree() {
    // TODO

    // Destructor implementation
    deleteNode(root);
    root = nullptr;
}

template <int treeOrder>
void HuffmanTree<treeOrder>::build(XArrayList<pair<char, int>> &symbolsFreqs) {
    // TODO
    int orderCounter = 0;
    // Create a heap from the symbols and frequencies
    Heap<HuffmanNode *> minHeap(minHeapComparator);

    // Check if we need to add dummy characters
    int leafCount = symbolsFreqs.size();
    int dummyCount = 0;

    // Calculate how many dummy characters we need to add
    // We need (leafCount - 1) % (treeOrder - 1) == 0
    // If not satisfied, add dummy nodes until it is
    // Special case: if there's only one symbol, we need (treeOrder-1) dummy nodes
    if (leafCount == 1) {
        dummyCount = treeOrder - 1;
    } else if ((leafCount - 1) % (treeOrder - 1) != 0) {
        dummyCount = (treeOrder - 1) - ((leafCount - 1) % (treeOrder - 1));
    }

    for (int i = 0; i < dummyCount; ++i) {
        HuffmanNode *node = new HuffmanNode('\0', 0, orderCounter++);
        minHeap.push(node);
    }

    // Add all symbols and their frequencies to the heap
    for (int i = 0; i < symbolsFreqs.size(); i++) {
        HuffmanNode *node = new HuffmanNode(symbolsFreqs.get(i).first, symbolsFreqs.get(i).second, orderCounter++);
        minHeap.push(node);
    }

    // While there is more than one node in the heap
    while (minHeap.size() > 1) {
        // Take at most treeOrder nodes with lowest frequency
        XArrayList<HuffmanNode *> children;
        int totalFreq = 0;

        for (int i = 0; i < treeOrder; i++) {
            HuffmanNode *node = minHeap.pop();
            children.add(node);
            totalFreq += node->freq;
        }

        // Create a new internal node with the combined frequency
        HuffmanNode *internal = new HuffmanNode(totalFreq, children, orderCounter++);

        // Push the internal node back to the heap
        minHeap.push(internal);
    }

    // The last remaining node is the root
    root = minHeap.pop();
}

template <int treeOrder>
void HuffmanTree<treeOrder>::generateCodes(xMap<char, std::string> &table) {
    // TODO
    // Clear the existing table
    table.clear();
    // Generate the Huffman codes and store them in the table
    if (!root)
        return;
    auto f = [&table](auto &ref, HuffmanNode *node, const std::string &code) {
        if (!node)
            return;
        if (node->children.size() == 0) {
            table.put(node->symbol, code);
        } else {
            for (int i = 0; i < node->children.size(); ++i) {
                ref(ref, node->children.get(i), code + ((i < 10) ? std::to_string(i) : std::string(1, 'a' + i - 10)));
            }
        }
    };

    // Start recursive code generation from the root with an empty code
    f(f, root, "");
}

template <int treeOrder>
std::string HuffmanTree<treeOrder>::decode(const std::string &huffmanCode) {
    // TODO
    std::stringstream result;
    if (!root)
        return "";

    HuffmanNode *currentNode = root;
    for (char digit : huffmanCode) {
        // Convert the character digit to integer (hexadecimal)
        int index;
        if (digit >= '0' && digit <= '9') {
            index = digit - '0';
        } else if (digit >= 'a' && digit <= 'f') {
            index = digit - 'a' + 10;
        } 
        // else {
        //     throw std::runtime_error("Invalid character in Huffman code");
        // }

        // Check if index is valid for this node's children
        // if (index < 0 || index >= currentNode->children.size()) {
        //     throw std::runtime_error("Invalid Huffman code index");
        // }

        // Move to the child node indicated by the digit
        currentNode = currentNode->children.get(index);

        // If we've reached a leaf node (no children), add its symbol to the result
        if (currentNode->children.size() == 0) {
            // Skip dummy nodes (with symbol '\0')
            // if (currentNode->symbol != '\0') {
            //     result << currentNode->symbol;
            // }
            result << currentNode->symbol;
            // Reset to root for the next symbol
            currentNode = root;
        }
    }

    // If we didn't end at the root, the code is invalid
    if (currentNode != root) {
        // throw std::runtime_error("Incomplete Huffman code");
    }

    return result.str();
}

template <int treeOrder>
InventoryCompressor<treeOrder>::InventoryCompressor(InventoryManager *manager) : invManager(manager) {
    // TODO
    huffmanTable = new xMap<char, std::string>([](char &key, int tableSize) { return (int)key % tableSize; });
    tree = new HuffmanTree<treeOrder>();
}
template <int treeOrder>
InventoryCompressor<treeOrder>::~InventoryCompressor() {
    // TODO
    if (huffmanTable) {
        delete huffmanTable;
        huffmanTable = nullptr;
    }
    if (tree) {
        delete tree;
        tree = nullptr;
    }
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::buildHuffman() {
    // TODO
    // Create a frequency table for all characters in product strings
    // Create an array of 256 elements to store frequency counts for all ASCII characters
    int freqArray[256] = {0};

    // Get all products from the inventory manager
    List2D<InventoryAttribute> products = invManager->getAttributesMatrix();
    List1D<std::string> names = invManager->getProductNames();

    // (a) Process each product to count character frequencies
    for (int i = 0; i < products.rows(); ++i) {
        // Convert product to string representation
        std::string productStr = productToString(products.getRow(i), names.get(i));

        // Count frequency of each character
        for (char ch : productStr) {
            // Increment the count for this character's ASCII value
            freqArray[(unsigned char)ch]++;
        }
    }

    // Prepare data for Huffman tree construction
    XArrayList<pair<char, int>> symbolsFreqs;
    
    // Add characters with non-zero frequencies to symbolsFreqs in ASCII order
    for (int i = 0; i < 256; ++i) {
        if (freqArray[i] > 0) {
            symbolsFreqs.add({(char)i, freqArray[i]});
        }
    }

    // (b) Build the Huffman tree and generate the codes
    tree->build(symbolsFreqs);
    tree->generateCodes(*huffmanTable);
}

template <int treeOrder>
void InventoryCompressor<treeOrder>::printHuffmanTable() {
    DLinkedList<char> keys = huffmanTable->keys();
    for (char ch : keys) {
        std::cout << "'" << ch << "' : " << huffmanTable->get(ch) << std::endl;
    }
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::productToString(const List1D<InventoryAttribute> &attributes, const std::string &name) {
    // TODO
    std::stringstream ss;
    // Add the product name followed by colon
    ss << name << ":";

    // Add each attribute in the format (name:value)
    for (int i = 0; i < attributes.size(); i++) {
        // Get the current attribute
        InventoryAttribute attr = attributes.get(i);

        // Add the attribute in the format (name:value)
        // ss << "(" << attr.name << ":" << to_string(attr.value) << ")";
        ss << "(" << attr << ")";

        // Add comma and space if not the last attribute
        if (i < attributes.size() - 1) {
            ss << ", ";
        }
    }

    return ss.str();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::encodeHuffman(const List1D<InventoryAttribute> &attributes, const std::string &name) {
    // TODO
    // Convert the product to a string representation
    std::string productStr = productToString(attributes, name);
    
    // Encode the string using the Huffman table
    std::stringstream encodedStream;
    for (char ch : productStr) {
        // Check if the character exists in the huffman table
        if (huffmanTable->containsKey(ch)) {
            encodedStream << huffmanTable->get(ch);
        } else {
            // Handle characters not found in table
            throw std::runtime_error("Character not found in Huffman table: " + std::string(1, ch));
        }
    }

    return encodedStream.str();
}

template <int treeOrder>
std::string InventoryCompressor<treeOrder>::decodeHuffman(const std::string &huffmanCode, List1D<InventoryAttribute> &attributesOutput, std::string &nameOutput) {
    // TODO
    // (a) Decode the Huffman code using the Huffman tree
    std::string decodedString = tree->decode(huffmanCode);
    
    // (b) Parse the decoded string to extract product name and attributes
    // while (attributesOutput.size() > 0) {
    //     attributesOutput.removeAt(attributesOutput.size() - 1);
    // }
    nameOutput = "";

    // Find the position of the first colon which separates name from attributes
    size_t colonPos = decodedString.find(':');
    // if (colonPos == std::string::npos) {
    //     throw std::runtime_error("Invalid format in decoded string: missing colon");
    // }

    // Extract the name part
    nameOutput = decodedString.substr(0, colonPos);

    // Process the attributes part
    std::string attributesStr = decodedString.substr(colonPos + 1);
    size_t pos = 0;
    while ((pos = attributesStr.find('(', pos)) != std::string::npos) {
        // Find the end of the current attribute
        size_t endPos = attributesStr.find(')', pos);
        if (endPos == std::string::npos) {
            throw std::runtime_error("Invalid format in decoded string: missing closing parenthesis");
        }

        // Extract the attribute string
        std::string attrStr = attributesStr.substr(pos + 1, endPos - pos - 1);

        // Find the colon in the attribute
        size_t attrColonPos = attrStr.find(':');
        if (attrColonPos == std::string::npos) {
            throw std::runtime_error("Invalid attribute format: missing colon");
        }

        // Parse name and value
        std::string attrName = attrStr.substr(0, attrColonPos);
        std::string attrValue = attrStr.substr(attrColonPos + 1);

        // Add to attributes list - convert attrValue from string to double
        attributesOutput.add(InventoryAttribute(attrName, std::stod(attrValue)));

        // Move past this attribute
        pos = endPos + 1;
    }

    // (c) Return the decoded string
    return decodedString;
}