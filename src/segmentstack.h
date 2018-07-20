#ifndef SRC_SEGMENTSTACK_H_
#define SRC_SEGMENTSTACK_H_

#include <stack>
#include <tuple>

#define DFS_NO_MORE_NODES (unsigned)11
#define DFS_DO_RESTORE (unsigned)12
typedef std::tuple<uint, uint> State;

/*
  auxiliary stack, used by the EHKDFS
  - the segment stack has a low and a high segment
  - the trailer (the last slot) of each segment is kept
  - on push, both segments are filled from low to high
    - if both segments are full, the low segment is dropped and a new segment is
  placed above of the high segment
  - on pop, values are taken from high and then low segment
    - if the segments are empty and there are trailers left, a restoration
  request is issued
    - if the segments are empty and the trailers are empty, there is no more
  data in the stack
  - instead of using a list of trailers, there is the option to store only the
  last trailer in a variable (used in EHKDFS)

  @author Simon Heuser
*/
namespace Sealib {
class SegmentStack {
 public:
  /* size: number of stored entries, useTrailers: if true, use stack of
   * trailers; if false, use last pushed entry*/
  explicit SegmentStack(uint size, uint segmentSize, bool useTrailers);
  ~SegmentStack();

  int push(State u);
  int pop(State *r);
  bool empty();
  void dropAll();
  /* save a trailer to survive the restoration */
  void saveTrailer();
  /* is the restoration finished? (i.e. saved trailer and actual trailer are
   * aligned) */
  bool isAligned();
};
}

#endif  // SRC_SEGMENTSTACK_H_
