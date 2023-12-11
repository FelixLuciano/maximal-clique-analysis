#include <iostream>
#include <sstream>
#include <cmath>
#include <algorithm>
#include <vector>
#include <tuple>

#include "../src/read-graph.cpp"

using namespace std;


int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Usage: " << argv[0] << " <input_file> [indexes...]" << endl;
        return 1;
    }
    string filename = argv[1];

    vector<unsigned int> indexes;

    for (int i = 2; i < argc; ++i) {
        istringstream iss(argv[i]);
        unsigned int index;

        if (iss >> index) {
            indexes.push_back(index-1);
        } else {
            cerr << "Invalid argument: " << argv[i] << ". Skipping..." << endl;
        }
    }

    unsigned int nVertices;
    vector<vector<bool>> graph = readGraph(filename, nVertices);

    unsigned long padSize = pow(10, (int)log10(indexes.back()+1));

    for (unsigned int i : indexes) {
        for (unsigned long j = i+1; j < padSize; j *= 10) {
            cout << ' ';
        }
        cout << i+1 << "  -  ";

        for (unsigned int j : indexes) {
            cout << graph[i][j] << ' ';
        }
        cout << endl;
    }

    return 0;
}
