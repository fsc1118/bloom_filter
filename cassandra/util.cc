#include <memory>
#include <stdint.h>
#include "util.h"
#include <limits>
#include <algorithm>
#include <vector>

long abstract_val(long index) {
    long negbit = index >> 63;
    return (index ^ negbit) - negbit;
}