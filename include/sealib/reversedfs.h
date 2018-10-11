#ifndef SEALIB_REVERSEDFS_H_
#define SEALIB_REVERSEDFS_H_
#include <stack>
#include <vector>
#include "sealib/_types.h"
#include "sealib/compactarray.h"
#include "sealib/dfs.h"
#include "sealib/graph.h"
#include "sealib/iterator.h"

namespace Sealib {
/**
* Representation of a user call. A sequence of these can be retrieved from
* the ReverseDFS iterator.
*/
struct UserCall {
 public:
  enum Type { preprocess = 0, preexplore, postexplore, postprocess };
  unsigned type;
  uint u, v;
  UserCall(unsigned p1, uint p2, uint p3 = 0) : type(p1), u(p2), v(p3) {}
};

/**
* Reverse DFS iterator.
*/
class ReverseDFS : Iterator<UserCall>, DFS {
 public:
  explicit ReverseDFS(Graph *);
  ~ReverseDFS();

  /**
   * Run a n*log(log(n))-Bit DFS to record data about the intervals
   */
  void init() override;

  /**
   * @return true if there are more UserCalls in the DFS
   */
  bool more() override;

  /**
   * Get the next user call from the interval.
   * If necessary, first reconstruct the stack and simulate the DFS until the
   * end of the interval.
   * @return next user call from the reverse sequence
   */
  UserCall next() override;

 private:
  struct IntervalData {
   public:
    Pair h1 = Pair(INVALID, INVALID),
         h2 = Pair(INVALID,
                   INVALID);  // top entries at start and end of the interval
    Pair hd;                  // value of deepest entry
    uint hdc = std::numeric_limits<int>::max();  // index of deepest entry
    uint size = 0;  // call counter for the interval
  };

  Graph *g;
  uint n, r, w;
  CompactArray c;
  CompactArray d, f;
  uint ns = 0;
  IntervalData *i;
  uint j = 0;  // interval pointer
  std::vector<UserCall> seq;
  uint sp = 0;

  void storeTime(unsigned df, uint u);
  void updateInterval(Pair top, bool end = false);

  std::stack<Pair> reconstructPart(Pair from, Pair to);

  std::vector<UserCall> simulate(std::stack<Pair> *sj, Pair until);
};
}  // namespace Sealib
#endif  // SEALIB_REVERSEDFS_H_