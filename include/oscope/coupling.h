#pragma once

#ifndef _OSCOPE_COUPLING_H_
#define _OSCOPE_COUPLING_H_

#include <oscope/common.h>

NAMESPACE_BEGIN (oscope)

enum class Coupling : std::uint8_t {
    AC = 0x0,
    DC = 0x1
};

NAMESPACE_END (oscope)

#endif // _OSCOPE_COUPLING_H_