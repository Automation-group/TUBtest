#pragma once

#include <QObject>
#include <QIODevice>
#include <QByteArray>
#include <QVariant>

class QlfOven : public QObject
{
  Q_OBJECT
public:
  /**
   * \brief Конструктор
   * \param ioDevice Указатель на эземпляр класса QIODevice
   */
  QlfOven (QIODevice* ioDevice);  

  /** Записать целое значение в регистр
   * \param addr 11 битный адрес устройства
   * \param name имя локального идентификатора
   * \param value новое значение параметра
   * \return true - если успешно 
   */
  bool writeInt(int addr, const char* name, int value);

  /** Прочитать целое значение из регистра
   * \param addr 11 битный адрес устройства
   * \param name имя локального идентификатора
   * \param status результат выполнения команды, true - успешно
   * \return значение 
   */
  int readInt(int addr, const char* name, bool* status = 0);

  /** Прочитать целое значение из регистра
   * \param addr 11 битный адрес устройства
   * \param name имя локального идентификатора
   * \param dataType тип читаемого параметра (DEC_doti, STR, STORED_DOT, INCR, U or UB, 
   * F32, F24, Time, ASCII or T, I)
   * \param status результат выполнения команды, true - успешно
   * \return значение 
   */
  QVariant read(int addr, const char* name, const QByteArray& dataType = "I",  bool* status = 0);

  bool write(int addr, const char* name, const QVariant& value, const QByteArray& dataType = "");

  /** Прочитать данные из регистра
   * \param addr 11 битный адрес устройства
   * \param name имя локального идентификатора
   * \param status результат выполнения команды, true - успешно
   * \return данные
   */
  QByteArray readData(int addr, const char* name, bool* status = 0);

  /** Записать данные в регистр
   * \param addr 11 битный адрес устройства
   * \param name имя локального идентификатора
   * \param result данные
   * \return резульата выполнения команды, true - успешно
   */
  bool writeData(int addr, const char* name, const QByteArray& data);
protected:
  /** Вычисление контрольной суммы CRC16
   * \param data Указатель на данные
   * \param datalen Размер данных в байтах
   * \return Контрольная сумма CRC16
   */
  unsigned short calculate_crc16(const QByteArray& data) const;     

  /** Вычисление контрольной суммы CRC16 для имени параметра
   * \param data Указатель на данные
   * \param datalen Размер данных в байтах
   * \return Контрольная сумма CRC16
   */
  unsigned short calculate_name_crc16(const QByteArray& data) const;

  /** Закодировать сообщение по принципу "Тетрада в ASCII символ"
   * \param message сообщение
   * \return закодированное сообщение
   */
  QByteArray encodeMessage(const QByteArray& message) const;

  /** Раскодировать сообщение закодированное по принципу "Тетрада в ASCII символ"
   * \param message закодированное сообщение
   * \return раскодированное сообщение, если раскодировка не удалась вернет пустое сообщение 
   */
  QByteArray decodeMessage(const QByteArray& message) const;

  /** Закодировать локальный идентификатор параметра
   * \param name локальный идентификатор параметра
   * \return закодированный локальный идентификатор параметра
   */
  QByteArray encodeName(const QByteArray& name) const;
  
private:
  QIODevice *device;  
};

