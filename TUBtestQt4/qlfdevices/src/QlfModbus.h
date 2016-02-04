#ifndef QLFMODBUS_H_
#define QLFMODBUS_H_

#include <QObject>
#include <QIODevice>

class QlfModbus : public QObject
{
  Q_OBJECT
public:
  /**
   * \brief Конструктор
   * \param ioDevice Указатель на эземпляр класса QIODevice
   */
  QlfModbus (QIODevice* ioDevice);  
  
  /** Читать слово из устройсва
   * \param devAddress Адрес устройсва на шине
   * \param address Номер регистра
   */
  unsigned short readWord (unsigned char devAddress, unsigned short address);
  
  /**  Читать несколько слов из устройсва
   * \param devAddress Адрес устройсва на шине
   * \param address Номер регистра
   * \param wordsToRead Сколько слов читать
   * \param words Указатель на буфер куда будет записан результат чтения. 
   * \return Кол-во прочитанных слов
   */
  int readWords (unsigned char devAddress, unsigned short address, 
		 unsigned short wordsToRead, unsigned short* words);
  
  /** Записать слово
   * \param devAddress Адрес устройсва на шине
   * \param address Номер регистра
   * \param value Значение которое следует записать в регистр
   * \return true - если успешно
   */
  bool writeWord (unsigned char devAddress, unsigned short address, 
		  unsigned short value);
  
protected:
  /** Вычисление контрольной суммы CRC16
   * \param data Указатель на данные
   * \param datalen Размер данных в байтах
   * \return Контрольная сумма CRC16
   */
  unsigned short calculate_crc16(const unsigned char* data, int datalen) const;     
private:
  QIODevice *device;  
};

#endif /*QLFMODBUS_H_*/
