#include <controller/seadControllerWrapper.h>
#include <prim/seadMemUtil.h>

namespace sead {

const u8 ControllerWrapper::cPadConfigDefault[Controller::cPadIdx_Max] = {
    0,
    1,
    2,
    3,
    4,
    5,
    6,
    7,
    8,
    9,
    10,
    11,
    12,
    13,
    14,
    Controller::cPadIdx_Touch,
    Controller::cPadIdx_Up,
    Controller::cPadIdx_Down,
    Controller::cPadIdx_Left,
    Controller::cPadIdx_Right,
    Controller::cPadIdx_LeftStickUp,
    Controller::cPadIdx_LeftStickDown,
    Controller::cPadIdx_LeftStickLeft,
    Controller::cPadIdx_LeftStickRight,
    Controller::cPadIdx_RightStickUp,
    Controller::cPadIdx_RightStickDown,
    Controller::cPadIdx_RightStickLeft,
    Controller::cPadIdx_RightStickRight
};

ControllerWrapper::ControllerWrapper()
{
    MemUtil::copy(mPadConfig, cPadConfigDefault, Controller::cPadIdx_Max);
}

void ControllerWrapper::calc(u32 prev_hold, bool prev_pointer_on)
{
    if (mIsEnable && mController && mController->isConnected())
    {
        mPadHold = BitFlag32(createPadMaskFromControllerPadMask_(mController->getHoldMask()));

        mLeftStick.set(mController->getLeftStick());
        mRightStick.set(mController->getRightStick());
        mLeftAnalogTrigger = mController->getLeftAnalogTrigger();
        mRightAnalogTrigger = mController->getRightAnalogTrigger();

        bool pointer_on = mController->isPointerOn();

        bool touchkey_hold = false;
        if (mTouchKeyBit > 0)
            touchkey_hold = mPadHold.isOnBit(mTouchKeyBit);

        setPointerWithBound_(pointer_on, touchkey_hold, mController->getPointer());
        updateDerivativeParams_(createPadMaskFromControllerPadMask_(prev_hold), prev_pointer_on);
    }
    else
    {
        setIdle();
    }

    if (isIdle_())
        mIdleFrame++;
    else
        mIdleFrame = 0;
}

void ControllerWrapper::setPadConfig(s32 padbit_max, const u8* pad_config, bool enable_stickcross_emulation)
{
    MemUtil::copy(mPadConfig, pad_config, padbit_max);

    mLeftStickCrossStartBit = -1;
    mRightStickCrossStartBit = -1;

    if (enable_stickcross_emulation)
    {
        for (s32 i = 0; i < padbit_max; i++)
        {
            if (pad_config[i] == Controller::cPadIdx_LeftStickUp)
                mLeftStickCrossStartBit = i;

            else if (pad_config[i] == Controller::cPadIdx_RightStickUp)
                mRightStickCrossStartBit = i;
        }
    }

    mTouchKeyBit = -1;

    for (s32 i = 0; i < padbit_max; i++)
    {
        if (pad_config[i] == Controller::cPadIdx_Touch)
        {
            mTouchKeyBit = i;
            break;
        }
    }
}

u32 ControllerWrapper::createPadMaskFromControllerPadMask_(u32 controller_mask) const
{
    BitFlag32 controller_pad_mask(controller_mask);
    BitFlag32 pad_mask;

    for (u32 i = 0; i < mPadBitMax; i++)
    {
        if (controller_pad_mask.isOnBit(mPadConfig[i]))
            pad_mask.setBit(i);
    }

    return pad_mask.getDirect();
}

} // namespace sead