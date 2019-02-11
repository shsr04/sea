#ifndef SRC_SUBGRAPH_BASESUBGRAPH_H_
#define SRC_SUBGRAPH_BASESUBGRAPH_H_
#include <sealib/dictionary/rankselect.h>
#include <sealib/graph/undirectedgraph.h>
#include <tuple>
#include "./subgraph.h"

namespace Sealib {
class BaseSubGraph : public SubGraph {
    typedef std::shared_ptr<UndirectedGraph> rgraph_t;
 private:
    rgraph_t rGraph;

 public:
    uint head(uint u, uint k) const override;

    std::tuple<uint, uint> mate(uint u, uint k) const override;

    uint phi(uint u) const override;
    uint psi(uint a) const override;
    uint phiInv(uint u) const override;
    uint psiInv(uint a) const override;
    explicit BaseSubGraph(stack_t *stack, std::shared_ptr<UndirectedGraph> rGraph);
    ~BaseSubGraph() override;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_BASESUBGRAPH_H_
