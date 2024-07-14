#include <chrono>

template <class Tree, typename keyType>
double getInsertionTime(Tree& tree, const keyType& key)
{
    double time = 0;
    const int COUNT_OF_ITERATIONS = 100000;
    for (int i = 0; i < COUNT_OF_ITERATIONS; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        tree.insert(key);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        time += duration.count();
        tree.remove(key);
    }

    return time / COUNT_OF_ITERATIONS;
}

template <class Tree, typename keyType>
double getRemovalTime(Tree& tree, const keyType& key)
{
    double time = 0;
    const int COUNT_OF_ITERATIONS = 100000;
    for (int i = 0; i < COUNT_OF_ITERATIONS; i++)
    {
        auto start = std::chrono::high_resolution_clock::now();
        tree.remove(key);
        auto end = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> duration = end - start;

        time += duration.count();
        tree.insert(key);
    }

    return time / COUNT_OF_ITERATIONS;
}