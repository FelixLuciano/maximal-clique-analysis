#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <set>


std::vector<std::tuple<unsigned int, unsigned int, std::vector<bool>>> getNodeNeighboorsIntersections(
    std::vector<std::vector<bool>>& graph,
    unsigned int nVertices,
    unsigned int node
) {
    std::vector<std::tuple<unsigned int, unsigned int, std::vector<bool>>> intersections;

    for (unsigned int i = 0; i < nVertices; i++) {
        if (i == node || graph[node][i] == 0)
            continue;

        std::vector<bool> intersection(nVertices, 0);
        int degree = 0;

        for (unsigned int j = 0; j < nVertices; j++)
            if (j == node || j == i || graph[node][j] & graph[i][j]) {
                intersection[j] = 1;
                ++degree;
            }

        intersections.emplace_back(i, degree, intersection);
    }

    return intersections;
}

void updateNodeNeighboorsDegree(
    std::vector<std::tuple<unsigned int, unsigned int, std::vector<bool>>>& intersections,
    unsigned int nVertices
) {
    for (auto u = intersections.begin(); u != intersections.end(); ++u) {
        auto& u_vec = std::get<2>(*u);
        int degree = 0;

        for (auto v = u+1; v != intersections.end(); ++v) {
            if (!u_vec[std::get<0>(*v)]) continue;

            auto& v_vec = std::get<2>(*v);
            degree = 0;

            for (unsigned long i = 0; i < nVertices; i++) {
                if (u_vec[i] & v_vec[i]) {
                    ++degree;
                }
            }

            std::get<1>(*v) += degree;
        }

        std::get<1>(*u) += degree;
    }

    std::stable_sort(intersections.begin(), intersections.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) > std::get<1>(b);
    });
}

unsigned int getNodeDegree(std::vector<bool>& node, unsigned int start = 0) {
    unsigned int degree = start;

    for (bool n : node)
        degree += n;

    return degree;
}

std::vector<bool> mergeNodeNeighboors(
    std::vector<std::tuple<unsigned int, unsigned int, std::vector<bool>>>& intersections,
    unsigned int nVertices
) {
    std::vector<bool> result(nVertices, true);
    auto v = intersections.begin();

    for (; v != intersections.end(); ++v) {
        unsigned int index = std::get<0>(*v);

        if (!result[index])
            continue;

        std::vector<bool> v_vec = std::get<2>(*v);
        for (unsigned int i = 0; i < nVertices; i++) {
            result[i] = result[i] & v_vec[i];
        }

        for (auto w = v+1; w != intersections.end(); ++w) {
            auto& w_vec = std::get<2>(*w);

            unsigned int degree = 0;
            for (unsigned int i = 0; i < w_vec.size(); ++i)
                if (w_vec[i] & result[i])
                    ++degree;

            std::get<1>(*w) = degree;
        }

        std::sort(v+1, intersections.end(), [](auto &a, auto &b) {
            if (std::get<1>(a) > std::get<1>(b)) {
                return true;
            }
            else if (std::get<1>(a) == std::get<1>(b)) {
                return getNodeDegree(std::get<2>(a)) < getNodeDegree(std::get<2>(b));
            }

            return false;
        });
    }

    return result;
}

std::vector<unsigned int> getNodeIds(std::vector<bool> node) {
    std::vector<unsigned int> ids;

    for (unsigned int i = 0; i < node.size(); i++)
        if (node[i] != 0)
            ids.push_back(i);

    return ids;
}

std::vector<bool> getNodeMaximalClique(
    std::vector<std::vector<bool>>& graph,
    unsigned int nVertices,
    unsigned int node
) {
    std::vector<std::tuple<unsigned int, unsigned int, std::vector<bool>>> intersections = getNodeNeighboorsIntersections(graph, nVertices, node);

    updateNodeNeighboorsDegree(intersections, nVertices);

    return mergeNodeNeighboors(intersections, nVertices);
}

std::vector<unsigned int> getMaximalClique(std::vector<std::vector<bool>>& graph, unsigned int nVertices) {
    std::vector<std::tuple<unsigned int, int, std::vector<bool>>> cliques;
    unsigned int maxDegree = 0;

    for (unsigned int i = 0; i < nVertices; i++) {
        if (getNodeDegree(graph[i], 1) < maxDegree)
            continue;

        std::vector<bool> nodeClique = getNodeMaximalClique(graph, nVertices, i);
        unsigned int nodeDegree = getNodeDegree(nodeClique);

        if (maxDegree < nodeDegree) {
            cliques.clear();
            maxDegree = nodeDegree;
        }
        
        if (maxDegree == nodeDegree) {
            cliques.emplace_back(i, 0, nodeClique);
        }
    }

    return getNodeIds(std::get<2>(cliques[0]));
}
