#include "QlfTornPend.h"
#include <QtDebug>
#include "QlfThread.h"
#include <QMutexLocker>

QlfTornPend::QlfTornPend (QIODevice* ioDevice)
  : ioDevice (ioDevice), mutex (), waitCondition (),
  terminated (false),
  tpThread (this, &QlfTornPend::runThread),
  readLineFlag (false)
{
  tpThread.start ();
}

QlfTornPend::~QlfTornPend ()
{
  terminated = true;
  tpThread.wait ();
  tpThread.terminate ();
}

void QlfTornPend::run ()
{
  ioDevice -> write ("RP\n",3);
}

void QlfTornPend::stop ()
{
  ioDevice -> write ("SP\n",3);
}

void QlfTornPend::runInTestMode ()
{
  ioDevice -> write ("PP\n",3);
}

void QlfTornPend::runADC ()
{
  ioDevice -> write ("RA\n",3);  
}

void QlfTornPend::stopADC ()
{
  ioDevice -> write ("SA\n",3);
}

void QlfTornPend::enableMagnet ()
{
  ioDevice -> write ("EM\n",3);
}

void QlfTornPend::disableMagnet ()
{
  ioDevice -> write ("DM\n",3);
}

QlfTornPend::State QlfTornPend::state ()
{
  switch (symbolState ())
  {
  case 'R': return Running;
  case 'S': return Stoped;
  case 'A': return InADCMode;
  case 'P': return InTestMode;
  case '\0': return Disconnected; 
  }  
  return Unknown;
}

char QlfTornPend::symbolState ()
{
  ioDevice -> write ("GS\n", 3);
  QByteArray buf = readLine ();
  if (buf.isEmpty ())
    return '\0';
  return buf[2];
}

int QlfTornPend::getResistorCode ()
{
  ioDevice -> write ("GR\n", 3);
  QByteArray buf = readLine ();
  buf.remove (0, 2);
  buf.chop (1);
  return buf.toInt ();
}

void QlfTornPend::setResistorCode (int code)
{
  QByteArray buf("SR");
  buf.append (QByteArray::number (code));
  buf.append ('\n');
  ioDevice -> write (buf);
}

void QlfTornPend::clearScreen ()
{
  ioDevice -> write ("clr\n", 4);  
}

void QlfTornPend::print (const char* message)
{
  QByteArray buf ("prt");
  buf.append (message);
  buf.append ('\n');
  ioDevice -> write (buf);
}

QString QlfTornPend::getID ()
{
  ioDevice -> write ("ID\n", 3);
  QByteArray buf = readLine ();
  buf.remove (0, 2);
  buf.chop (1);  
  return buf;
}

int QlfTornPend::getGainCode ()
{
  ioDevice -> write ("GG\n", 3);
  QByteArray buf = readLine ();
  buf.remove (0, 2);
  buf.chop (1);
  return buf.toInt (); 
}

QByteArray QlfTornPend::readLine ()
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
  return buf;
}

void QlfTornPend::onADCCode (QByteArray& data)
{
  data.remove (0, 2);
  data.chop (1);
  adcCodeReaded (data.toInt ()); 
}

void QlfTornPend::onPeriod (QByteArray& data)
{
  data.remove (0, 2);
  data.chop (1);
  QList<QByteArray> buf = data.split (',');
  int number = buf[0].toInt ();
  int maxAmpl = buf[1].toInt ();
  int minAmpl = buf[2].toInt ();
  int period = buf[3].toInt ();
  periodDataReaded (number, maxAmpl, minAmpl, period);
}

void QlfTornPend::runThread (TPThread* thread)
{
  while (!terminated)
  {
    if ( ! ioDevice -> isOpen ()) continue;
    
    QByteArray buf = ioDevice -> readLine (100);
        
    if (buf.isEmpty () && !readLineFlag) continue;
    
    if (buf.startsWith ("AR")) 
    {
      onADCCode (buf);
      continue;
    }
    if (buf.startsWith ("PR"))
    {
      onPeriod (buf);
      continue;
    }
    
    QMutexLocker locker(&mutex);
    buffer.push_back (buf);
    waitCondition.wakeOne ();
  }
}
