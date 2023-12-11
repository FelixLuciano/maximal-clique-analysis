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

    bool isClique = true;
    unsigned int nVertices;
    vector<vector<bool>> graph = readGraph(filename, nVertices);

    for (unsigned int i : indexes) {
        for (unsigned int j : indexes) {
            if (i != j && graph[i][j] == 0)
                isClique = false;
        }
    }

    cout << "Filename: " << filename << endl;
    cout << "Size:     " << indexes.size() << ":" << indexes.size() << " (of " << nVertices << ":" << nVertices << ")" << endl;

    if (isClique == true)
        cout << "Status:   PASS";
    else
        cout << "Status:   FAIL";

    return 0;
}
