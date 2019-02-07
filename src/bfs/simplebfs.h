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

 private:
    Graph const *g;
    uint n;
    std::vector<uint8_t> color;
    std::vector<uint> dist;
    std::queue<std::pair<uint, uint>> queue;
    Consumer preprocess;
    BiConsumer preexplore;
};
}  // namespace Sealib
#endif  // SRC_BFS_SIMPLEBFS_H_
