#include <iostream>
#include <iomanip>
#include <vector>

#include <mpi.h>

#include "./read-graph.cpp"
extern std::vector<unsigned int> getMaximalClique(std::vector<std::vector<unsigned char>> graph, unsigned int nVertices);

using namespace std;


int main(int argc, char *argv[]) {
    MPI_Init(&argc, &argv);

    int rank, nProc;
    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &nProc);

    unsigned int nVertices;
    std::vector<std::vector<unsigned char>> graph = readGraph("graph.txt", nVertices);

    double duration = MPI_Wtime();

    std::vector<unsigned int> maximalClique = getMaximalClique(graph, nVertices);
    unsigned int cliqueSize = maximalClique.size();

    if (rank != 0) {
        MPI_Request request;
        MPI_Send(&cliqueSize, 1, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD);
        MPI_Isend(maximalClique.data(), cliqueSize, MPI_UNSIGNED_LONG, 0, 0, MPI_COMM_WORLD, &request);
        
        duration = MPI_Wtime() - duration;
    }
    else {
        for (int i = 1; i < nProc; i++) {
            MPI_Recv(&cliqueSize, 1, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

            if (cliqueSize > maximalClique.size()) {
                maximalClique.resize(cliqueSize, 0);
                MPI_Recv(maximalClique.data(), cliqueSize, MPI_UNSIGNED_LONG, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
            }
            else {
                cliqueSize = maximalClique.size();
            }
        }

        duration = MPI_Wtime() - duration;
    }

    double total_duration;
    MPI_Reduce(&duration, &total_duration, 1, MPI_DOUBLE, MPI_SUM, 0, MPI_COMM_WORLD);

    if (rank == 0) {
        std::cout << fixed << setprecision(2) << "Finished in  " << duration << " seconds. Total of  " << total_duration << " seconds." << std::endl;

        for (unsigned int i : maximalClique) {
            std::cout << i+1;

            if (i != maximalClique.back()) {
                std::cout << ", ";
            }
        }
        std::cout << std::endl;
    }

    MPI_Finalize();
    return 0;
}
