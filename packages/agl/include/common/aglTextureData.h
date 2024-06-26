#pragma once

#include <common/aglTextureEnum.h>
#include <math/seadMathCalcCommon.h>

#include "wut/gx2.h>

namespace agl {

class TextureData
{
public:
    TextureData();
    virtual ~TextureData() { }

    bool operator==(const TextureData& rhs) const;
    bool operator!=(const TextureData& rhs) const { return !(*this == rhs); }

    TextureFormat getTextureFormat() const { return mFormat; }
    TextureType getTextureType() const { return TextureType(mSurface.dim); }

    u32 getWidth(s32 mip_level = 0) const { return sead::Mathi::max(mSurface.width >> mip_level, mMinWidth); }
    u32 getHeight(s32 mip_level = 0) const { return sead::Mathi::max(mSurface.height >> mip_level, mMinHeight); }

    MultiSampleType getMultiSampleType() const { return MultiSampleType(mSurface.aa); }

    u32 getSliceNum(s32 mip_level = 0) const { return sead::Mathi::max(mSurface.depth >> mip_level, mMinSlice); }

    void* getImagePtr() const { return mSurface.imagePtr; }
    void setImagePtr(void* ptr) { mSurface.imagePtr = ptr; }

    u32 getImageByteSize() const { return mSurface.imageSize; }

    void* getMipPtr() const { return mSurface.mipPtr; }
    void setMipPtr(void* ptr) { mSurface.mipPtr = ptr; }

    u32 getMipByteSize() const { return mSurface.mipSize; }

    u32 getAlignment() const { return mSurface.alignment; }

    const GX2Surface& getSurface() const { return mSurface; }

    TextureCompSel getComponentR() const { return mCompR; }
    TextureCompSel getComponentG() const { return mCompG; }
    TextureCompSel getComponentB() const { return mCompB; }
    TextureCompSel getComponentA() const { return mCompA; }

    void setTextureCompSel(TextureCompSel r, TextureCompSel g, TextureCompSel b, TextureCompSel a)
    {
        mCompR = r;
        mCompG = g;
        mCompB = b;
        mCompA = a;
    }

    void initialize(TextureFormat format, u32 width, u32 height, u32 mip_level_num)
    {
        initialize_(cTextureType_2D, format, width, height, 1, mip_level_num, cMultiSampleType_1x);
    }

    void initialize3D(TextureFormat format, u32 width, u32 height, u32 slice_num, u32 mip_level_num)
    {
        initialize_(cTextureType_3D, format, width, height, slice_num, mip_level_num, cMultiSampleType_1x);
    }

    void initializeCubeMap(TextureFormat format, u32 width, u32 height, u32 mip_level_num)
    {
        initialize_(cTextureType_CubeMap, format, width, height, 6, mip_level_num, cMultiSampleType_1x);
    }

    void initializeCubeMapArray(TextureFormat format, u32 width, u32 height, u32 array_num, u32 mip_level_num)
    {
        initialize_(cTextureType_CubeMap, format, width, height, array_num * 6, mip_level_num, cMultiSampleType_1x);
    }

    void initializeArray(TextureFormat format, u32 width, u32 height, u32 array_num, u32 mip_level_num)
    {
        initialize_(cTextureType_2D_Array, format, width, height, array_num, mip_level_num, cMultiSampleType_1x);
    }

    void initializeMultiSample(TextureFormat format, u32 width, u32 height, MultiSampleType multi_sample_type)
    {
        initialize_(cTextureType_2D_MSAA, format, width, height, 1, 1, multi_sample_type);
    }

    u32 getMipLevelNum() const { return mSurface.numMips; }
    void setMipLevelNum(u32 mip_level_num);

    void setSurfaceSwizzle(u32 swizzle) { GX2SetSurfaceSwizzle(&mSurface, swizzle); }

    void invalidateGPUCache() const;

private:
    void copyTo_(const TextureData* dst, s32 dst_slice, s32 dst_mip_level, s32 src_slice, s32 src_mip_level, bool restore_state) const;

public:
    void copyTo(const TextureData* dst, s32 slice, s32 mip_level) const
    {
        copyTo_(dst, slice, mip_level, slice, mip_level, false);
    }

    void copyTo(const TextureData* dst, s32 dst_slice, s32 dst_mip_level, s32 src_slice, s32 src_mip_level) const
    {
        copyTo_(dst, dst_slice, dst_mip_level, src_slice, src_mip_level, false);
    }

    void copyToAll(const TextureData* dst) const;

private:
    void initialize_(TextureType type, TextureFormat format, u32 width, u32 height, u32 slice_num, u32 mip_level_num, MultiSampleType multi_sample_type);
    void initializeSize_(u32 width, u32 height, u32 slice_num);

public:
    void initializeFromSurface(const GX2Surface& surface);

private:
    TextureFormat mFormat;
    mutable GX2Surface mSurface;
    u32 mMinWidth;
    u32 mMinHeight;
    u32 mMinSlice;
    u32 mMaxMipLevel;
    TextureCompSel mCompR;
    TextureCompSel mCompG;
    TextureCompSel mCompB;
    TextureCompSel mCompA;
};
static_assert(sizeof(TextureData) == 0x9C, "agl::TextureData size mismatch");

inline bool
TextureData::operator==(const TextureData& rhs) const
{
    return (
        mSurface.dim        == rhs.mSurface.dim         &&
        mSurface.width      == rhs.mSurface.width       &&
        mSurface.height     == rhs.mSurface.height      &&
        mSurface.depth      == rhs.mSurface.depth       &&
        mSurface.numMips    == rhs.mSurface.numMips     &&
        mSurface.format     == rhs.mSurface.format      &&
        mSurface.swizzle    == rhs.mSurface.swizzle     &&
        mSurface.tileMode   == rhs.mSurface.tileMode    &&
        mSurface.aa         == rhs.mSurface.aa
    );
}

}
