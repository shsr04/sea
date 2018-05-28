#ifndef DFS_H
#define DFS_H
#include <stdio.h>
#include <math.h>
#include "sealib/graph.h"
#include "sealib/stack.h"
#include "sealib/compactarray.h"
#define DFS_WHITE 0
#define DFS_GRAY 1
#define DFS_BLACK 2
typedef void (*UserFunc1)(Node*);
typedef void (*UserFunc2)(Node*,Node*);
typedef unsigned int uint;

/*
	This class contains depth-first search algorithms.
	The depth-first search of a graph processes all its nodes and
	explores all its edges.
	During this procedure, the nodes will be colored
	from white (initial) to gray (being processed) to black (finished processing).

	The following DFS variants are available:
	- StandardDFS: the normal DFS, uses implicit recursion stack
*/
class DFS {
	private:
		/* @param u: vertex (in V) to be processed
		*/
		static void process_standard(Graph *g,UserFunc1 preProcess,UserFunc2 preExplore,
			UserFunc2 postExplore,UserFunc1 postProcess,uint *color,uint u);

		static void process_small(uint node,
				Graph *g,CompactArray *color,
				UserFunc1 preProcess,UserFunc2 preExplore,
				UserFunc2 postExplore,UserFunc1 postProcess,
				double epsilon);

		static void tryPush(uint u,Stack *low,Stack *high,Stack *trailers);
		static uint tryPop(Stack *low,Stack *high,Stack *trailers);

	public:

		/* runStandardDFS:
			Run a standard depth-first search over a graph. <br>
			EFFICIENCY: O(n+m) time, O(n log n) bits
			@param g graph G=(V,E) to iterate over
			@param [pre|post][Process|Explore]: user-defined functions <br>
				to be executed before/after processing a node/exploring an edge
		*/
		static void runStandardDFS(Graph *g,UserFunc1 preprocess,
			UserFunc2 preexplore,UserFunc2 postexplore,
			UserFunc1 postprocess);

		static void runSmallDFS(Graph *g,UserFunc1 preprocess,
			UserFunc2 preexplore,UserFunc2 postexplore,
			UserFunc1 postprocess);

		/* nop:
			Do nothing. Will be used in place of a NULL user function.
		*/
		static void nop();
};
#endif
