#include "QlfIOBlender.h"

QlfIOBlender::QlfIOBlender (QIODevice* inDevice, QIODevice* outDevice)
  : QIODevice (), inDevice (inDevice), outDevice (outDevice) 
{
  
}

qint64 QlfIOBlender::readData (char* data, qint64 maxSize)
{
  if (inDevice)
    return inDevice -> read (data, maxSize);
  
  for(qint64 i = 0; i < maxSize; i++)
    data[i] = 0;
  return maxSize;
}

qint64 QlfIOBlender::writeData (const char* data, qint64 maxSize)
{
  if (outDevice)
    return outDevice -> write (data, maxSize);
  
  return maxSize;
}
