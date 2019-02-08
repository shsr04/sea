#ifndef SEALIB__TYPES_H_
#define SEALIB__TYPES_H_
#include <cstdint>
#include <functional>

namespace Sealib {
// the standard numeric type (used mainly for node names)
typedef uint32_t uint;

typedef std::function<void(uint)> Consumer;
typedef std::function<void(uint, uint)> BiConsumer;

static constexpr uint INVALID = static_cast<uint>(-1);

/**
 * Result class that is used instead of exceptions.
 * If a method returns a Result<T>, you should first check if the method was
 * successful with result.ok().
 * Then, you can simply access the object without any type casting.
 * Example:
 * Result<std::string> r = readSomething();
 * if(char.ok()) std::cout << r << "\n";
 *
 * @param T the type of the contained value
 */
template <class T>
class Result {
 public:
    /**
     * Creates an empty Result to indicate failure.
     */
    constexpr Result() : status(false), value() {}

    /**
     * Creates a successful Result that contains a value.
     * @param p value to be stored
     */
    constexpr Result(T p) : status(true), value(p) {}

    /**
     * Checks if this object holds a value (i.e. if an operation was
     * successful).
     * @return true on success, false otherwise
     */
    constexpr bool ok() const { return status; }

    /**
     * Implicit conversion of type Result<T> to the contained type T.
     * @return the contained value of type T
     */
    constexpr operator T() const { return value; }
    /**
     * An explicit conversion to the contained type, useful if the implicit
     * conversion is not working.
     * Example:
     * Result<std::tuple<uint32_t,uint32_t>> r = computeTuple();
     * std::get<0>(r);    // not working
     * std::get<0>(r());  // ok, explicitly converted
     *
     * @return the contained value of type T
     */
    constexpr T operator()() const { return value; }

 private:
    bool status;
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
