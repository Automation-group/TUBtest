#include "QlfSPDevice.h"

QlfSPDevice::QlfSPDevice (QIODevice* ioDevice)
  : ioDevice (ioDevice), mutex (), waitCondition (),
  terminated (false),
  thread (this, &QlfSPDevice::runThread),
  readLineFlag (false)
{
  thread.start ();
}

QlfSPDevice::~QlfSPDevice ()
{
  terminated = true;
  thread.wait ();
  thread.terminate ();
}

qint64 QlfSPDevice::write (const QByteArray& byteArray)
{
  return ioDevice -> write (byteArray);
}

QByteArray QlfSPDevice::readLine ()
{
  QMutexLocker locker(&mutex);
  if (buffer.isEmpty ())
  {
    readLineFlag = true;
    if (! waitCondition.wait (&mutex) )
    {
      readLineFlag = false;
      return QByteArray ();
    }
  }
  
  QByteArray buf = buffer.front ();
  buffer.pop_front ();
  readLineFlag = false;
  return buf;
}

bool QlfSPDevice::lineFilter (QByteArray& byteArray)
{
  return true;
}

void QlfSPDevice::runThread (Thread* thread)
{
  while (!terminated)
  {
    if ( ! ioDevice -> isOpen ()) continue;
   
    QByteArray buf = ioDevice -> readLine (100);
    
    if (buf.isEmpty () && !readLineFlag) continue;
    
    if (lineFilter (buf))
    {
      QMutexLocker locker(&mutex);    
      buffer.push_back (buf);
      waitCondition.wakeOne ();
    }
  }
}
