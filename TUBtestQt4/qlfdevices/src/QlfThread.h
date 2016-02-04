#ifndef QLFTHREAD_H_
#define QLFTHREAD_H_

#include <QThread>

template <class T>
class QlfThread : public QThread
{
public:
  typedef void (T::*threadRunProc)(QlfThread* thread);
  QlfThread (T* owner, threadRunProc runProc)
    : owner (owner), runProc (runProc), QThread ()
  {

  }


  void sleep(unsigned long secs)
  {
	  QThread::sleep(secs);
  }

protected:
  void run () { (owner->*runProc) (this); }
private:
  const threadRunProc runProc;
  T* owner;
};

#endif /*QLFTHREAD_H_*/
