#include "./simplecutvertexiterator.h"
#include <cstdio>

namespace Sealib {

void SimpleCutVertexIterator::init() {
    CompactArray color = CompactArray(n);
    StaticSpaceStorage s = StaticSpaceStorage(g);
    uint i = 0;
    for (uint a = 0; a < n; a++) {
        if (color.get(a) == DFS_WHITE) {
            DFS::visit_nplusm(
                a, g, &color, &s,
                [&](uint u) {
                    number[u] = lowpt[u] = i;
                    i++;
                },
                [&](uint u, uint k) {
                    uint v = g->head(u, k);
                    if (color.get(v) == DFS_GRAY && number[v] < lowpt[u]) {
                        printf("lowpt[%u] <- %u\n", u, number[v]);
                        lowpt[u] = number[v];
                    }
                },
                [&](uint u, uint k) {
                    uint v = g->head(u, k);
                    if (lowpt[v] < lowpt[u]) lowpt[u] = lowpt[v];
                    if (lowpt[v] >= number[u]) {
                        printf("cut + %u\n", u);
                        cut.insert(u);
                    }
                },
                DFS_NOP_PROCESS);
        }
    }
    cutI = cut.begin();
}

bool SimpleCutVertexIterator::more() { return cutI != cut.end(); }

uint SimpleCutVertexIterator::next() { return *cutI++; }

}  // namespace Sealib