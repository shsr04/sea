#ifndef SRC_TRAIL_NAIVETRAILSTRUCTURE_H_
#define SRC_TRAIL_NAIVETRAILSTRUCTURE_H_

#include <map>
#include <list>

namespace Sealib {
class NaiveTrailStructure {
 private:
    std::map<uint, uint> pairedEdges;
    std::list<uint> unusedEdges;
    uint currentDegree;
    bool black;
    bool grey;
 public:
    explicit NaiveTrailStructure(uint degree_);
    uint leave();
    uint enter(uint e);
    bool isBlack();
    bool isWhite();
    bool isGrey();
    bool isEven();
    uint getMatched(uint i);
};
}  // namespace Sealib
#endif  // SRC_TRAIL_NAIVETRAILSTRUCTURE_H_
