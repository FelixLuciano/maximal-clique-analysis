#include <iostream>
#include <vector>
#include <chrono>

#include "./read-graph.cpp"
extern std::vector<unsigned int> getMaximalClique(std::vector<std::vector<bool>>& graph, unsigned int nVertices);

using namespace std;


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Usage: " << argv[0] << " <input_file>" << std::endl;
        return 1;
    }

    std::string filename = argv[1];

    unsigned int nVertices;
    std::vector<std::vector<bool>> graph = readGraph(filename, nVertices);

    auto start = std::chrono::high_resolution_clock::now();

    std::vector<unsigned int> maximalClique = getMaximalClique(graph, nVertices);

    auto end = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double> duration = end - start;

    for (unsigned int i : maximalClique) {
        std::cout << i+1;

        if (i != maximalClique.back()) {
            std::cout << ", ";
        }
    }
    std::cout << std::endl;

    std::cout << "Filename: " << filename << std::endl;
    std::cout << "Execution time: " << duration.count() << " seconds" << std::endl;

    return 0;
}
