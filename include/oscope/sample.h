#pragma once

#ifndef _OSCOPE_SAMPLE_H_
#define _OSCOPE_SAMPLE_H_

#include <oscope/common.h>

#include <stdint.h>

NAMESPACE_BEGIN(oscope)

#ifdef __cplusplus
extern "C" {
#endif

class Sample {
    
public:
    Sample(long timestamp, uint64_t count, int16_t *samples);
    
    long timestamp() { return _timestamp; }
    uint64_t count() { return _count; }
    int16_t *samples() { return _samples; }
    
private:
    long _timestamp;
    uint64_t _count;
    int16_t *_samples;
};
    
#ifdef __cplusplus
} // extern "C"
#endif


NAMESPACE_END(oscope)

#endif // _OSCOPE_SAMPLE_H_