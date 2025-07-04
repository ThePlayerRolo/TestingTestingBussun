#pragma once

#include <revolution/types.h>

#if defined(PAL)
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_PAL.bin"
#elif defined(USA)
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_USA.bin"
#define HEAP_WATCHER_INSTANCE 0x806A245C
#elif defined(JPN)
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_JPN.bin"
#elif defined(TWN)
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_TWN.bin"
#elif defined(KOR)
#define KAMEK_BINARY_NAME "/CustomCode/CustomCode_KOR.bin"
#define HEAP_WATCHER_INSTANCE 0x806B6E2C
#endif

#define ALIGN_32(a) ((a + 31) & ~31)

namespace {
    struct KamekHeader {
        u32 magic1;
        u16 magic2;
        u16 version;
        u32 bssSize;
        u32 codeSize;
    };
    /// @brief Added in Kamek 2.0, we dont need any of this info as we already have it elsewhere.
    struct KamekExtra {
        u32 ctorStart;
        u32 ctorEnd;
        u32 unk[2];
    };

    void BussunInit();
    void BussunLink(u8 *linkedBuffer, u32 linkedSize, u8 *kamekBuffer, u32 kamekSize);

    void BussunError(const char *msg);
};


#define kAddr32 1
#define kAddr16Lo 4
#define kAddr16Hi 5
#define kAddr16Ha 6
#define kRel24 10
#define kWrite32 32
#define kWrite16 33
#define kWrite8 34
#define kCondWritePointer 35
#define kCondWrite32 36
#define kCondWrite16 37
#define kCondWrite8 38
#define kBranch 64
#define kBranchLink 65