#include "./naivetrailstructure.h"

uint Sealib::NaiveTrailStructure::leave() {
    if (unusedEdges.empty()) {
        return (uint) -1;
    }
    uint beg = unusedEdges.front();
    unusedEdges.pop_front();

    if (unusedEdges.empty()) {
        black = true;
    }
    grey = true;

    pairedEdges[beg] = (uint) -1;
    currentDegree--;
    return beg;
}

uint Sealib::NaiveTrailStructure::enter(uint idx) {
    if (unusedEdges.empty()) {
        return (uint) -1;
    }
    size_t size = unusedEdges.size();
    unusedEdges.remove(idx);
    if (size == unusedEdges.size()) {
        return (uint) -1;  // couldn't remove that idx
    }
    currentDegree--;

    if (unusedEdges.empty()) {
        black = true;
        pairedEdges[idx] = (uint) -1;
        return (uint) -1;
    }
    grey = true;

    uint next = leave();
    if (next != (uint) -1) {
        pairedEdges[next] = idx;
        pairedEdges[idx] = next;
    }
    return next;
}

bool Sealib::NaiveTrailStructure::isBlack() {
    return black;
}

bool Sealib::NaiveTrailStructure::isWhite() {
    return !isGrey();
}

bool Sealib::NaiveTrailStructure::isGrey() {
    return grey;
}

bool Sealib::NaiveTrailStructure::isEven() {
    return currentDegree % 2 == 0;
}

Sealib::NaiveTrailStructure::NaiveTrailStructure(uint degree) :
    currentDegree(degree),
    black(degree == 0),
    grey(false) {
    for (uint i = 0; i < degree; i++) {
        unusedEdges.push_back(i);
    }
}

uint Sealib::NaiveTrailStructure::getMatched(uint i) {
    if (pairedEdges.count(i)) {
        return pairedEdges[i];
    } else {
        return (uint) -1;
    }
}
