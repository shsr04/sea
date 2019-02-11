#ifndef SRC_TRAIL_SIMPLETRAIL_H_
#define SRC_TRAIL_SIMPLETRAIL_H_

#include <vector>
#include <tuple>

namespace Sealib {
class SimpleTrail {
 private:
    std::vector<std::tuple<uint, uint>> trail;
 public:
    SimpleTrail();
    void insertSubTrail(const SimpleTrail &subTrail, uint idx);
    void pushBackSubTrail(const SimpleTrail &subTrail);
    void addArc(std::tuple<uint, uint> arc);
    uint getFirstIndexOf(std::tuple<uint, uint> arc) const;
    const std::vector<std::tuple<uint, uint>> &getTrail() const;

    std::tuple<uint, uint> getOutgoingFrom(uint u) const;
};
}  // namespace Sealib
#endif  // SRC_TRAIL_SIMPLETRAIL_H_
