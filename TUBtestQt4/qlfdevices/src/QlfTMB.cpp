#include "QlfTMB.h"
#include <QByteArray>
#include <QTextStream>

QlfTMB::QlfTMB (QIODevice* ioDevice)
  : ioDevice (ioDevice)
{
  
}

QlfTMB::TMBData QlfTMB::readData ()
{
  QByteArray buffer;
  while (true)
  {
    char data = -1;
    ioDevice -> getChar (&data);
    if (!data) break;
    buffer.push_back (data);
  };
  
  QTextStream str (buffer);
  str.seek (3);
  TMBData result;
  
  for(int i=0; i < 5; ++i)
  {
    QString temp;
    str >> temp;
    result.push_back (temp.toDouble());
  }
  return result;
}

QlfTMB::TMBData QlfTMB::readTemperatures ()
{
  ioDevice -> write ("GT", 3);
  return readData ();
}

QlfTMB::TMBData QlfTMB::readCodes ()
{
  ioDevice -> write ("GC", 3);
  return readData ();
}
