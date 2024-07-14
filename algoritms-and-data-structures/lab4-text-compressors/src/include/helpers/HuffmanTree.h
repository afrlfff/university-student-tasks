#pragma once

#include <string>
#include <vector>
#include <map>
#include <utility> // for std::pair
#include <list>

#include "BinaryUtils.h"

// START

struct HuffmanNode {
    std::u32string chars;
    double freq;
    HuffmanNode *left, *right;
    int height;

    HuffmanNode() = default;
    HuffmanNode(const std::u32string chars, double freq, HuffmanNode* left, HuffmanNode* right, int height) : 
        chars(chars), freq(freq), left(left), right(right), height(height) {}
    HuffmanNode(const char32_t c, double freq, HuffmanNode* left, HuffmanNode* right, int height) : 
        chars(std::u32string(1, c)), freq(freq), left(left), right(right), height(height) {}
    bool operator<(const HuffmanNode& other) const {
        if ((freq < other.freq) && (other.freq - freq < 0.0000001)) {
            return true;
        }
        return false;
    }
};

class HuffmanTree {
private:
    void fillHuffmanCodesMap(HuffmanNode* node, const std::string& currentCode, std::map<char32_t, std::string>& huffmanCodes) {
        if (node->left == nullptr) { // also means that node.right == nullptr
            // write the code of current character
            huffmanCodes[node->chars[0]] = currentCode;
            return;
        }
        fillHuffmanCodesMap(node->left, currentCode + "0", huffmanCodes);
        fillHuffmanCodesMap(node->right, currentCode + "1", huffmanCodes);
    }
    void fillHuffmanCodesCanonicalTuple(HuffmanNode* node, const std::string& currentCode, std::vector<std::tuple<char32_t, size_t, std::string>>& huffmanCodesCanonicalTuple) {
        if (node->left == nullptr) { // also means that node.right == nullptr
            // write the code of current character
            huffmanCodesCanonicalTuple.push_back(std::make_tuple(node->chars[0], currentCode.size(), currentCode));
            return;
        }
        fillHuffmanCodesCanonicalTuple(node->left, currentCode + "0", huffmanCodesCanonicalTuple);
        fillHuffmanCodesCanonicalTuple(node->right, currentCode + "1", huffmanCodesCanonicalTuple);
    }
    HuffmanNode* root;
public:
    HuffmanTree(HuffmanNode* root) : root(root) {}
    int GetHeight() { return root->height; }
    friend std::map<char32_t, std::string> GetHuffmanCodesMap(HuffmanTree& tree, const size_t& alphabetSize);
    friend std::vector<std::tuple<char32_t, size_t, std::string>> GetHuffmanCodesCanonicalTuple(HuffmanTree& tree, const size_t& alphabetSize);
    void clearNode(HuffmanNode* node) {
        if (node->left != nullptr) {
            clearNode(node->left);
        } if (node->right != nullptr) {
            clearNode(node->right);
        }
        delete node;
    }
    ~HuffmanTree() { clearNode(root); }
};

HuffmanTree BuildHuffmanTree(const std::vector<std::pair<char32_t, double>>& sortedCharFrequencies, size_t alphabetSize) {
    std::list<HuffmanNode*> freeNodesList;

    // fill freeNodesVector
    for (size_t i = 0; i < alphabetSize; ++i) {
        freeNodesList.push_back(new HuffmanNode(sortedCharFrequencies[i].first, sortedCharFrequencies[i].second, nullptr, nullptr, 1));
    }

    HuffmanNode *left, *right, *parent;
    
    // special case
    if (alphabetSize == 1) {
        parent = new HuffmanNode(sortedCharFrequencies[0].first, 1.0, nullptr, nullptr, 1);
    } else if (alphabetSize == 0) {
        parent = new HuffmanNode(' ', 0.0, nullptr, nullptr, 0);
    }
    // build Huffman tree
    while(freeNodesList.size() > 1) {
        left = freeNodesList.front(); freeNodesList.pop_front();
        right = freeNodesList.front(); freeNodesList.pop_front();
        parent = new HuffmanNode(left->chars + right->chars, left->freq + right->freq, left, right, std::max(left->height, right->height) + 1);

        std::list<HuffmanNode*>::iterator it;
        // insert parent into freeNodes
        for (it = freeNodesList.begin(); it != freeNodesList.end(); ++it) {
            if (*parent < **it) {
                // insert part before elem
                if (it == freeNodesList.begin()) {
                    freeNodesList.push_front(parent);
                } else {
                    freeNodesList.insert(--it, parent);
                }
                break;
            }
        }
        // if parent should be inserted at the end
        if (it == freeNodesList.end()) {
            freeNodesList.push_back(parent);
        }
    }

    return HuffmanTree(parent);
}

std::map<char32_t, std::string> GetHuffmanCodesMap(HuffmanTree& tree, const size_t& alphabetSize) {
    std::map<char32_t, std::string> huffmanCodesMap;
    
    if (alphabetSize == 1) {
        // special case
        huffmanCodesMap[tree.root->chars[0]] = "0";
    } else {
        tree.fillHuffmanCodesMap(tree.root, "", huffmanCodesMap);
    }

    return huffmanCodesMap;
}

std::vector<std::tuple<char32_t, size_t, std::string>> GetHuffmanCodesCanonicalTuple(HuffmanTree& tree, const size_t& alphabetSize) {
    std::vector<std::tuple<char32_t, size_t, std::string>> huffmanCodesCanonicalTuple;
    huffmanCodesCanonicalTuple.reserve(alphabetSize);
    
    if (alphabetSize == 1) {
        // special case
        huffmanCodesCanonicalTuple.push_back(std::make_tuple(tree.root->chars[0], 1, "0"));
    } else {
        tree.fillHuffmanCodesCanonicalTuple(tree.root, "", huffmanCodesCanonicalTuple);
    }

    // sort tuple by length of huffman codes
    std::sort(huffmanCodesCanonicalTuple.begin(), huffmanCodesCanonicalTuple.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) < std::get<1>(b);
    });

    // remake this tuple using length of codes we got in huffman tree
    // to be able to decode it using only length of codes
    std::set<std::string> usingCodes;
    std::string binRepresentation; // temporary variable
    std::string temp; // temporary variable
    bool isCodeUsed;
    for (size_t i = 0; i < huffmanCodesCanonicalTuple.size(); ++i) {
        size_t lengthOfCode = std::get<1>(huffmanCodesCanonicalTuple[i]);
        // find the code of length = lengthOfCode
        // which has the smallest number (considering the code as the binary representation)
        // which was not used before and which satisfy the huffman rule
        for (size_t j = 0; j < 1000000000; ++j) {
            // find the code of length = lengthOfCode
            // which was not used before
            binRepresentation = GetBinaryStringFromNumber(j, lengthOfCode);
            if (usingCodes.find(binRepresentation) == usingCodes.end()) {
                // check if the code satisfies the huffman rule
                // (if there are no any codes from which the current code begins)
                isCodeUsed = false;
                for (size_t k = lengthOfCode - 1; k > 0; --k) {
                    temp = binRepresentation.substr(0, k);
                    if (usingCodes.find(temp) != usingCodes.end()) {
                        isCodeUsed = true;
                        break;
                    }
                }
                if (!isCodeUsed) {
                    huffmanCodesCanonicalTuple[i] = std::make_tuple(std::get<0>(huffmanCodesCanonicalTuple[i]), lengthOfCode, binRepresentation);
                    usingCodes.insert(binRepresentation);
                    break;
                }
            }
        }
    }

    return huffmanCodesCanonicalTuple;
}

// END