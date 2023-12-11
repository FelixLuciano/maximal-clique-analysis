#include <iostream>
#include <fstream>
#include <vector>


std::vector<std::vector<bool>> readGraph(const std::string& filename, unsigned int& nVertices) {
    std::ifstream file(filename);
    unsigned int nEdges;
    file >> nVertices >> nEdges;

    std::vector<std::vector<bool>> graph(nVertices, std::vector<bool>(nVertices, 0));

    for (unsigned int i = 0; i < nEdges; ++i) {
        unsigned int u, v;
        file >> u >> v;

        graph[u - 1][v - 1] = 1;
        graph[v - 1][u - 1] = 1;  // Non-directional graph
    }

    file.close();

    return graph;
}
