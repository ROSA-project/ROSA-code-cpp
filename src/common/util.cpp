#include "util.hpp"
#include <chrono>

namespace rosa {

uint64_t time_since_epoch_in_millisec() {
    namespace chr = std::chrono;
    return chr::duration_cast<chr::milliseconds>(
               chr::system_clock::now().time_since_epoch())
        .count();
}

} // namespace rosa