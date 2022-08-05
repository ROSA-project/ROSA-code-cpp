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

uint64_t time_since_epoch_in_millisec();

} // namespace rosa