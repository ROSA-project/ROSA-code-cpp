#pragma once

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

} // namespace rosa