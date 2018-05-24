Library for Space-Efficient (Graph) Algorithms [![Build Status](https://travis-ci.org/thm-mni-ii/sea.svg?branch=master)](https://travis-ci.org/thm-mni-ii/sea) [![License: GPL v3](https://img.shields.io/badge/License-GPL%20v3-blue.svg)](https://www.gnu.org/licenses/gpl-3.0) [![Coverage Status](https://coveralls.io/repos/github/thm-mni-ii/sea/badge.svg?branch=master)](https://coveralls.io/github/thm-mni-ii/sea?branch=master)
===
Because of the rapid growth of the data sizes in nowadays applications, algorithms
that are designed to efficiently utilize space are becoming more and more
important. Another reason for the need for such algorithms is the limitation in
the memory sizes of tiniest devices.

Therefore, this library provides algorithms that treat space as a scarce resource.

# Library features
- [x] Graph data type. Requires $O((n + m) \log n)$ bits
- [ ] Hierholzer's algorithm
- [ ] Choice Dictionary
- [ ] Rank Select
- [ ] Linear time in-place BSF and DSF in the restore model [Link to PDF](https://arxiv.org/pdf/1803.04282.pdf)

# Building the Library from Source
1. Install CMake and a c++ compiler for your specific system.
2. Build a make file for your system by using CMake -> `cmake .`
3. Build the artifacts by executing make -> `make`

# Project Structure
```bash
.
├── CMakeLists.txt  # CMake build script
├── LICENSE         # Licence descriptionö
├── README.md       # You are reading this file now
├── third-party     # Third party libraries
├── include         # The libraries header files (*.h)
├── src             # The libraries source files (*.cpp)
├── src-view        # The source files for the visualization (*.cpp)
├── test            # The test files
├── lib             # The library files
└── bin             # Executable files to test the project
```

# License
Licensed under the GNU General Public License version 3. For detailed license information look inside the [LICENSE](LICENSE) file.

# Acknowledgments
We thank the [DFG](http://www.dfg.de/en/index.jsp) (Deutsche Forschungsgemeinschaft) for funding this project. For more details visit [http://gepris.dfg.de/gepris/projekt/379157101](http://gepris.dfg.de/gepris/projekt/379157101).