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
    uint64_t dSum = 0;
    bitset_t q(rGraph->getOrder());
    for (uint32_t i = 0; i < rGraph->getOrder(); i++) {
        uint64_t deg = rGraph->deg(i);
        dSum += deg;
        if (deg > 0) {
            q[i] = 1;
        }
    }

    bitset_t p(dSum);
    uint64_t deg = 0;
    for (uint32_t i = 0; i < rGraph->getOrder(); i++) {
        if (rGraph->deg(i) > 0) {
            deg += rGraph->deg(i);
            p[deg - 1] = 1;
        }
    }
    qSelect = new RankSelect(std::move(q));
    pSelect = new RankSelect(std::move(p));
}

uint64_t Sealib::BaseSubGraph::head(uint64_t u,
                                         uint64_t k) const {
    return rGraph->head(static_cast<uint64_t>(u - 1), static_cast<uint64_t>(k - 1)) + 1;
}

std::tuple<uint64_t, uint64_t>
Sealib::BaseSubGraph::mate(uint64_t u, uint64_t k) const {
    uint64_t k2 = rGraph->mate(
        static_cast<uint64_t>(u - 1),
        static_cast<uint64_t>(k - 1));
    return std::tuple<uint64_t, uint64_t>(
            rGraph->head(static_cast<uint64_t>(u-1), static_cast<uint64_t>(k-1)) + 1,
            k2 + 1);
}

uint64_t Sealib::BaseSubGraph::phi(uint64_t u) const {
    if (u == 0) {
        throw ZeroArgumentGiven();
    }
    return u;
}
uint64_t Sealib::BaseSubGraph::psi(uint64_t a) const {
    if (a == 0) {
        throw ZeroArgumentGiven();
    }
    return a;
}
uint64_t Sealib::BaseSubGraph::phiInv(uint64_t u) const {
    if (u == 0) {
        throw ZeroArgumentGiven();
    }
    return u;
}
uint64_t Sealib::BaseSubGraph::psiInv(uint64_t a) const {
    if (a == 0) {
        throw ZeroArgumentGiven();
    }
    return a;
}
