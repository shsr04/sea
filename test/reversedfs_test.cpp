#include "sealib/reversedfs.h"
#include <gtest/gtest.h>
#include "sealib/graphcreator.h"

using namespace Sealib;  // NOLINT

TEST(ReverseDFSTest, postprocess) {
  Graph *g = Sealib::GraphCreator::createRandomFixed(1024, 16);
  std::vector<uint> ref;
  DFS::nloglognBitDFS(g, DFS_NOP_PROCESS, DFS_NOP_EXPLORE, DFS_NOP_EXPLORE,
                      [&ref](uint u) { ref.push_back(u); });
  std::reverse(ref.begin(), ref.end());
  std::vector<uint> seq;
  ReverseDFS d(g);
  d.init();
  while (d.more()) {
    UserCall a = d.next();
    if (a.type == UserCall::postprocess) {
      seq.push_back(a.u);
    }
  }
  ASSERT_EQ(ref.size(), seq.size());
  for (uint a = 0; a < ref.size(); a++) {
    EXPECT_EQ(seq[a], ref[a]);
  }
}
