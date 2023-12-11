#include <iostream>
#include <algorithm>
#include <vector>
#include <tuple>
#include <set>

#include <omp.h>


std::vector<unsigned int> getNodeMaximalClique(std::vector<std::vector<bool>>& graph, unsigned int nVertices, unsigned int node) {
    std::vector<std::tuple<unsigned int, unsigned int, std::vector<bool>>> intersections;

    #pragma omp parallel num_threads(nVertices)
    {
        #pragma omp for schedule(static, 12)
        for (unsigned int i = 0; i < nVertices; i++) {
            if (i == node || graph[node][i] == 0)
                continue;

            std::vector<bool> intersection(nVertices, 0);

            for (unsigned int j = 0; j < nVertices; j++)
                if (j == node || j == i || graph[node][j] & graph[i][j])
                    intersection[j] = 1;

            #pragma omp critical
            {
                intersections.emplace_back(i, 0, intersection);
            }
        }
    }

    #pragma omp parallel num_threads(intersections.size())
    {
        #pragma omp for schedule(static, 12)
        for (auto u = intersections.begin(); u != intersections.end(); ++u) {
            auto& u_vec = std::get<2>(*u);
            unsigned int degree1 = 0;

            for (auto v = u+1; v != intersections.end(); ++v) {
                if (!u_vec[std::get<0>(*v)]) continue;

                auto& v_vec = std::get<2>(*v);
                unsigned int degree2 = 0;

                for (unsigned long i = 0; i < nVertices; i++)
                    if (u_vec[i] & v_vec[i])
                        ++degree2;
                
                if (degree1 < degree2)
                    degree1 = degree2;

                if (std::get<1>(*v) < degree2)
                    std::get<1>(*v) = degree2;
            }

            if (std::get<1>(*u) < degree1)
                std::get<1>(*u) = degree1;
        }
    }

    std::sort(intersections.begin(), intersections.end(), [](const auto& a, const auto& b) {
        return std::get<1>(a) > std::get<1>(b);
    });

    std::vector<bool> clique(nVertices);

    for (auto u = intersections.begin(); u != intersections.end(); ++u) {
        std::fill(clique.begin(), clique.end(), 1);

        for (auto v = u+1; v != intersections.end(); ++v) {
            if (clique[std::get<0>(*v)] == 0)
                continue;

            for (unsigned int i = 0; i < nVertices; i++)
                clique[i] = clique[i] & std::get<2>(*v)[i];
        }

        if (clique[std::get<0>(*u)])
            break;
    }

    std::vector<unsigned int> ids;

    for (unsigned int i = 0; i < clique.size(); i++)
        if (clique[i] != 0)
            ids.push_back(i);

    return ids;
}

std::vector<unsigned int> getMaximalClique(std::vector<std::vector<bool>>& graph, unsigned int nVertices) {
    std::vector<unsigned int> clique;

    #pragma omp parallel num_threads(omp_get_max_threads() / nVertices)
    {
        #pragma omp for schedule(static, 12)
        for (unsigned int i = 0; i < nVertices; i++) {
            unsigned int degree = 1;

            for (bool n : graph[i])
                degree += n;

            if (degree < clique.size())
                continue;

            auto nodeClique = getNodeMaximalClique(graph, nVertices, i);

            if (nodeClique.size() > clique.size())
                clique = nodeClique;
        }
    }

    return clique;
}
