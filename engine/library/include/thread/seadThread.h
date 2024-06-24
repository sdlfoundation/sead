#ifndef SEAD_THREAD_H_
#define SEAD_THREAD_H_

#include <container/seadTList.h>
#include <heap/seadDisposer.h>
#include <heap/seadHeap.h>
#include <hostio/seadHostIODummy.h>
#include <prim/seadNamable.h>
#include <thread/seadMessageQueue.h>
#include <thread/seadThreadLocalStorage.h>
#include <time/seadTickSpan.h>

#ifdef cafe
#include "wut/os.h"
#endif // cafe

namespace sead {

class CoreIdMask;
class Thread;

typedef TList<Thread *> ThreadList;
typedef TListNode<Thread *> ThreadListNode;

class Thread : public IDisposer, public INamable {
public:
  virtual ~Thread();

  virtual void destroy() { waitDone(); }
  virtual bool sendMessage(MessageQueue::Element, MessageQueue::BlockType);
  virtual MessageQueue::Element recvMessage(MessageQueue::BlockType);
  virtual bool start();
  virtual void quit(bool);
  bool isDone() const;
  virtual void waitDone();
  virtual void quitAndDestroySingleThread(bool is_jam) {
    quitAndWaitDoneSingleThread(is_jam);
  }
  virtual void quitAndWaitDoneSingleThread(bool is_jam);
  static void sleep(TickSpan span);
  static void yield();
  u32 getID() const { return mID; }
  Heap *getCurrentHeap() const { return mCurrentHeap; }
  virtual void setPriority(s32);
  virtual s32 getPriority() const;
  virtual MessageQueue::BlockType getBlockType() const { return mBlockType; }
  virtual s32 getStackSize() const { return mStackSize; }
  virtual s32 calcStackUsedSizePeak() const;
  void checkStackOverFlow();
  const CoreIdMask &getAffinity() const;
  void setAffinity(const CoreIdMask &);

protected:
  ThreadListNode *getListNode();
  void setCurrentHeap_(Heap *);
  void run_();
  virtual void calc_(MessageQueue::Element) = 0;
  void initStackCheck_();
  virtual u32 *getStackCheckStartAddress_() const;
#ifdef cafe
  static void wut.hreadFunc_(void *);
#endif // cafe

public:
  void *getStackTop() const { return mStackTop; }
  void *getStackBottom() const {
    return PtrUtil::addOffset(mStackTop, mStackSize);
  }

  // Temp
  s32 getState() const { return mState[0]; }
  MessageQueue::Element getQuitMsg() const { return mQuitMsg; }

protected:
  MessageQueue mMessageQueue;
  s32 mStackSize;
  ThreadListNode mListNode;
  Heap *mCurrentHeap;
  MessageQueue::BlockType mBlockType;
  MessageQueue::Element mQuitMsg;
  u32 mID;
  s32 mState[1]; // State
  u32 mCoreIdMask;
#ifdef cafe
  OSThread *mThreadInner;
#endif // cafe
  u8 *mStackTop;
  s32 mPriority;
};
#ifdef cafe
static_assert(sizeof(Thread) == 0x90, "sead::Thread size mismatch");
#endif // cafe

class ThreadMgr : public hostio::Node {
  SEAD_SINGLETON_DISPOSER(ThreadMgr)

public:
  ThreadMgr();
  virtual ~ThreadMgr();

  void initialize(Heap *);
  void destroy();
  // void initHostIO();

  Thread *getCurrentThread() const;
  bool isMainThread() const;
  Thread *getMainThread() const { return mMainThread; }

  ThreadList::constIterator constBegin() const { return mList.constBegin(); }
  ThreadList::constIterator constEnd() const { return mList.constEnd(); }

  static void quitAndWaitDoneMultipleThread(Thread **, s32, bool);
  static void waitDoneMultipleThread(Thread *const *, s32);

protected:
  void initMainThread_(Heap *);
  void destroyMainThread_();

protected:
  ThreadList mList;
  Thread *mMainThread;
  ThreadLocalStorage mThreadPtrTLS;
};
#ifdef cafe
static_assert(sizeof(ThreadMgr) == 0x28, "sead::ThreadMgr size mismatch");
#endif // cafe

#ifdef cafe

inline Thread *ThreadMgr::getCurrentThread() const {
  return static_cast<Thread *>(OSGetThreadSpecific(mThreadPtrTLS.getValue()));
}

#endif // cafe

} // namespace sead

#endif // SEAD_THREAD_H_
