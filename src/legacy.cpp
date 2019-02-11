#include "sealib/legacy.h"
#include <vector>
#include "sealib/collection/bitset.h"
#include "sealib/dictionary/rankselect.h"
#include "sealib/graph/graphcreator.h"
#include "sealib/iterator/choicedictionaryiterator.h"
#include "sealib/iterator/dfs.h"

namespace Sealib {

void *Sealib_Graph_new(uint **m, uint order) {
    return GraphCreator::createPointerFromAdjacencyMatrix(m, order);
}
void Sealib_Graph_delete(void *self) {
    delete static_cast<UndirectedGraph const *>(self);
}
void *Sealib_Graph_generateRandom(uint order) {
    std::vector<SimpleNode> n(order);
    static std::random_device rng;
    std::uniform_int_distribution<uint> rnd(0, order - 1);
    for (uint a = 0; a < order; a++) {
        uint deg = rnd(rng);
        std::vector<uint> ad;
        for (uint b = 0; b < deg; b++) {
            ad.emplace_back(rnd(rng));
        }
        n[a] = SimpleNode(ad);
    }
    return new DirectedGraph(n);
}

void *Sealib_ChoiceDictionary_new(uint size) {
    return new ChoiceDictionary(size);
}
void Sealib_ChoiceDictionary_delete(void *self) {
    delete static_cast<ChoiceDictionary *>(self);
}
void Sealib_ChoiceDictionary_set(void *self, uint index) {
    static_cast<ChoiceDictionary *>(self)->insert(index);
}
int Sealib_ChoiceDictionary_get(void *self, uint index) {
    return static_cast<ChoiceDictionary *>(self)->get(index);
}
uint Sealib_ChoiceDictionary_choice(void *self) {
    return static_cast<ChoiceDictionary *>(self)->choice();
}

void *Sealib_ChoiceDictionaryIterator_new(void *choiceDictionary) {
    return new ChoiceDictionaryIterator(
        static_cast<ChoiceDictionary *>(choiceDictionary));
}
void Sealib_ChoiceDictionaryIterator_delete(void *self) {
    delete static_cast<ChoiceDictionaryIterator *>(self);
}
void Sealib_ChoiceDictionaryIterator_init(void *self) {
    static_cast<ChoiceDictionaryIterator *>(self)->init();
}
int Sealib_ChoiceDictionaryIterator_more(void *self) {
    return static_cast<ChoiceDictionaryIterator *>(self)->more();
}
uint Sealib_ChoiceDictionaryIterator_next(void *self) {
    return static_cast<ChoiceDictionaryIterator *>(self)->next();
}

void *Sealib_Bitset_new(uint size) { return new Bitset<uint8_t>(size); }
void Sealib_Bitset_delete(void *self) {
    delete static_cast<Bitset<uint8_t> *>(self);
}
void Sealib_Bitset_set(void *self, uint index) {
    Bitset<uint8_t> &b = *static_cast<Bitset<uint8_t> *>(self);
    b[index] = 1;
}
int Sealib_Bitset_get(void *self, uint index) {
    return static_cast<Bitset<uint8_t> *>(self)->get(index);
}

void *Sealib_RankSelect_new(void *bitset) {
    return new RankSelect(*static_cast<Bitset<uint8_t> *>(bitset));
}
void Sealib_RankSelect_delete(void *self) {
    delete static_cast<RankSelect *>(self);
}
uint Sealib_RankSelect_rank(void *self, uint index) {
    return static_cast<RankSelect *>(self)->rank(index);
}
uint Sealib_RankSelect_select(void *self, uint bit) {
    return static_cast<RankSelect *>(self)->select(bit);
}
uint Sealib_RankSelect_size(void *self) {
    return static_cast<RankSelect *>(self)->size();
}

void Sealib_DFS_nloglognBitDFS(void *graph, void (*preprocess)(uint),
                               void (*preexplore)(uint, uint),
                               void (*postexplore)(uint, uint),
                               void (*postprocess)(uint)) {
    if (preprocess == nullptr) preprocess = [](uint) {};
    if (preexplore == nullptr) preexplore = [](uint, uint) {};
    if (postexplore == nullptr) postexplore = [](uint, uint) {};
    if (postprocess == nullptr) postprocess = [](uint) {};
    DFS::nloglognBitDFS(static_cast<Graph const *>(graph), preprocess,
                        preexplore, postexplore, postprocess);
}
}  // namespace Sealib
