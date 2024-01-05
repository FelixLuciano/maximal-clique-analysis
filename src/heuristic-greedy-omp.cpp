#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
// #include <omp.h>

using namespace std;
typedef tuple<unsigned int, unsigned int, vector<bool>> intersection_t;


vector<intersection_t> getNodeNeighboorsIntersections(vector<vector<bool>> &graph, unsigned int nVertices, unsigned int node) {
    vector<intersection_t> intersections;

    for (unsigned int i = 0; i < nVertices; i++) {
        if (i == node || graph[node][i] == 0)
            continue;

        vector<bool> intersection(nVertices, 0);
        int degree = 0;

        for (unsigned int j = 0; j < nVertices; j++) {
            if (j == node || j == i || graph[node][j] & graph[i][j]) {
                intersection[j] = 1;
                ++degree;
            }
        }

        intersections.emplace_back(i, degree, intersection);
    }

    return intersections;
}

void updateNodeNeighboorsDegree(vector<intersection_t> &intersections, unsigned int nVertices) {
    vector<unsigned int> map1(nVertices, 0), map2(nVertices, 0);

    for (auto u = intersections.begin(); u != intersections.end(); ++u) {
        auto &u_vec = get<2>(*u);

        for (auto v = u+1; v != intersections.end(); ++v) {
            auto &v_vec = get<2>(*v);

            for (unsigned long i = 0; i < nVertices; i++) {
                if (u_vec[i] & v_vec[i]) {
                    ++map1[i];
                }
                else if (u_vec[i] != v_vec[i]) {
                    ++map2[i];
                }
            }
        }
    }

    sort(intersections.begin(), intersections.end(), [map1, map2](const intersection_t &a, const intersection_t &b) {
        const unsigned int i = get<0>(a), j = get<0>(b);

        return ((float)map1[i]/(float)map2[i]) > ((float)map1[j]/(float)map2[j]);

        if (((float)map1[i]/(float)map2[i]) > ((float)map1[j]/(float)map2[j])) {
            return true;
        }
        else if (((float)map1[i]/(float)map2[i]) == ((float)map1[j]/(float)map2[j])) {
            // if (map2[i] < map2[j]) {
            //     return true;
            // }
            // else if (map2[i] == map2[j]) {
                return get<1>(a) > get<1>(b);
            // }
        }

        return false;
    });
}

unsigned int getNodeDegree(vector<bool> &node, unsigned int start = 0) {
    unsigned int degree = start;

    for (bool n : node)
        degree += n;

    return degree;
}

vector<bool> mergeNodeNeighboors(vector<intersection_t> &intersections, unsigned int nVertices) {
    vector<bool> result(nVertices, true);

    vector<unsigned int> map3(nVertices, 0);
    for (auto u = intersections.begin(); u != intersections.end(); ++u) {
        auto &u_vec = get<2>(*u);

        for (auto v = u+1; v != intersections.end(); ++v) {
            auto &v_vec = get<2>(*v);

            for (unsigned long i = 0; i < nVertices; i++) {
                if (u_vec[i] & v_vec[i]) {
                    ++map3[i];
                }
            }
        }
    }

    for (auto v = intersections.begin(); v != intersections.end(); ++v) {
        unsigned int index = get<0>(*v);

        if (!result[index])
            continue;

        vector<bool> v_vec = get<2>(*v);
        for (unsigned int i = 0; i < nVertices; i++)
            result[i] = result[i] & v_vec[i];
        vector<unsigned int> map1(nVertices, 0), map2(nVertices, 0);

        // #pragma omp parallel for
        for (auto w = v+1; w != intersections.end(); ++w) {
            auto& w_vec = get<2>(*w);

            for (unsigned int i = 0; i < nVertices; ++i) {
                if (w_vec[i] & result[i]) {
                    ++map1[get<0>(*w)];
                }
                else if (w_vec[i] != result[i]) {
                    ++map2[get<0>(*w)];
                }
            }
        }

        stable_sort(v+1, intersections.end(), [map1, map2, map3](const intersection_t &a, const intersection_t &b) {
            const unsigned int i = get<0>(a), j = get<0>(b);

            if (((float)(map1[i]*map3[i])/(float)map2[i]) > ((float)(map1[j]*map3[j])/(float)map2[j])) {
                return true;
            }
            else if (map1[i] > map1[j]) {
                return true;
            }
            else if (map1[i] == map1[j]) {
                if (map2[i] < map2[j]) {
                    return true;
                }
            }

            return false;
        });
    }

    return result;
}

vector<unsigned int> getNodeIds(vector<bool> node) {
    vector<unsigned int> ids;

    for (unsigned int i = 0; i < node.size(); i++)
        if (node[i] != 0)
            ids.push_back(i);

    return ids;
}

vector<bool> getNodeMaximalClique(vector<vector<bool>> &graph, unsigned int nVertices, unsigned int node) {
    auto intersections = getNodeNeighboorsIntersections(graph, nVertices, node);

    // for (auto i : intersections) {
    //     cout << "bef " << (get<0>(i)+1) << "\t- ";
    //     for (auto n : getNodeIds(get<2>(i))) {
    //         cout << (n+1) << ' ';
    //     }
    //     cout << endl;
    // }

    updateNodeNeighboorsDegree(intersections, nVertices);

    // for (auto i : intersections) {
    //     cout << "mid " << (get<0>(i)+1) << "\t- ";
    //     for (auto n : getNodeIds(get<2>(i))) {
    //         cout << (n+1) << ' ';
    //     }
    //     cout << endl;
    // }

    auto a = mergeNodeNeighboors(intersections, nVertices);

    // for (auto i : intersections) {
    //     cout << "aft " << (get<0>(i)+1) << "\t- ";
    //     for (auto n : getNodeIds(get<2>(i))) {
    //         cout << (n+1) << ' ';
    //     }
    //     cout << endl;
    // }

    return a;
}

vector<unsigned int> getMaximalClique(vector<vector<bool>> &graph, unsigned int nVertices) {
    vector<tuple<unsigned int, int, vector<bool>>> cliques;
    unsigned int maxDegree = 0;

    #pragma omp parallel for shared(graph)
    for (unsigned int i = 0; i < nVertices; i++) {
        if (getNodeDegree(graph[i], 1) < maxDegree)
            continue;

        vector<bool> nodeClique = getNodeMaximalClique(graph, nVertices, i);
        unsigned int nodeDegree = getNodeDegree(nodeClique);

        #pragma omp critical
        {
            if (maxDegree < nodeDegree) {
                cliques.clear();
                maxDegree = nodeDegree;
            }
        }
        
        #pragma omp critical
        {
            if (maxDegree == nodeDegree) {
                cliques.emplace_back(i, 0, nodeClique);
            }
        }
    }

    // cout << "Cliques" << endl;

    // for (auto c : cliques) {
    //     for (auto n : getNodeIds(get<2>(c))) {
    //         cout << (n+1) << ' ';
    //     }
    //     cout << endl;
    // }

    return getNodeIds(get<2>(cliques[0]));
}
