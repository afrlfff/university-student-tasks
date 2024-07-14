#include <iostream>
#include <fstream>
#include <queue>

template<typename keyType>
class AVLTree{
private:
    struct Node
    {
        int height;
        keyType key;
        Node* left;
        Node* right;

        Node(): left(nullptr), right(nullptr), height(1) {}
        Node(int key) : Node()
        { this->key = key; }
    };
    Node* root;

    int getHeight(Node* node) const;
    int getBalance(Node* node) const;

    Node* rotateLeft(Node* x);
    Node* rotateRight(Node* y);

    void updateHeight(Node* node);
    Node* updateBalance(Node* node);

    Node* predecessor(Node* node) const;
     
    Node* insertNode(Node* node, const keyType& key);
    Node* removeNode(Node* node, const keyType& key);

    void printInOrder(Node* node) const;
    void printPreOrder(Node* node) const;
    void printPostOrder(Node* node) const;
    void printWidthTraversal(Node* node) const;

    void printHelper(Node* node, int current_height, const int& tree_height,
        const int& number_length, std::fstream* file = nullptr) const;

public:
    AVLTree() : root(nullptr) {};
    AVLTree(const keyType*& keys, int size);
    AVLTree(const std::vector<keyType>& keys);

    void insert(const keyType& key) { insertNode(root, key); }
    void remove(const keyType& key) { removeNode(root, key); }

    keyType get_max_key() const;
    keyType get_min_key() const;

    void print_in_order() const { printInOrder(root); }
    void print_pre_order() const { printPreOrder(root); }
    void print_post_order() const { printPostOrder(root); }
    void print_width_traversal() const { printWidthTraversal(root); }

    int height() const { return getHeight(root); }

    void clear();
    
    void print(std::fstream* file = nullptr) const;

    ~AVLTree() { clear(); };
};

template <typename keyType>
AVLTree<keyType>::AVLTree(const keyType*& keys, int size): root(nullptr)
{
    for (int i = 0; i < size; i++){
        insert(keys[i]);
    }
}

template <typename keyType>
AVLTree<keyType>::AVLTree(const std::vector<keyType>& keys): root(nullptr)
{
    for (int i = 0; i < keys.size(); i++){
        insert(keys[i]);
    }
}

template <typename keyType>
void AVLTree<keyType>::clear()
{
    while (root != nullptr) {
        removeNode(root, root->key);
    }
}

template <typename keyType>
keyType AVLTree<keyType>::get_max_key() const {
    if (root == nullptr)
        return 0;
    
    Node* current = root;
    while (current->right != nullptr)
        current = current->right;
    
    return current->key;
}

template <typename keyType>
keyType AVLTree<keyType>::get_min_key() const {
    if (root == nullptr)
        return 0;
    
    Node* current = root;
    while (current->left != nullptr)
        current = current->left;
    
    return current->key;
}

template <typename keyType>
int AVLTree<keyType>::getBalance(Node* node) const
{
    if (node == nullptr)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

template <typename keyType>
int AVLTree<keyType>::getHeight(Node* node) const
{ if (node == nullptr)
        return 0;
    return node->height;
}

template <typename keyType>
void AVLTree<keyType>::
updateHeight(Node* node)
{ node->height = 1 + std::max(getHeight(node->left), getHeight(node->right)); }

// Updates balance of the given node if it needed
// Returns the new root of the subtree rooted on the given node
template <typename keyType>
typename AVLTree<keyType>::Node* AVLTree<keyType>::
updateBalance(Node* node)
{
    if (getBalance(node) < -1) {
        if (getBalance(node->right) > 0) {
            node->right = rotateRight(node->right);
        }
        return rotateLeft(node);
    }
    else if (getBalance(node) > 1) {
        if (getBalance(node->left) < 0) {
            node->left = rotateLeft(node->left);
        }
        return rotateRight(node);
    }

    return node;
}

// Returns first node less than the given node.
template <typename keyType>
typename AVLTree<keyType>::Node* AVLTree<keyType>::
predecessor(Node* node) const
{
    if (node == nullptr || node->left == nullptr) {
        return nullptr;
    }
    
    Node* pred = node->left;
    while (pred->right != nullptr) {
        pred = pred->right;
    }

    return pred;
}

template <typename keyType>
typename AVLTree<keyType>::Node* AVLTree<keyType>::
rotateLeft(Node* x)
{
    //     x              y
    //    / \            / \
    //   a   y    =>    x   c
    //      / \        / \
    //     b   c      a   b

    Node* y = x->right;
    x->right = y->left;
    y->left = x;
    if (x == root)
        root = y;

    updateHeight(x);
    updateHeight(y);

    return y;
}

template <typename keyType>
typename AVLTree<keyType>::Node* AVLTree<keyType>::
rotateRight(Node* y)
{
    //     y              x     
    //    / \            / \
    //   x   c    =>    a   y
    //  / \                / \
    // a   b              b   c

    Node* x = y->left;
    y->left = x->right;
    x->right = y;
    if (y == root)
        root = x;

    updateHeight(y);
    updateHeight(x);

    return x;
}

template <typename keyType>
typename AVLTree<keyType>::Node* AVLTree<keyType>::
insertNode(Node* node, const keyType& key)
{
    if (node == nullptr) {
        if (node == root) {
            root = new Node(key);
            return root;
        }
        return new Node(key);
    }

    if (key <= node->key) {
        node->left = insertNode(node->left, key);
    } else {
        node->right = insertNode(node->right, key);
    }

    // Update the height of the current node
    updateHeight(node);

    // Update the balance of the current node
    return updateBalance(node);
}

// Removes node with the specified key.
// Returns the new root of the subtree rooted on the removed node.
template <typename keyType>
typename AVLTree<keyType>::Node* AVLTree<keyType>::
removeNode(Node* node, const keyType& key)
{
    if (node == nullptr)
        return nullptr;
    
    if (key < node->key) {
        node->left = removeNode(node->left, key);
    } else if (key > node->key) {
        node->right = removeNode(node->right, key);
    } else {
        if (node->left == nullptr || node->right == nullptr)
        {
            // set to the "tmp" not nullptr node
            Node* tmp = node->left ? node->left : node->right;
            if (tmp == nullptr) {
                // if both children are nullptr
                if (node == root)
                    root = nullptr;
                delete node;
                return nullptr; // because parent of this element should be poined on nullptr
            }
            else {
                // if only one child is not nullptr
                if (node == root)
                    root = tmp;
                delete node;
                return tmp; // because parent of this element should be poined on tmp
            } 
        }
        else
        {
            // if both children are not nullptr
            Node* pred = predecessor(node);   
            node->key = pred->key;

            // predessor removing is called recursively to
            // ensure rebalancing for all elements above the predecessor
            node->left = removeNode(node->left, pred->key);
        }
    }

    if (node == nullptr) {
        // It is possible if recursion which is next to the current recursion
        // makes parent point to nullptr by "return nullptr"
        return nullptr;
    }

    updateHeight(node);
    return updateBalance(node);
}

// ======================== PRINT ======================== //

template <typename keyType>
void AVLTree<keyType>::printInOrder(Node* node) const
{
    if (node == nullptr) {
        return;
    }

    printInOrder(node->left);
    std::cout << node->key << " ";
    printInOrder(node->right);
}

template <typename keyType>
void AVLTree<keyType>::printPreOrder(Node* node) const
{
    if (node == nullptr) {
        return;
    }

    std::cout << node->key << " ";
    printInOrder(node->left);
    printInOrder(node->right);
}

template <typename keyType>
void AVLTree<keyType>::printPostOrder(Node* node) const
{
    if (node == nullptr) {
        return;
    }

    printPostOrder(node->left);
    printPostOrder(node->right);
    std::cout << node->key << " ";
}

template <typename keyType>
void AVLTree<keyType>::printWidthTraversal(Node* node) const
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
void AVLTree<keyType>::
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
    std::cout << space << str_normalized_key << std::endl;
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
void AVLTree<keyType>::print(std::fstream* file) const
{
    if (root == nullptr) {
        std::cout << "Tree is empty." << std::endl;
        if (file != nullptr) {
            *file << "Tree is empty." << std::endl;
        }
        return;
    }

    // Calculating count of simbols for longest number
    // to be able to print any number with the same length
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
        printHelper(root, 1, getHeight(root), number_length, file);
    } else {
        printHelper(root, 1, getHeight(root), number_length);
    }
}
