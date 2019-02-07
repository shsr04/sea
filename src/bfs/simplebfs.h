#ifndef SRC_BFS_SIMPLEBFS_H_
#define SRC_BFS_SIMPLEBFS_H_
#include <queue>
#include <vector>
#include "sealib/iterator/bfs.h"

namespace Sealib {
class SimpleBFS : Iterator<std::pair<uint, uint>> {
 public:
    SimpleBFS(Graph const *g, Consumer preprocess, BiConsumer preexplore);

    void init() override;

    bool more() override;

    bool nextComponent();

    std::pair<uint, uint> next() override;

    void forEach(std::function<void(std::pair<uint, uint>)> f) override {
        do {
            while (more()) f(next());
        } while (nextComponent());
    }

    uint64_t byteSize() const {
        return color.capacity() * sizeof(uint8_t) +
               qmax * sizeof(std::pair<uint, uint>);
    }

 private:
    Graph const *g;
    uint n;
    std::vector<uint8_t> color;
    std::queue<std::pair<uint, uint>> queue;
    uint64_t qmax = 0;
    Consumer preprocess;
    BiConsumer preexplore;
};
}  // namespace Sealib
#endif  // SRC_BFS_SIMPLEBFS_H_
