#ifndef SRC_SUBGRAPH_SUBGRAPH_H_
#define SRC_SUBGRAPH_SUBGRAPH_H_
#include <sealib/collection/bitset.h>
#include <sealib/dictionary/rankselect.h>
#include <sealib/collection/subgraphstack.h>
#include <string>
#include <tuple>

namespace Sealib {
class SubGraph {
    friend class BaseSubGraph;
    friend class RecursiveSubGraph;
 public:
    typedef Sealib::Bitset<uint8_t> bitset_t;
    typedef Sealib::RankSelect rankselect_t;
    typedef Sealib::SubGraphStack stack_t;

 protected:
    uint sidx;
    uint ridx;
    stack_t *stack;
    rankselect_t *qSelect;
    rankselect_t *pSelect;

    SubGraph(uint sidx_, uint ridx_, stack_t *stack_)
        : sidx(sidx_),
          ridx(ridx_),
          stack(stack_),
          qSelect(nullptr),
          pSelect(nullptr) {}

    inline uint select_q(uint i) const {
        return qSelect->select(i);
    }

    inline uint select_p(uint i) const {
        return pSelect->select(i);
    }

    inline uint rank_q(uint i) const {
        return qSelect->rank(i);
    }

    inline uint rank_p(uint i) const {
        return pSelect->rank(i);
    }

 public:
    inline uint getSidx() const {
        return sidx;
    }

    inline uint getRidx() const {
        return ridx;
    }

    uint degree(uint u) const {
        if (u == 0) {
            throw std::invalid_argument("u needs to be > 0");
        }
        uint a = select_p(rank_q(u));  //  pSelect.select(qSelect.rank(v));
        uint b = select_p(rank_q(u - 1));  // pSelect.select(qSelect.rank(v - 1));

        if (a == b) {
            return 0;
        } else if (b == (uint) -1) {
            return a;
        } else {
            return a - b;
        }
    }

    virtual uint head(uint u, uint k) const = 0;

    inline uint head(std::tuple<uint, uint> uk) const {
        return head(std::get<0>(uk), std::get<1>(uk));
    }

    virtual std::tuple<uint, uint>
    mate(uint u, uint k) const = 0;

    inline std::tuple<uint, uint>
    mate(std::tuple<uint, uint> uk) const {
        return mate(std::get<0>(uk), std::get<1>(uk));
    }

    inline uint order() const {
        return qSelect->size();
    }

    uint g(uint j, uint k) const {
        if (j == 0 || k == 0) {
            throw std::invalid_argument(
                "j and k need to be > 0! (j,k)=(" +
                    std::to_string(j) + "," + std::to_string(k) + ")");
        }

        uint deg = degree(j);
        if (deg == 0 || k > deg) {
            throw std::out_of_range("node j has a degree < k! (j,k)=(" +
                std::to_string(j) + "," + std::to_string(k) + ")");
        }

        uint qRank = rank_q(j);  // qSelect.rank(j);
        uint n = 0;
        if (qRank > 1) {
            n = select_p(qRank - 1);  // pSelect.select(qRank - 1) - 1;
        }
        uint arc = n + k;
        return arc;  // pSelect.rank(n + 1) != pSelect.rank(arc)
    }

    inline uint gMax() {
        return pSelect->size();
    }

    inline uint g(std::tuple<uint, uint> jk) const {
        return g(std::get<0>(jk), std::get<1>(jk));
    }

    std::tuple<uint, uint> gInv(uint r) const {
        if (r == 0) {
            throw std::invalid_argument("r needs to be > 0 (r = "
                                            + std::to_string(r) + ")");
        }
        uint j = r == 1 ? 0 : rank_p(r - 1);  // pSelect.rank(r - 1);
        if (j == (uint) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = "
                                        + std::to_string(r) + ")");
        }
        j++;
        uint a = select_q(j);  // qSelect.select(j);
        if (a == (uint) -1) {
            throw std::out_of_range("out of range - no arc r exists! (r = "
                                        + std::to_string(r) + ")");
        }
        uint b = select_p(j - 1);  // pSelect.select(j - 1);
        b = b == (uint) -1 ? 0 : b;
        return std::tuple<uint, uint>(a, r - b);
    }

    virtual uint phi(uint u) const = 0;

    virtual uint psi(uint a) const = 0;

    virtual uint phiInv(uint u) const = 0;

    virtual uint psiInv(uint a) const = 0;
    virtual ~SubGraph() {
        delete pSelect;
        delete qSelect;
    }
    SubGraph() = delete;
};
}  // namespace Sealib
#endif  // SRC_SUBGRAPH_SUBGRAPH_H_
