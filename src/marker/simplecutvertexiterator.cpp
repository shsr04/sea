#include "./simplecutvertexiterator.h"

namespace Sealib {

void SimpleCutVertexIterator::init() {
    uint i = 0;
    bool isRootProtected = true;  // root vertex is allowed one child cut vertex
    for (uint a = 0; a < n; a++) {
        if (color[a] == DFS_WHITE) {
            DFS::visit_standard(
                a, g, &color, &s,
                [&](uint u) {
                    number[u] = lowpt[u] = i;
                    i++;
                },
                [&](uint u, uint v) {
                    if (number[v] == INVALID) {
                        tree.push_back(v);
                    } else if (number[v] < number[u] && number[v] < lowpt[u]) {
                        lowpt[u] = number[v];
                    }
                },
                [&](uint u, uint v) {
                    if (tree.back() == v) {
                        tree.pop_back();
                        if (lowpt[v] < lowpt[u]) lowpt[u] = lowpt[v];
                    }
                    if (number[u] != 0 && lowpt[v] >= number[u]) {
                        if (number[u] != 0 || !isRootProtected) {
                            cut.insert(u);
                        } else {
                            isRootProtected = false;
                        }
                    }
                },
                [&](uint u) {
                    if (number[u] == 0) {
                        // setup next spanning tree
                        i = 0;
                        isRootProtected = true;
                    }
                });
        }
    }

    cutI = cut.begin();
}

bool SimpleCutVertexIterator::more() { return cutI != cut.end(); }

uint SimpleCutVertexIterator::next() { return *cutI++; }

}  // namespace Sealib
