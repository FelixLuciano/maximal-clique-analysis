#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>

#include <omp.h>
#include <mpi.h>


std::vector<unsigned int> getNodeMaximalClique(std::vector<std::vector<unsigned char>> grafo, unsigned int nVertices, unsigned int node) {
    std::vector<std::tuple<unsigned int, unsigned int, std::vector<unsigned char>>> intersections;

    #pragma omp parallel
    {
        #pragma omp for
        for (unsigned int i = 0; i < nVertices; i++) {
            if (i == node || grafo[node][i] == 0)
                continue;

            unsigned int degree = 0;
            std::vector<unsigned char> intersection(nVertices, 0);

            for (unsigned int j = 0; j < nVertices; j++) {
                if (j == node || j == i || grafo[node][j] & grafo[i][j]) {
                    intersection[j] = 1;
                    degree++;
                }
            }

            #pragma omp critical
            {
                intersections.emplace_back(i, degree, intersection);
            }
        }
    }

    std::sort(intersections.begin(), intersections.end(), [](const std::tuple<unsigned int, unsigned int, std::vector<unsigned char>> &a, const std::tuple<unsigned int, unsigned int, std::vector<unsigned char>> &b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    std::vector<unsigned char> clique(nVertices, 1);

    for (auto row : intersections) {
        if (clique[std::get<0>(row)] == 0)
            continue;

        for (unsigned int i = 0; i < nVertices; i++)
            clique[i] = clique[i] & std::get<2>(row)[i];
    }

    std::vector<unsigned int> ids;

    for (unsigned int i = 0; i < clique.size(); i++) {
        if (clique[i] != 0) {
            ids.push_back(i);
        }
    }

    return ids;
}

std::vector<unsigned int> getMaximalClique(std::vector<std::vector<unsigned char>> graph, unsigned int nVertices) {
    int rank, nProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    std::vector<unsigned int> clique;

    for (unsigned int i = rank; i < nVertices; i += nProc) {
        auto nodeClique = getNodeMaximalClique(graph, nVertices, i);

        if (nodeClique.size() > clique.size())
            clique = nodeClique;
    }

    return clique;
}
