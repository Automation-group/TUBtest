#ifndef QLFIOCOMPRESSOR_H_
#define QLFIOCOMPRESSOR_H_

#include <QIODevice>
#include <QByteArray>

class QlfIOCompressor: public QIODevice
{
  Q_OBJECT
public:
  enum StreamFormat {ZlibFormat};
public:
  QlfIOCompressor (QIODevice* device, int compressionLevel = 6, int bufferSize = 65500); 
  ~QlfIOCompressor ();
  bool open (QIODevice::OpenMode mode);
  void close ();
  void flush ();
protected:
  qint64 readData (char* data, qint64 maxSize);
  qint64 writeData (const char* data, qint64 maxSize);
private:
  QIODevice* device;
  StreamFormat streamFormat;
  int compressionLevel;
  int bufferSize;
  QByteArray inBuffer;
  QByteArray outBuffer;
};

#endif /*QLFIOCOMPRESSOR_H_*/
