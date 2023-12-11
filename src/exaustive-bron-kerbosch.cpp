/* F. Cazals, C. Karande,
 * A note on the problem of reporting maximal cliques,
 * Theoretical Computer Science,
 * Volume 407, Issues 1–3,
 * 2008,
 * Pages 564-568,
 * ISSN 0304-3975,
 * https://doi.org/10.1016/j.tcs.2008.05.010.
 * (https://www.sciencedirect.com/science/article/pii/S0304397508003903)
*/

/* Etsuji Tomita, Akira Tanaka, Haruhisa Takahashi,
 * The worst-case time complexity for generating all maximal cliques and computational experiments,
 * Theoretical Computer Science,
 * Volume 363, Issue 1,
 * 2006,
 * Pages 28-42,
 * ISSN 0304-3975,
 * https://doi.org/10.1016/j.tcs.2006.06.015.
 * (https://www.sciencedirect.com/science/article/pii/S0304397506003586)
 */

#include <iostream>
#include <vector>
#include <set>


void bronKerbosch(
    std::vector<std::vector<bool>>& graph,
    std::vector<unsigned int>& r,
    std::set<unsigned int>& p,
    std::set<unsigned int>& x,
    std::vector<unsigned int>& clique
) {
    // if P = {} and X = {} then
    if (p.empty() && x.empty()) {
        // Report X as maximal clique
        if (r.size() > clique.size()) {
            clique = r;
        }
        return;
    }
    // else

    // Assume P = { u_1, u_2, ..., u_k }
    // for i <- 1 to k do
    // v as u_i
    for (auto it = p.begin(); it != p.end();) {
        int v = *it;

        // R_new = R ∪ { v }
        std::vector<unsigned int> rNew = r;
        rNew.push_back(v);

        // P_new = P ∩ N[v]
        std::set<unsigned int> pNew;
        for (unsigned int w : p) {
            if (graph[v][w]) {
                pNew.insert(w);
            }
        }

        // X_new = X ∩ N[v]
        std::set<unsigned int> xNew;
        for (unsigned int w : x) {
            if (graph[v][w]) {
                xNew.insert(w);
            }
        }

        bronKerbosch(graph, rNew, pNew, xNew, clique);

        // X = X ∪ { v }
        x.insert(v);

        // P = P - {v}
        it = p.erase(it);
    }
}

std::vector<unsigned int> getMaximalClique(std::vector<std::vector<bool>>& graph, unsigned int nVertices) {
    std::vector<unsigned int> clique, r;
    std::set<unsigned int> p, x;

    for (unsigned int i = 0; i < nVertices; i++) {
        p.insert(i);
    }

    bronKerbosch(graph, r, p, x, clique);

    return clique;
}
