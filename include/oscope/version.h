#ifndef _OSCOPE_VERSION_H_
#define _OSCOPE_VERSION_H_

#include <oscope/common.h>

#include <string>

NAMESPACE_BEGIN(oscope)

#ifdef __cplusplus
extern "C" {
#endif

using namespace std;

struct Version {
    uint8_t major;
    uint8_t minor;
    string git_revision;
    
public:
    static Version instance();
};
    
#ifdef __cplusplus
} // extern "C"
#endif

NAMESPACE_END(oscope)

#endif // _OSCOPE_VERSION_H_
