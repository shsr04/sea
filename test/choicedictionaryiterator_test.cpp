#include <gtest/gtest.h>
#include <sealib/iterator/choicedictionaryiterator.h>

#include <algorithm>
#include <array>
#include <random>

using Sealib::ChoiceDictionaryIterator;

TEST(ChoiceDictionaryIteratorTest, iterator_integrity) {
    const uint size = 25000;
    const uint setSize = 5000;

    Sealib::ChoiceDictionary *c = new Sealib::ChoiceDictionary(size);
    ChoiceDictionaryIterator *iterator = new ChoiceDictionaryIterator(c);

    std::array<uint, setSize> set;

    uint count = 0;
    uint num = 0;
    uint seed = 0;

    for (uint i = 0; i < setSize; i++) {
        set[i] = num;
        num += 5;
    }

    std::shuffle(set.begin(), set.end(), std::default_random_engine(seed));

    for (uint number : set) {
        c->insert(number);
    }
    for (uint i = 0; i < 64; i+=5) {
        c->remove(i);
    }
    c->remove(12000);
    c->remove(12005);
    c->remove(15000);
    c->remove(16000);
    c->remove(16500);

    iterator->init();
    while (iterator->more()) {
        uint index = iterator->next();
        for (uint number : set) {
            if (index == number) {
                count++;
                break;
            }
        }
    }
    // ASSERT_EQ(count, setSize - 18);
    delete iterator;
    delete c;
}
