#pragma once

#include <string>
#include <fstream>
#include <queue>

template <typename keyType>
class BinarySearchTree {
private:
    struct Node
    {
        keyType key;
        Node* left;
        Node* right;
        int height; // to avoid recursion in calculating tree height measuring

        Node(): left(nullptr), right(nullptr), height(1) {};
        Node(keyType key) : Node()
            { this->key = key; }
    };
    Node* root;

    Node* predecessor(Node* node, Node*& parent = nullptr) const;

    int getHeight(Node* node) const { return node == nullptr ? 0 : node->height; };

    void printInOrder(Node* node) const;
    void printPreOrder(Node* node) const;
    void printPostOrder(Node* node) const;
    void printWidthTraversal(Node* node) const;
    
    void printHelper(Node* node, int current_height, const int& tree_height,
        const int& number_length, std::fstream* file = nullptr) const;
    
public:
    BinarySearchTree() : root(nullptr) {};
    BinarySearchTree(const keyType*& keys, int size);
    BinarySearchTree(const std::vector<keyType>& keys);
    
    void insert(const keyType& key);
    void remove(const keyType& key);

    keyType get_max_key() const;
    keyType get_min_key() const;

    int height() const { return getHeight(root); };

    void clear();

    void print_in_order() { printInOrder(root); }
    void print_pre_order() { printPreOrder(root); }
    void print_post_order() { printPostOrder(root); }
    void print_width_traversal() { printWidthTraversal(root); }
    void print(std::fstream* file = nullptr) const;

    ~BinarySearchTree() { clear(); };
};

// START OF IMPLEMENTATION

template <typename keyType>
BinarySearchTree<keyType>::BinarySearchTree(const keyType*& keys, int size)
    : root(nullptr)
{
    for (int i = 0; i < size; i++) {
        insert(keys[i]);
    }
}

template <typename keyType>
BinarySearchTree<keyType>::BinarySearchTree(const std::vector<keyType>& keys)
    : root(nullptr)
{
    for (int i = 0; i < keys.size(); i++) {
        insert(keys[i]);
    }
}

template <typename keyType>
void BinarySearchTree<keyType>::clear()
{
    while (root != nullptr)
        remove(root->key);
}

template <typename keyType>
keyType BinarySearchTree<keyType>::
get_max_key() const {
    if (root == nullptr)
        return 0;
    
    Node* current = root;
    while (current->right != nullptr)
        current = current->right;
    
    return current->key;
}

template <typename keyType>
keyType BinarySearchTree<keyType>::
get_min_key() const {
    if (root == nullptr)
        return 0;
    
    Node* current = root;
    while (current->left != nullptr)
        current = current->left;
    
    return current->key;
}

// Returns first node less than the given node.
template <typename keyType>
typename BinarySearchTree<keyType>::Node* BinarySearchTree<keyType>::
predecessor(Node* node, Node*& parent) const
{
    if (node == nullptr || node->left == nullptr)
        return nullptr;
    
    parent = node;
    Node* pred = node->left;
    while (pred->right != nullptr) {
        parent = pred;
        pred = pred->right;
    }

    return pred;
}

// Inserts a new node with the specified key
template <typename keyType>
void BinarySearchTree<keyType>::insert(const keyType& key)
{
    Node* newNode = new Node(key);

    if (root == nullptr) {
        root = newNode; // height will be 1 automatically
        return;
    }

    Node* current = root;

    // to update heights
    Node** nodesOnTheWay = new Node*[root->height];
    int countOfNodesOnTheWay = 0;
    

    // Find the appropriate position to insert the new node
    while (true)
    {
        if (key <= current->key && current->left != nullptr){
            // Update nodes on the way
            nodesOnTheWay[countOfNodesOnTheWay] = current;
            countOfNodesOnTheWay++;

            current = current->left;
        }
        else if (key > current->key && current->right != nullptr){
            // Update nodes on the way
            nodesOnTheWay[countOfNodesOnTheWay] = current;
            countOfNodesOnTheWay++;
            
            current = current->right;
        }
        else break;
    }

    // Inserting the new node
    if (key <= current->key) {
        current->left = newNode;
    } else {
        current->right = newNode;
    }

    // Update height for current
    current->height = std::max(getHeight(current->left), getHeight(current->right)) + 1;

    // Calculating heights for other nodes on the way to current
    for (int i = countOfNodesOnTheWay - 1; i >= 0; i--) {
        current = nodesOnTheWay[i];
        current->height = std::max(getHeight(current->left), getHeight(current->right)) + 1;
    }
    
    delete[] nodesOnTheWay;
}

// Removes node with the specified key from subtree rooted at the given node.
// Returns the new root of the subtree based on the removed node.
template <typename keyType>
void BinarySearchTree<keyType>::
remove(const keyType& key)
{
    if (root == nullptr)
        return;

    Node* nodeToRemove = root;
    Node* parent = nullptr;

    // to update heights
    Node** nodesOnTheWay = new Node*[root->height];
    int countOfNodesOnTheWay = 0;


    // Find node to remove
    while (nodeToRemove != nullptr)
    {
        // searching for node to remove
        if (key < nodeToRemove->key) {
            parent = nodeToRemove;
            nodeToRemove = nodeToRemove->left;

            // Update nodes on the way
            nodesOnTheWay[countOfNodesOnTheWay] = parent;
            countOfNodesOnTheWay++;
        } else if (key > nodeToRemove->key) {
            parent = nodeToRemove;
            nodeToRemove = nodeToRemove->right;

            // Update nodes on the way
            nodesOnTheWay[countOfNodesOnTheWay] = parent;
            countOfNodesOnTheWay++;
        }
        else break;
    }

    if (nodeToRemove == nullptr)
        return;
    
    if (nodeToRemove->left == nullptr || nodeToRemove->right == nullptr)
    {
        // CASE 1: if nodeToRemove has less than two children

        // set to the "tmp" not nullptr node
        Node* tmp = nodeToRemove->left ? nodeToRemove->left : nodeToRemove->right;

        if (tmp == nullptr) {
            // CASE 1.1: if both children are nullptr
            if (nodeToRemove == root) {
                root = nullptr;
            } else if (nodeToRemove == parent->left) {
                parent->left = nullptr;
            } else {
                parent->right = nullptr;
            }
            
        } else {
            // CASE 1.2: if only one child is not nullptr
            if (nodeToRemove == root) {
                root = tmp;
            } else if (nodeToRemove == parent->left) {
                parent->left = tmp;
            } else {
                parent->right = tmp;
            }
        }

        if (parent != nullptr) {
            // Update height of parent
            parent->height = std::max(getHeight(parent->left), getHeight(parent->right)) + 1;
        }

        // delete node
        delete nodeToRemove;
    } else {
        // CASE 2: if both children exist

        // use predecessor instead of successor
        // because otherwise tree structure can be changed in some cases
        // by tree structure I mean this: left <= key < right 
        Node* pred_parent;
        Node* pred = predecessor(nodeToRemove, pred_parent);
        nodeToRemove->key = pred->key;
        
        if (pred != nodeToRemove->left) {
            // typical case
            pred_parent->right = pred->left;
        } else {
            // if pred going right next to the nodeToRemove
            pred_parent->left = pred->left;
        }

        // Update height of pred_parent
        pred_parent->height = std::max(getHeight(pred_parent->left), getHeight(pred_parent->right)) + 1;

        delete pred;
    }

    // Update heights for other nodes on the way to parent
    Node* current;
    for (int i = countOfNodesOnTheWay - 1; i >= 0; i--) {
        current = nodesOnTheWay[i];
        current->height = std::max(getHeight(current->left), getHeight(current->right)) + 1;
    }

    delete[] nodesOnTheWay;
}

template <typename keyType>
void BinarySearchTree<keyType>::printInOrder(Node* node) const
{
    if (node == nullptr) {
        return;
    }

    printInOrder(node->left);
    std::cout << node->key << " ";
    printInOrder(node->right);
}

template <typename keyType>
void BinarySearchTree<keyType>::printPreOrder(Node* node) const
{
    if (node == nullptr) {
        return;
    }

    std::cout << node->key << " ";
    printPreOrder(node->left);
    printPreOrder(node->right);
}

template <typename keyType>
void BinarySearchTree<keyType>::printPostOrder(Node* node) const
{
    if (node == nullptr) {
        return;
    }

    printPostOrder(node->left);
    printPostOrder(node->right);
    std::cout << node->key << " ";
}

template <typename keyType>
void BinarySearchTree<keyType>::printWidthTraversal(Node* node) const
{
    if (root == nullptr) {
        return;
    }

    std::queue<Node*> queue;
    queue.push(root);

    while (!queue.empty()) {
        Node* node = queue.front();
        queue.pop();
        std::cout << node->key << " ";

        if (node->left != nullptr) {
            queue.push(node->left);
        }
        if (node->right != nullptr) {
            queue.push(node->right);
        }
    }

    std::cout << std::endl;
}

template <typename keyType>
void BinarySearchTree<keyType>::
printHelper(Node* node, int current_height, const int& tree_height, const int& number_length, std::fstream* file) const
{
    if (node == nullptr) {
        return;
    }
    
    printHelper(node->left, current_height + 1, tree_height, number_length, file);

    // ========== START PRINT CURRENT NODE ========== //

    // use static to save memory
    // it will work because variable can be initialized once.
    static std::string space;
    static std::string str_normalized_key;
    int diff = tree_height - current_height;
    int current_length;

    // Caclulating spaces for current value (key)
    for (int j = 0; j < diff; j++) {
        for (int k = 0; k < number_length; k++) {
            space += ' ';   
        }
    }

    // Setting insignificant zeros to current value (key)
    current_length = std::to_string(node->key).size();
    if (current_length < number_length) {
        for (int i = 0; i < (number_length - current_length); i++) {
            str_normalized_key += '0';
        }
    }
    str_normalized_key += std::to_string(node->key);

    // print space and then the current value (key)
    std::cout << space << str_normalized_key << "-|" << std::endl;
    if (file != nullptr) {
        *file << space << str_normalized_key << std::endl;
    }

    // Clearing variables
    space.clear();
    str_normalized_key.clear();

    // ========== END PRINT CURRENT NODE ========== //

    printHelper(node->right, current_height + 1, tree_height, number_length, file);
}

template <typename keyType>
void BinarySearchTree<keyType>::print(std::fstream* file) const
{
    if (root == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        if (file != nullptr) {
            *file << "Tree is empty." << std::endl;
        }
        return;
    }

    // Calculating maximum length of all keys
    int number_length;
    keyType min_key = get_min_key();
    keyType max_key = get_max_key();

    if (min_key < 0) {
        // if minKey is negative so it contains '-' symbol 
        number_length = std::max(std::to_string(min_key).size(), std::to_string(max_key).size());
    } else {
        number_length = std::to_string(max_key).size();
    }

    // print Tree
    if (file != nullptr) {
        printHelper(root, 1, height(), number_length, file);
    } else {
        printHelper(root, 1, height(), number_length);
    }
}