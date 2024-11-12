#pragma once

#include <map>
#include <set>
#include <algorithm>
#include <utility> // for std::pair
#include <list>

#include "BinaryUtils.h"
#include "Array.h"
#include "StringL.h"


/**
 * HuffmanTree.
 * 
 * Brief:
 * - Class defines HuffmanTree object which has methods to build and huffman tree and get corresponding huffman codes
 * 
 * Parameters:
 * - charType - The type of the characters in the alphabet (char, char16_t/wchar_t, char32_t).
 * 
 * Memory usage:
 * ...
 * 
 */
template <typename charType>
class HuffmanTree {
public:
    struct CanonicalCode {
        charType character;
        uint32_t codeLength;
        uint32_t code; // code in decimal representation

        CanonicalCode() = default;
        CanonicalCode(const charType _character, const uint32_t _codeLength, const uint32_t& _code) :
            character(_character), codeLength(_codeLength), code(_code) {}
    };
private:
    struct HuffmanNode {
        StringL<charType> chars;
        uint32_t freq;
        HuffmanNode *left, *right;

        HuffmanNode(const StringL<charType>& chars, const uint32_t freq, HuffmanNode* left, HuffmanNode* right) : 
            chars(chars), freq(freq), left(left), right(right) {}
        HuffmanNode(const charType c, const uint32_t freq, HuffmanNode* left, HuffmanNode* right) : 
            chars(StringL<charType>(1, c)), freq(freq), left(left), right(right) {}
        bool operator<(const HuffmanNode& other) const {
            if (freq < other.freq)
                return true;
            return false;
        }
    };

    void fillCanonicalCodes(HuffmanNode* node, const uint32_t codeLength, const uint32_t currentCode, Array<CanonicalCode>& canonicalCodes);
    void clearNode(HuffmanNode* node);
    HuffmanNode* root;
public:
    HuffmanTree() = default;
    HuffmanTree(HuffmanNode* root) : root(root) {}
    HuffmanTree(const Array<charType>& alphabet, const Array<uint32_t>& frequencies);
    std::map<charType, StringL<char>> GetCodesMap(HuffmanTree<charType>& tree, const size_t& alphabetSize);
    Array<CanonicalCode> GetCanonicalCodes(HuffmanTree& tree, const size_t alphabetSize);
    void Clear() { clearNode(root); }
    ~HuffmanTree() { clearNode(root); }
};


// START IMPLEMENTATION

template <typename charType>
HuffmanTree<charType>::HuffmanTree(const Array<charType>& alphabet, const Array<uint32_t>& frequencies)
{
    // This constructor build Huffman tree

    // fill freeNodesVector
    std::list<HuffmanNode*> freeNodesList;
    for (size_t i = 0; i < alphabet.size(); ++i) {
        freeNodesList.push_back(new HuffmanNode(alphabet[i], frequencies[i], nullptr, nullptr));
    }

    HuffmanNode *left, *right, *parent;
    
    if (alphabet.size() == 0) {
        // special case
        parent = new HuffmanNode(' ', 0, nullptr, nullptr);
    } else if (alphabet.size() == 1) {
        // special case
        parent = new HuffmanNode(alphabet[0], frequencies[0], nullptr, nullptr);
    } else {
        while (freeNodesList.size() > 1) {
            left = freeNodesList.front(); freeNodesList.pop_front();
            right = freeNodesList.front(); freeNodesList.pop_front();
            parent = new HuffmanNode(left->chars + right->chars, left->freq + right->freq, left, right);

            bool inserted = false;
            // insert parent into freeNodes
            for (auto it = freeNodesList.begin(); it != freeNodesList.end(); ++it) {
                if (*parent < **it) {
                    // insert part before it
                    if (it == freeNodesList.begin()) {
                        freeNodesList.push_front(parent);
                    } else {
                        freeNodesList.insert(--it, parent);
                    }
                    inserted = true;
                    break;
                }
            }
            // if parent should be inserted at the end
            if (!inserted) {
                freeNodesList.push_back(parent);
            }
        }
    }

    root = parent; // build Huffman tree
}

template <typename charType>
void HuffmanTree<charType>::fillCanonicalCodes(HuffmanNode* node, const uint32_t codeLength, const uint32_t currentCode, Array<CanonicalCode>& canonicalCodes)
{
    if (node->left == nullptr) { // also means that node.right == nullptr
        // write the code of current character
        canonicalCodes.push_back(CanonicalCode(node->chars[0], codeLength, currentCode));
        return;
    }
    fillCanonicalCodes(node->left, codeLength + 1, currentCode << 1, canonicalCodes);
    fillCanonicalCodes(node->right, codeLength + 1, (currentCode << 1) + 1, canonicalCodes);
}

template <typename charType>
Array<typename HuffmanTree<charType>::CanonicalCode> HuffmanTree<charType>::GetCanonicalCodes(HuffmanTree<charType>& tree, const size_t alphabetSize)
{
    Array<CanonicalCode> canonicalCodes(alphabetSize);
    
    if (alphabetSize == 1) {
        // special case
        canonicalCodes.push_back(CanonicalCode(tree.root->chars[0], 1, 0));
    } else {
        tree.fillCanonicalCodes(tree.root, 0, 0, canonicalCodes);
    }

    // sort tuple by length of huffman codes
    std::sort(canonicalCodes.begin(), canonicalCodes.end(), [](const auto& a, const auto& b) {
        return a.codeLength < b.codeLength;
    });

    std::set<std::pair<uint32_t, uint32_t>> usingCodes; // <code, length>
    std::pair<uint32_t, uint32_t> binRepresentation;
    std::pair<uint32_t, uint32_t> temp;
    bool isCodeUsed;
    for (size_t i = 0; i < canonicalCodes.size(); ++i) {
        uint32_t lengthOfCode = (canonicalCodes[i].codeLength);
        // find the code of length = lengthOfCode
        // which has the smallest number (considering the code as the binary representation)
        // which was not used before and which satisfy the huffman rule
        for (uint32_t j = 0; j < 1000000000; ++j) {
            // find the code of length = lengthOfCode
            // which was not used before
            binRepresentation = { j, lengthOfCode };
            if (usingCodes.find(binRepresentation) == usingCodes.end()) {
                // check if the code satisfies the huffman rule
                // (if there are no any codes from which the current code begins)
                isCodeUsed = false;
                temp = { j , lengthOfCode };
                for (uint32_t _ = 0; _ < lengthOfCode; ++_) {
                    temp.first >>= 1;
                    --temp.second;
                    if (usingCodes.find(temp) != usingCodes.end()) {
                        isCodeUsed = true;
                        break;
                    }
                }
                if (!isCodeUsed) {
                    canonicalCodes[i].code = j;
                    usingCodes.insert(binRepresentation);
                    break;
                }
            }
        }
    }

    return canonicalCodes;
}

template <typename charType>
void HuffmanTree<charType>::clearNode(HuffmanNode* node) {
    if (node->left != nullptr)
        clearNode(node->left);
    if (node->right != nullptr)
        clearNode(node->right);
    delete node;
}

// END