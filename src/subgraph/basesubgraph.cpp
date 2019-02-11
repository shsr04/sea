#include "./basesubgraph.h"
#include <iostream>
#include <utility>

/**
 * deletion of the subgraphs will be in the subgraph stack
 * or with the deletion of the subgraph stack
 */
Sealib::BaseSubGraph::~BaseSubGraph() = default;

Sealib::BaseSubGraph::BaseSubGraph(stack_t *stack_, rgraph_t rGraph_) :
    SubGraph(0, 0, stack_),
    rGraph(std::move(rGraph_)) {
    uint dSum = 0;
    bitset_t q(rGraph->getOrder());
    for (uint i = 0; i < rGraph->getOrder(); i++) {
        uint deg = rGraph->deg(i);
        dSum += deg;
        if (deg > 0) {
            q[i] = 1;
        }
    }

    bitset_t p(dSum);
    uint deg = 0;
    for (uint i = 0; i < rGraph->getOrder(); i++) {
        if (rGraph->deg(i) > 0) {
            deg += rGraph->deg(i);
            p[deg - 1] = 1;
        }
    }
    qSelect = new RankSelect(q);
    pSelect = new RankSelect(p);
}

uint Sealib::BaseSubGraph::head(uint u,
                                         uint k) const {
    return rGraph->head(static_cast<uint>(u - 1), static_cast<uint>(k - 1)) + 1;
}

std::tuple<uint, uint>
Sealib::BaseSubGraph::mate(uint u, uint k) const {
    uint k2 = rGraph->mate(
        static_cast<uint>(u - 1),
        static_cast<uint>(k - 1));
    return std::tuple<uint, uint>(
            rGraph->head(static_cast<uint>(u-1), static_cast<uint>(k-1)) + 1,
            k2 + 1);
}

uint Sealib::BaseSubGraph::phi(uint u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return u;
}
uint Sealib::BaseSubGraph::psi(uint a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return a;
}
uint Sealib::BaseSubGraph::phiInv(uint u) const {
    if (u == 0) {
        throw std::invalid_argument("u needs to be > 0");
    }
    return u;
}
uint Sealib::BaseSubGraph::psiInv(uint a) const {
    if (a == 0) {
        throw std::invalid_argument("a needs to be > 0");
    }
    return a;
}
