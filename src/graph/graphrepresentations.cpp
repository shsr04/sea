#include "sealib/graph/graphrepresentations.h"
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>
#include <algorithm>
#include <set>
#include "sealib/graph/compactgraph.h"
#include "sealib/graph/node.h"

using Sealib::Graphrepresentations;
using Sealib::CompactGraph;

void Graphrepresentations::standardToCrosspointer(uint* a) {
  uint n = a[0], v, u, pv, pu;
  // n = order of the graph
  v = pv = pu = 0;
  u = 1;
  while (u < n) {
    while (!(a[a[u]] > n || a[u] == a[u + 1])) {
      pu = a[u];
      v = a[pu];
      pv = a[v];
      a[pu] = pv;
      a[pv] = pu;
      ++a[v];
      ++a[u];
    }
    u = u + 1;
  }
  // restore T pointers
  v = n;
  while (v > 1) {
    a[v] = a[v - 1];
    --v;
  }
  a[1] = n + 2;
  return;
}

void Graphrepresentations::standardToBeginpointer(uint* a) {
  uint order = a[0];
  uint numEdges = a[order + 1];
  uint graphSize = order + numEdges + 2;
  for (uint i = order + 2; i < graphSize; ++i) {
    // checks if a[i] is not a node of order 0
    if (a[a[i]] != a[a[i] - 1] || i == order + 2) {
      a[i] = a[a[i]];
    }
  }
  return;
}

void Graphrepresentations::swappedBeginpointerToStandard(uint* a) {
  uint order = a[0];
  uint numEdges = a[order + 1];
  uint graphSize = order + numEdges + 2;
  for (uint i = order + 2; i < graphSize; ++i) {
    if (a[i] > order) {
      a[i] = a[a[i]];
    }
  }
  for (uint i = 1; i <= order; ++i) {
    a[i] = a[a[i]];
  }

  uint v = order;
  while (a[v] == v) {
    --v;
  }
  for (uint i = graphSize - 1; i > order + 1; --i) {
    if (a[i] == v) {
      a[i] = a[v];
      a[v] = i;
      --v;
      while (a[v] == v) {
        --v;
      }
    }
  }
  return;
}

/**
 * Transforms a graph from cross or beginpointer representation to
 * swapped cross or beginpointer representation
 * @param a graph in cross or beginpointer representation
 */
void Graphrepresentations::swapRepresentation(uint* a) {
  uint order = a[0];
  for (uint i = 1; i <= order; ++i) {
    uint temp = a[a[i]];
    a[a[i]] = i;
    a[i] = temp;
  }
  return;
}
