#ifndef SRC_SUBGRAPH_RECURSIVESUBGRAPH_H_
#define SRC_SUBGRAPH_RECURSIVESUBGRAPH_H_
#include <sealib/dictionary/rankselect.h>
#include <tuple>
#include "./subgraph.h"

namespace Sealib {
class RecursiveSubGraph : public SubGraph {
 protected:
    rankselect_t vSelect;
    rankselect_t aSelect;

    bitset_t initializeVSelect(const bitset_t &v);
    bitset_t initializeASelect(const bitset_t &v);

    inline uint select_v(uint i) const {
        return vSelect.select(i);
    }

    inline uint select_a(uint i) const {
        return aSelect.select(i);
    }

    inline uint rank_v(uint i) const {
        return vSelect.rank(i);
    }

    inline uint rank_a(uint i) const {
        return aSelect.rank(i);
    }

 public:
    RecursiveSubGraph(stack_t *stack,
                      uint sidx_,
                      uint ridx_,
                      const Sealib::Bitset<uint8_t> &v,
                      const Sealib::Bitset<uint8_t> &a);

    uint head(uint u, uint k) const override;
    std::tuple<uint, uint> mate(uint u, uint k) const override;

    uint phi(uint u) const override;
    uint psi(uint a) const override;
    uint phiInv(uint u) const final;
    uint psiInv(uint a) const final;
    ~RecursiveSubGraph() override;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_RECURSIVESUBGRAPH_H_
