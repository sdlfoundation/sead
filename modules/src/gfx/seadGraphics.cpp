#include <gfx/seadGraphics.h>
#include <thread/seadThread.h>

namespace sead {

Graphics* Graphics::sInstance = NULL;

void Graphics::lockDrawContext()
{
    sead::Thread* current = ThreadMgr::instance()->getCurrentThread();
    if (current == mContextHolderThread && mContextRefCounter > 0)
    {
        mContextRefCounter++;
    }
    else
    {
        mContextCriticalSection.lock();
        // SEAD_ASSERT(mContextHolderThread == nullptr);
        // SEAD_ASSERT(mContextRefCounter == 0);
        lockDrawContextImpl();
        mContextHolderThread = current;
        mContextRefCounter = 1;
    }
}

void Graphics::unlockDrawContext()
{
    sead::Thread* current = ThreadMgr::instance()->getCurrentThread();
    // SEAD_ASSERT(mContextHolderThread == current);
    // SEAD_ASSERT(mContextRefCounter > 0);
    if (--mContextRefCounter == 0)
    {
        mContextHolderThread = NULL;
        unlockDrawContextImpl();
        mContextCriticalSection.unlock();
    }
}

} // namespace sead
