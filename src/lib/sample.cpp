#include "sample.h"

NAMESPACE_BEGIN(oscope)

Sample::Sample(long timestamp, uint64_t count, int16_t *samples) : _timestamp(timestamp), _count(count), _samples(samples) {
    
}


NAMESPACE_END(ocope)