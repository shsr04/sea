#include "./simplebfs.h"

namespace Sealib {

SimpleBFS::SimpleBFS(Graph const *graph, Consumer pp, BiConsumer pe)
    : g(graph), n(g->getOrder()), color(n), preprocess(pp), preexplore(pe) {
    for (uint a = 0; a < n; a++) {
        color[a] = BFS_WHITE;
    }
}

void SimpleBFS::init() {
    color[0] = BFS_GRAY1;
    queue.push({0, 0});
}

bool SimpleBFS::more() { return !queue.empty(); }

bool SimpleBFS::nextComponent() {
    for (uint u = 0; u < n; u++) {
        if (color[u] == BFS_WHITE) {
            queue.push({u, 0});
            return true;
        }
    }
    return false;
}

std::pair<uint, uint> SimpleBFS::next() {
    std::pair<uint, uint> p = queue.front();
    queue.pop();
    uint u = p.first, d = p.second;
    preprocess(u);
    for (uint k = 0; k < g->deg(u); k++) {
        uint v = g->head(u, k);
        preexplore(u, v);
        if (color[v] == BFS_WHITE) {
            color[v] = BFS_GRAY1;
            queue.push({v, d + 1});
        }
    }
    if (queue.size() > qmax) qmax = queue.size();
    color[u] = BFS_BLACK;
    return {u, d};
}

}  // namespace Sealib
