#ifndef SEAD_THREAD_UTIL_H_
#define SEAD_THREAD_UTIL_H_

#include <basis/seadTypes.h>

namespace sead {

class ThreadUtil
{
public:
    static s32 ConvertPrioritySeadToPlatform(s32);
    static s32 ConvertPriorityPlatformToSead(s32);
};

} // namespace sead

#endif // SEAD_THREAD_UTIL_H_
