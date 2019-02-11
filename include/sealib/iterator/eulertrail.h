#ifndef SEALIB_ITERATOR_EULERTRAIL_H_
#define SEALIB_ITERATOR_EULERTRAIL_H_

#include <sealib/dictionary/rankselect.h>
#include <sealib/graph/undirectedgraph.h>
#include <ostream>
#include <vector>
#include <tuple>

namespace Sealib {
template<class TrailStructureType>
/**
 * Space efficient Euler Trail class. Initialized with an undirected graph object G_0,
 * creates a set of euler partitions for G_0 during construction.
 * Uses O(n+m) time for the construction and occupies O(n) space after construction.
 * @tparam TrailStructureType
 */
class EulerTrail {
 private:
    std::shared_ptr<Sealib::UndirectedGraph> graph;
    std::vector<TrailStructureType>  trail;
    Sealib::RankSelect trailStarts;

    uint findStartingNode();
    std::vector<TrailStructureType> initializeTrail();
    Sealib::Bitset<uint8_t> findTrailStarts();

    /**
     * Iterator used for iterating over the created trails.
     * the operator*() returns a tuple containing the next vertex that is traversed in the trail,
     * and a boolean that is set to true, if that vertex is the end of a trail.
     */
    class iterator {
     public:
        explicit iterator(const EulerTrail<TrailStructureType> &Container, uint index = 1);
        std::tuple<uint, bool> operator*() const;
        iterator &operator++();
        iterator &operator++(int);
        bool operator!=(const iterator &) const;
     private:
        const EulerTrail<TrailStructureType> &eulerTrail;
        uint nIndex;
        uint mIndex;
        uint arc;
        bool ending;
    };

 public:
    /**
     *
     * @param graph - undirected graph object for which the euler trails should be created
     */
    explicit EulerTrail(const std::shared_ptr<Sealib::UndirectedGraph> &graph);

    /**
     * @return iterates over all trails and writes them to the output steam
     */
    friend std::ostream &operator<<(std::ostream &os,
                                    const EulerTrail<TrailStructureType> &eulerTrail) {
        uint tourNum = 1;
        bool newTour = true;
        for (auto v0 : eulerTrail) {
            bool ending = std::get<1>(v0);
            if (!ending) {
                if (newTour) {
                    newTour = false;
                    os << "T" << tourNum++ << ": ";
                }
                os << std::get<0>(v0) << " -> ";
            }
            if (ending) {
                newTour = true;
                os << std::get<0>(v0) << std::endl;
            }
        }
        return os;
    }

    /**
     * Iterator which enables to iterate over all created euler trails for the graph G_0.
     */
    iterator begin() const;
    iterator end() const;
};
}  // namespace Sealib
#endif  // SEALIB_ITERATOR_EULERTRAIL_H_
