#ifndef QLFSPDEVICE_H_
#define QLFSPDEVICE_H_

#include <QObject>
#include <QIODevice> 
#include "QlfThread.h"
#include <QList>
#include <QMutex>
#include <QWaitCondition>

class QlfSPDevice: public QObject
{
  Q_OBJECT
public:
  QlfSPDevice (QIODevice* ioDevice);
  ~QlfSPDevice ();

protected:
  qint64 write (const QByteArray& byteArray);
  QByteArray readLine ();
  /**
   * Фильтр данных
   * \return false - данные будут недоступны readLine ()
   */
  virtual bool lineFilter (QByteArray& byteArray);
private:
  typedef QlfThread<QlfSPDevice> Thread;
  void runThread (Thread* thread);
private:
  QIODevice* ioDevice;
  QList<QByteArray> buffer;
  Thread thread;
  QMutex mutex;
  QWaitCondition waitCondition;
  bool terminated;
  bool readLineFlag;
};
  
#endif /*QLFSPDEVICE_H_*/
