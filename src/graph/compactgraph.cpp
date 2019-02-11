#include <memory>
#include "sealib/graph/compactgraph.h"

using Sealib::CompactGraph;

CompactGraph::CompactGraph(uint _A[]) {
	this->A = std::unique_ptr<uint[]>(_A);
}

uint CompactGraph::deg(uint u) const {
	// Fix index u = u+1
	++u;
	// Following node
	if (A[u] == u) {
		return 0;
	}
	uint v = u + 1;
	if (u != getOrder()) {
		return A[v] - A[u];

		/*
		uint skip = 0;
		while (A[v + skip] <= getOrder() && (v + skip) <= getOrder()) {
			++skip;
		}
		v += skip;
		if (v > getOrder()) {
			return (A[0] + A[A[0]+1] + 2) - A[u];
		}
		return A[v] - A[u];
		*/
	} else {
		return (A[0] + A[A[0]+1] + 2) - A[u];
	}
}

uint CompactGraph::head(uint u, uint k) const {
	return A[A[u+1]+k];
}

uint CompactGraph::getOrder() const {
	return A[0];
}

uint* CompactGraph::getData() {
	return this->A.get();
}
