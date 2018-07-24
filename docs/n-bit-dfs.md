n-Bit Depth-First Search
===
The depth-first search over a graph G=(V,E) will *process* every node and *explore* every edge exactly once. The user can supply four *user-defined procedures*: `preprocess`, `preexplore`, `postexplore` and `postprocess`.

This space-efficient variant
- uses a compact bitset to store the *node colors*
- uses a segment stack which only keeps the two top-most segments and the last *trailer*.
    - When both segments are full and a push occurs, the lower segment is discarded.
    - When both segments are empty and a pop occurs, a restoration is started. The restoration is simply a *quiet* run (user procedures are disabled) of the DFS from the beginning. It runs until the current trailer matches with the trailer before the restoration.

## Efficiency
* Time: O((n+m) log n)
* Space: O((log(3)+ε) n) bits

##Usage
```cpp
Graph *g=new Graph(nodes,order);

DFS::nBitDFS(g,DFS_NOP_PROCESS,DFS_NOP_EXPLORE,DFS_NOP_EXPLORE,DFS_NOP_PROCESS);  // quiet run

DFS::nBitDFS(g,p0,e0,e1,p1);  // supply procedures to do something with the current node or edge
```