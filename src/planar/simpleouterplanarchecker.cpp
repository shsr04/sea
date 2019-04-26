#include "./simpleouterplanarchecker.h"

namespace Sealib {

static uint64_t countEdges(UndirectedGraph const &g) {
    uint64_t r = 0;
    for (uint64_t a = 0; a < g.getOrder(); a++) {
        for (uint64_t b = 0; b < g.deg(a); b++) {
            r++;
        }
    }
    return r / 2;
}

SimpleOuterplanarChecker::SimpleOuterplanarChecker(UndirectedGraph const &graph)
    : g(graph),
      n(g.getOrder()),
      m(countEdges(g)),
      present(n),
      virtualAdj(n),
      isOuter(n) {
    present.flip();
    for (uint64_t u = 0; u < n; u++) {
        for (uint64_t k = 0; k < g.deg(u); k++) {
            isOuter[u].push_back(0);
        }
    }
}

bool SimpleOuterplanarChecker::isOuterplanar() {
    if (m > 2 * n - 3) {
        return false;
    }

    uint64_t actualOrder = n;
    for (uint64_t u = 0; u < g.getOrder(); u++) {
        if (g.deg(u) < 2) {
            return false;
        } else if (g.deg(u) == 2) {
            degTwo.push(u);
        }
    }

    while (actualOrder > 3) {
        if (degTwo.empty()) {
            return false;
        }
        uint64_t u = degTwo.front();
        degTwo.pop();
        present[u] = 0;
        actualOrder--;
        uint64_t v0 = actualHead(u, 0), v1 = actualHead(u, 1);
        if (edgeBetween(v0, v1)) {
            if (actualDeg(v0) == 2) degTwo.push(v0);
            if (actualDeg(v1) == 2) degTwo.push(v1);
            uint64_t i0 = outerIndex(v0, v1), i1 = outerIndex(v1, v0);
            if (!isOuter[v0][i0]) {
                isOuter[v0][i0] = isOuter[v1][i1] = 1;
            } else {
                return false;
            }
        } else {
            connectNeighbours(u);
            isOuter[v0].push_back(1);
            isOuter[v1].push_back(1);
        }
    }
    return true;
}

uint64_t SimpleOuterplanarChecker::actualDeg(uint64_t u) {
    uint64_t r = 0;
    std::vector<std::pair<uint64_t, uint64_t>> adj = g.getNode(u).getAdj();
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (present[adj[a].first]) r++;
    }
    for (uint64_t a = 0; a < virtualAdj[u].size(); a++) {
        if (present[virtualAdj[u][a]]) r++;
    }
    return r;
}

uint64_t SimpleOuterplanarChecker::actualHead(uint64_t u, uint64_t k) {
    uint64_t i = 0;
    std::vector<std::pair<uint64_t, uint64_t>> const &adj =
        g.getNode(u).getAdj();
    for (uint64_t a = 0; a < g.deg(u); a++) {
        if (present[adj[a].first]) {
            if (i == k) {
                return adj[a].first;
            }
            i++;
        }
    }
    for (uint64_t a = 0; a < virtualAdj[u].size(); a++) {
        if (present[virtualAdj[u][a]]) {
            if (i == k) {
                return virtualAdj[u][a];
            }
            i++;
        }
    }
    return INVALID;
}

bool SimpleOuterplanarChecker::edgeBetween(uint64_t u, uint64_t v) {
    for (uint64_t a = 0; a < actualDeg(u); a++) {
        if (actualHead(u, a) == v) return true;
    }
    return false;
}

uint64_t SimpleOuterplanarChecker::outerIndex(uint64_t u, uint64_t v) {
    uint64_t b;
    for (b = 0; b < g.deg(u); b++) {
        if (g.head(u, b) == v) return b;
    }
    for (uint64_t a = 0; a < virtualAdj[u].size(); a++) {
        if (virtualAdj[u][a] == v) return b + a;
    }
    return INVALID;
}

void SimpleOuterplanarChecker::connectNeighbours(uint64_t u) {
    uint64_t v0 = actualHead(u, 0), v1 = actualHead(u, 1);
    virtualAdj[v0].push_back(v1);
    virtualAdj[v1].push_back(v0);
}

}  // namespace Sealib
