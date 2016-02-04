#include "QlfIOCompressor.h"
#include <zlib.h>

QlfIOCompressor::QlfIOCompressor (QIODevice* device, int compressionLevel, int bufferSize)
  : QIODevice (), device (device), streamFormat (ZlibFormat),
  compressionLevel (compressionLevel), bufferSize (bufferSize)
{
  inBuffer.reserve (bufferSize);
  //outBuffer;  
}

QlfIOCompressor::~QlfIOCompressor ()
{
  
}

bool QlfIOCompressor::open (QIODevice::OpenMode mode)
{
  QIODevice::open (mode);
}

void QlfIOCompressor::close ()
{
  flush ();
  QIODevice::close ();
}

void QlfIOCompressor::flush ()
{
  if (!isWritable ()) return;
  device -> write (qCompress (inBuffer, compressionLevel));
  inBuffer.clear ();
  inBuffer.reserve (bufferSize);
}

qint64 QlfIOCompressor::readData (char* data, qint64 maxSize)
{
  if (!isOpen ()) return -1;
  
  if (outBuffer.size() < maxSize)
  {
    
  }
  
  //if (!isReadable ()) return -1;
}

qint64 QlfIOCompressor::writeData (const char* data, qint64 maxSize)
{
  if (!isOpen ()) return -1;
  
  inBuffer.append (QByteArray (data, maxSize));
  if (inBuffer.size () >= bufferSize) flush ();
  
  return maxSize;
}

