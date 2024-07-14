#include <iostream>
#include <vector>
#include <chrono>

#include "include\BinarySearchTree.h"
#include "include\AVLTree.h"
#include "include\RedBlackTree.h"

// const std::string BST_PATH = "trees_print\\BST_print.txt";
// const std::string AVL_PATH = "trees_print\\AVL_print.txt";
// const std::string RBT_PATH = "trees_print\\RBT_print.txt";

int main()
{
    const int COUNT_OF_TESTS = 10;
    srand(time(0)); // Initialize random seed for rand()

    std::vector<int> A {5, 3, 9, 4, 1, 6, 7, 8, 2, 10};

    // TEST RBT TREE FOR VALUES CONSTANTLY INCREASING
    RedBlackTree<int> myBST;
    for(int key : A)
    {
        myBST.insert(key);
    }
    std::cout << "Current tree:" << std::endl;
    myBST.print();

    std::cout << "pre order traversal: ";
    myBST.print_pre_order();
    std::cout << std::endl;
    std::cout << "post order traversal: ";
    myBST.print_post_order();
    std::cout << std::endl;
    std::cout << "in order traversal: ";
    myBST.print_in_order();
    std::cout << std::endl;




// START TEST BST TREE
    /* 
        // TRAVERSAL
        std::vector<int> A {500, 250, 750, 120, 370, 650, 850, 200, 500, 600, 750};
        BinarySearchTree<int> myBST(A);
        std::cout << "Start tree: (height = " << myBST.height() << ")" << std::endl;
        myBST.print();

        std::cout << "In order traversal: ";
        myBST.print_in_order();
        std::cout << std::endl;
        std::cout << "Pre order traversal: ";
        myBST.print_pre_order();
        std::cout << std::endl;
        std::cout << "Post order traversal: ";
        myBST.print_post_order();
        std::cout << std::endl;
    */

  /*    // INSERT AND REMOVE
        int current_key;
        std::vector<int> InsertedKeys(COUNT_OF_TESTS);
        for (int i = 0; i < COUNT_OF_TESTS; i++) {
            current_key = rand() % 100;
            InsertedKeys[i] = current_key;
            myBST.insert(current_key);
            std::cout << "After insert " << current_key << ": (height = " << myBST.height() << ")" << std::endl;
            myBST.print();
        }

        for (int i = 0; i < COUNT_OF_TESTS; i++) {
            current_key = InsertedKeys[i];
            myBST.remove(current_key);
            std::cout << "After remove " << current_key << ": (height = " << myBST.height() << ")" << std::endl;
            myBST.print();
        }
    */
// END TEST BST TREE

// START TEST AVL TREE

    /*  // TRAVERSAL
        std::vector<int> A {50, 25, 75, 12, 37, 65, 85, 20, 5, 60, 75};
        BinarySearchTree<int> myAVL(A);
        std::cout << "Start tree: (height = " << myAVL.height() << ")" << std::endl;
        myAVL.print();

        std::cout << "In order traversal: ";
        myAVL.print_in_order();
        std::cout << std::endl;
        std::cout << "Pre order traversal: ";
        myAVL.print_pre_order();
        std::cout << std::endl;
        std::cout << "Post order traversal: ";
        myAVL.print_post_order();
        std::cout << std::endl;
    */

    /*  // INSERT AND REMOVE
        AVLTree<int> myAVL(A);

        std::cout << "Start tree:" << std::endl;
        myAVL.print();

        myAVL.insert(6);
        std::cout << "After insert 6:" << std::endl;
        myAVL.print();

        myAVL.remove(5);
        std::cout << "After remove 5:" << std::endl;
        myAVL.print();

        myAVL.remove(6);
        std::cout << "After remove 6:" << std::endl;
        myAVL.print();

        myAVL.remove(1);
        std::cout << "After remove 1:" << std::endl;
        myAVL.print();

        myAVL.insert(7);
        std::cout << "After insert 7:" << std::endl;
        myAVL.print();
    */
// END TEST AVL TREE

// START TEST RED-BALCK TREE 
    /* 
        // TRAVERSAL
        std::vector<int> A {50, 25, 75, 12, 37, 65, 85, 20, 5, 60, 75};
        BinarySearchTree<int> myRBT(A);
        std::cout << "Start tree: (height = " << myRBT.height() << ")" << std::endl;
        myRBT.print();

        std::cout << "In order traversal: ";
        myRBT.print_in_order();
        std::cout << std::endl;
        std::cout << "Pre order traversal: ";
        myRBT.print_pre_order();
        std::cout << std::endl;
        std::cout << "Post order traversal: ";
        myRBT.print_post_order();
        std::cout << std::endl;
    */

    /*  // INSERT AND REMOVE
        RedBlackTree<int> myRBT(A);

        std::cout << "Start tree:" << std::endl;
        myRBT.print();
        
        myRBT.insert(6);
        std::cout << "After insert 6:" << std::endl;
        myRBT.print();

        myRBT.remove(5);
        std::cout << "After remove 5:" << std::endl;
        myRBT.print();

        myRBT.remove(6);
        std::cout << "After remove 6:" << std::endl;
        myRBT.print();

        myRBT.remove(1);
        std::cout << "After remove 1:" << std::endl;
        myRBT.print();

        myRBT.insert(7);
        std::cout << "After insert 7:" << std::endl;
        myRBT.print();
    */
// END TEST RED-BALCK TREE

    return 0;
}