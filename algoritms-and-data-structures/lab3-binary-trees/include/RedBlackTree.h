#include <iostream>
#include <queue>

template<typename keyType>
class RedBlackTree{
private:
    enum Color { RED, BLACK };

    struct Node
    {
        keyType key;
        Color color;
        Node* left;
        Node* right;
        Node* parent;

        Node(): left(nullptr), right(nullptr), parent(nullptr) {}
        Node(Node* nil, const int& key): left(nil), right(nil), parent(nil)
        {
            this->key = key;
            color = RED;
        }
    };
    Node* root;
    Node* nil;

    void rotateLeft(Node* x);
    void rotateRight(Node* y);

    Node* predecessor(Node* node) const;

    void fixInsert(Node* node);
    void fixRemove(Node* node);

    int getHeight(Node* node) const;

    void printInOrder(Node* node) const;
    void printPreOrder(Node* node) const;
    void printPostOrder(Node* node) const;
    void printWidthTraversal(Node* node) const;

    void printHelper(Node* node, int current_height, const int& tree_height,
        const int& number_length, std::fstream* file = nullptr) const;

public:
    RedBlackTree() {
        nil = new Node();
        nil->color = BLACK;
        root = nil;
    };
    RedBlackTree(const keyType*& keys, int size);
    RedBlackTree(const std::vector<keyType>& keys);

    void insert(const keyType& key);
    void remove(const keyType& key);

    keyType get_max_key() const;
    keyType get_min_key() const;

    int height() const { return getHeight(root); };

    void print_in_order() const { printInOrder(root); };
    void print_pre_order() const { printPreOrder(root); };
    void print_post_order() const { printPostOrder(root); };
    void print_width_traversal() const { printWidthTraversal(root); };

    void clear();

    void print(std::fstream* file = nullptr) const;
    
    ~RedBlackTree() { clear(); };
};

template <typename keyType>
RedBlackTree<keyType>::RedBlackTree(const keyType*& keys, int size)
    : RedBlackTree()
{
    for (int i = 0; i < size; i++)
        insert(keys[i]);
}

template <typename keyType>
RedBlackTree<keyType>::RedBlackTree(const std::vector<keyType>& keys)
    : RedBlackTree()
{
    for (int i = 0; i < keys.size(); i++)
        insert(keys[i]);
}

template <typename keyType>
void RedBlackTree<keyType>::clear()
{
    while (root != nil){
        remove(root->key);
    }
}

template <typename keyType>
keyType RedBlackTree<keyType>::get_max_key() const {
    if (root == nil)
        return 0;
    
    Node* current = root;
    while (current->right != nil)
        current = current->right;
    
    return current->key;
}

template <typename keyType>
keyType RedBlackTree<keyType>::get_min_key() const {
    if (root == nullptr)
        return 0;
    
    Node* current = root;
    while (current->left != nil)
        current = current->left;
    
    return current->key;
}

template <typename keyType>
void RedBlackTree<keyType>::rotateLeft(Node* x)
{
    //     x              y
    //    / \            / \
    //   a   y    =>    x   c
    //      / \        / \
    //     b   c      a   b

    Node* y = x->right;
    x->right = y->left;

    if (y->left != nil)
        y->left->parent = x;
    
    y->parent = x->parent;

    if (x->parent == nil) {
        root = y;
    } else if (x == x->parent->left) {
        x->parent->left = y;
    } else {
        x->parent->right = y;
    }

    y->left = x;
    x->parent = y;
}

template <typename keyType>
void RedBlackTree<keyType>::rotateRight(Node* y)
{
    //     y              x
    //    / \            / \
    //   x   c    =>    a   y
    //  / \                / \
    // a   b              b   c

    Node* x = y->left;
    y->left = x->right;

    if (x->right != nil)
        x->right->parent = y;
    
    x->parent = y->parent;

    if (y->parent == nil) {
        root = x;
    } else if (y == y->parent->right) {
        y->parent->right = x;
    } else {
        y->parent->left = x;
    }

    x->right = y;
    y->parent = x;
}

// Returns first node less than the given node.
template <typename keyType>
typename RedBlackTree<keyType>::Node* RedBlackTree<keyType>::
predecessor(Node* node) const
{
    if (node == nil || node->left == nil) {
        return nil;
    }
    
    Node* pred = node->left;
    while (pred->right != nil) {
        pred = pred->right;
    }

    return pred;
}

template <typename keyType>
int RedBlackTree<keyType>::getHeight(Node* node) const
{
    if (node == nil) {
        return 0;
    } else {
        return std::max(getHeight(node->left), getHeight(node->right)) + 1;
    }
}

template <typename keyType>
void RedBlackTree<keyType>::insert(const keyType& key)
{
    Node* node = new Node(nil, key);
    
    // Inserting the node into the correct position
    if (root == nil) {
        root = node;
        root->color = BLACK;
    } else {
        Node* current = root;
        Node* parent = nil;

        while (current != nil) {
            parent = current;
            if (node->key < current->key) {
                current = current->left;
            } else {
                current = current->right;
            }
        }

        node->parent = parent;
        if (node->key < parent->key) {
            parent->left = node;
        } else {
            parent->right = node;
        }

        // Fixing the tree
        fixInsert(node);
    }
}

template <typename keyType>
void RedBlackTree<keyType>::fixInsert(Node* node)
{
    Node* parent = nil;
    Node* grandParent = nil;

    while (node != root && node->color != BLACK && node->parent->color == RED) {
        parent = node->parent;
        grandParent = node->parent->parent;

        // Case 1: Parent is the left child of grandparent
        if (parent == grandParent->left) {
            Node* uncle = grandParent->right;

            // Case 1.1: Uncle is red, recoloring needed
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandParent->color = RED;
                node = grandParent;
            }
            // Case 1.2: Uncle is black, rotation needed
            else {
                if (node == parent->right) {
                    rotateLeft(parent);
                    node = parent;
                    parent = node->parent;
                }

                parent->color = BLACK;
                grandParent->color = RED;
                rotateRight(grandParent);
            }
        }
        // Case 2: Parent is the right child of grandparent
        else {
            Node* uncle = grandParent->left;

            // Case 2.1: Uncle is red, recoloring needed
            if (uncle != nullptr && uncle->color == RED) {
                parent->color = BLACK;
                uncle->color = BLACK;
                grandParent->color = RED;
                node = grandParent;
            }
            // Case 2.2: Uncle is black, rotation needed
            else {
                if (node == parent->left) {
                    rotateRight(parent);
                    node = parent;
                    parent = node->parent;
                }

                parent->color = BLACK;
                grandParent->color = RED;
                rotateLeft(grandParent);
            }
        }
    }

    root->color = BLACK;
}

template <typename keyType>
void RedBlackTree<keyType>::remove(const keyType& key)
{
    if (root == nil)
        return;

    Node* nodeToRemove = root;

    while (nodeToRemove != nil)
    {
        // searching for node to remove
        if (key < nodeToRemove->key) {
            nodeToRemove = nodeToRemove->left;
        } else if (key > nodeToRemove->key) {
            nodeToRemove = nodeToRemove->right;
        }
        else break;
    }

    if (nodeToRemove == nil)
        return;
    
    // In the code below we just delete the node 
    // like it was done in the BST
    // But before deleting we need to fix the tree
    // If the node to remove is BLACK

    Node* y; // Node to remove
    Node* x; // Node to replace y

    // Setting y
    if (nodeToRemove->left == nil || nodeToRemove->right == nil) {
        y = nodeToRemove;
    } else {
        y = predecessor(nodeToRemove);
    }

    // Setting x=
    if (y->left != nil) {
        x = y->left;
    } else {
        // if y = nodeToRemove and it has no left child
        x = y->right;
    }


    // Setting x's parent
    x->parent = y->parent;

    // Setting y's parent
    if (y->parent == nil) {
        root = x;
    } else if (y == y->parent->left) {
        y->parent->left = x;
    } else {
        y->parent->right = x;
    }

    nodeToRemove->key = y->key;

    if (y->color == BLACK) {
        fixRemove(x);
    }

    delete y;
}

template <typename keyType>
void RedBlackTree<keyType>::fixRemove(Node* node)
{
    while (node != root && node->color == BLACK) {
        if (node == node->parent->left) {
            // sibling can't be null (due to properties of RBT)
            Node* sibling = node->parent->right;

            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateLeft(node->parent);
                sibling = node->parent->right;
            }
            
            if (sibling->left->color == BLACK && sibling->right->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->right->color == BLACK) {
                    sibling->left->color = BLACK;
                    sibling->color = RED;
                    rotateRight(sibling);
                    sibling = node->parent->right;
                }
                
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->right->color = BLACK;
                rotateLeft(node->parent);
                node = root;
            }
        } else {
            Node* sibling = node->parent->left;
            
            if (sibling->color == RED) {
                sibling->color = BLACK;
                node->parent->color = RED;
                rotateRight(node->parent);
                sibling = node->parent->left;
            }
            
            if (sibling->right->color == BLACK && sibling->left->color == BLACK) {
                sibling->color = RED;
                node = node->parent;
            } else {
                if (sibling->left->color == BLACK) {
                    sibling->right->color = BLACK;
                    sibling->color = RED;
                    rotateLeft(sibling);
                    sibling = node->parent->left;
                }
                
                sibling->color = node->parent->color;
                node->parent->color = BLACK;
                sibling->left->color = BLACK;
                rotateRight(node->parent);
                node = root;
            }
        }
    }
    
    node->color = BLACK;
}

template <typename keyType>
void RedBlackTree<keyType>::printInOrder(Node* node) const
{
    if (node == nil) {
        return;
    }

    printInOrder(node->left);
    std::cout << node->key << " ";
    printInOrder(node->right);
}

template <typename keyType>
void RedBlackTree<keyType>::printPreOrder(Node* node) const
{
    if (node == nil) {
        return;
    }

    std::cout << node->key << " ";
    printInOrder(node->left);
    printInOrder(node->right);
}

template <typename keyType>
void RedBlackTree<keyType>::printPostOrder(Node* node) const
{
    if (node == nil) {
        return;
    }

    printPostOrder(node->left);
    printPostOrder(node->right);
    std::cout << node->key << " ";
}

template <typename keyType>
void RedBlackTree<keyType>::printWidthTraversal(Node* node) const
{
    if (root == nil) {
        return;
    }

    std::queue<Node*> queue;
    queue.push(root);

    while (!queue.empty()) {
        Node* node = queue.front();
        queue.pop();
        std::cout << node->key << " ";

        if (node->left != nil) {
            queue.push(node->left);
        }
        if (node->right != nil) {
            queue.push(node->right);
        }
    }

    std::cout << std::endl;
}

template <typename keyType>
void RedBlackTree<keyType>::
printHelper(Node* node, int current_height, const int& tree_height, const int& number_length, std::fstream* file) const
{
    if (node == nil) {
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

    // Setting color
    if (node->color == RED) {
        str_normalized_key += 'r';
    } else {
        str_normalized_key += 'b';
    }

    // Caclulating spaces for current value (key)
    for (int j = 0; j < diff; j++) {
        for (int k = 0; k < number_length + 1; k++) {
            // + 1 because of 'r' or 'b' at the beginning
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
void RedBlackTree<keyType>::print(std::fstream* file) const
{
    if (root == nil) {
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