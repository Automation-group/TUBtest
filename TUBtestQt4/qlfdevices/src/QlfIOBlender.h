#ifndef QLFBLENDER_H_
#define QLFBLENDER_H_

#include <QIODevice>

/**
 * Смеситель
 */
class QlfIOBlender: public QIODevice
{
  Q_OBJECT
public:
  /** Конструктор
   * \param inDevice входное устройсво
   * \param outDevice выходное устройство
   */
  QlfIOBlender (QIODevice* inDevice = 0, QIODevice* outDevice = 0);
protected:
  qint64 readData (char* data, qint64 maxSize);
  qint64 writeData (const char* data, qint64 maxSize);
private:
  QIODevice* inDevice;
  QIODevice* outDevice;
};

#endif /*QLFBLENDER_H_*/
