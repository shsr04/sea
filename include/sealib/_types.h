#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <functional>
#include <cstdint>

namespace Sealib {
// the standard numeric type (used mainly for node names)
typedef uint32_t uint;

typedef std::function<void(uint)> Consumer;
typedef std::function<void(uint, uint)> BiConsumer;

static constexpr uint INVALID = static_cast<uint>(-1);

template<class T>
class Result {
    public:
    constexpr Result(): ok(false), value() {}
    constexpr Result(T p): ok(true), value(p) {}

    constexpr operator T() const { return value; }
    constexpr bool ok() const { return ok; }

    private:
    bool ok;
    T value;
};
}  // namespace Sealib

/**
 * Macro for constexpr qualifier on member functions
 */
#ifdef __clang__
#define CONSTEXPR_IF_CLANG constexpr
#else
#define CONSTEXPR_IF_CLANG inline
#endif

#endif  // SEALIB__TYPES_H_
