#pragma once

#include <cstdint>
#include <iostream>

namespace rosa {

#ifdef NDEBUG
#define rosa_assert(assertion, errorMsg) \
    do {                                 \
    } while (false)
#else
#define rosa_assert(assertion, errorMsg)                                            \
    do {                                                                            \
        if (!(assertion)) {                                                         \
            std::cerr << "[Failed Assert] " << errorMsg << " (" << __FILE__ << ": " \
                      << __LINE__ << ")" << std::endl;                              \
            std::exit(EXIT_FAILURE);                                                \
        }                                                                           \
    } while (false)
#endif

#define MUST_EQ(a, b)                                                \
    do {                                                             \
        if ((a) != (b)) {                                            \
            std::cerr << "[FAIL] #a != #b" << std::endl;             \
            std::cerr << "       left side  = " << (a) << std::endl; \
            std::cerr << "       right side = " << (b) << std::endl; \
        }                                                            \
    } while (false)

#define MUST_TRUE(a)                                           \
    do {                                                       \
        if (!(a)) {                                            \
            std::cerr << "[FAIL] #a is not TRUE" << std::endl; \
        }                                                      \
    } while (false)

uint64_t time_since_epoch_in_millisec();

} // namespace rosa