#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#ifndef __cplusplus
#include <stdint.h>
typedef uint32_t uint;
#else
#include <functional>
#include <cstdint>

namespace Sealib {
/**
 * The standard numeric type. Used for anything that is indexed, read or written in this library.
 * You can set `uint` to any integer type you want (e.g. for very small or large graphs)
 */
typedef uint32_t uint;

typedef std::function<void(uint)> Consumer;
typedef std::function<void(uint, uint)> BiConsumer;

static constexpr uint INVALID = static_cast<uint>(-1);
}  // namespace Sealib

/**
 * Macro for constexpr qualifier on member functions
 */
#ifdef __clang__
#define CONSTEXPR_IF_CLANG constexpr
#else
#define CONSTEXPR_IF_CLANG inline
#endif
#endif
#endif  // SEALIB__TYPES_H_
