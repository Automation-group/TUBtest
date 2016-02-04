#ifndef QLFRNET_H_
#define QLFRNET_H_

#include <QObject>
#include <QIODevice>

/**
 * \brief Интерфейс RNet 
 */
class QlfRNet : public QObject
{
  Q_OBJECT
public:
  /* Begin class RegValue ----------------------------------------------------- */
  /**
   * \brief Регистр RNet 
   */     
  class RegValue
  {
    friend class QlfRNet;
    public:
      /**
       * Тип RNet регистра
       */
      enum RegType { Bool = 0,Ubyte = 1, Byte = 2, Uint = 3, Int = 4, 
        Ulong = 5, Long = 6, Float = 7, Double = 8, Nothing = 0xFF};

    public:
      /** Коснтруктор копирования
       * \param value значение 
       */
      RegValue (bool value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */         
      RegValue (char value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */         
      RegValue (short value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */         
      RegValue (int value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */         
      RegValue (unsigned char value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */         
      RegValue (unsigned short value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */         
      RegValue (unsigned int value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */
      RegValue (float value);
      
      /** Коснтруктор копирования
       * \param value значение 
       */
      RegValue (double value);
    public:
      
      /** Опрератор приведения типа */
      operator bool ();
      
      /** Опрератор приведения типа */
      operator char ();
      
      /** Опрератор приведения типа */
      operator short ();
      
      /** Опрератор приведения типа */
      operator int ();
      
      /** Опрератор приведения типа */
      operator unsigned char ();
      
      /** Опрератор приведения типа */
      operator unsigned short ();
      
      /** Опрератор приведения типа */
      operator unsigned int ();
      
      /** Опрератор приведения типа */
      operator float ();
      
      /** Опрератор приведения типа */
      operator double ();
    public:
      /**
       * Получить текущий тип регистра
       * \return тип регистра
       */
      RegType getType () const { return regType; };
      
      /**
       * Задать тип регистра
       * \param type тип регистра
       */
      void setType (RegType type) { regType = type; };
    private:
      RegValue (RegType type = Uint);
    private:
      int getDataSize() const;
    private:
      union Data
      {
        unsigned char BoolVal;
        unsigned char UbyteVal;
        char ByteVal;
        unsigned short UintVal;
        short IntVal;
        unsigned int UlongVal;
        int LongVal;
        float FloatVal;
        double DoubleVal; 
      };
    private:
      bool readOnly;
      RegType regType;
      Data data;      
  };
  /* End class RegValue ----------------------------------------------------- */
public:
  /**
   * \brief Конструктор
   * \param ioDevice Указатель на эземпляр класса QIODevice
   */
  QlfRNet (QIODevice* ioDevice);
  
  /**
   * Читать значение регистра
   * \param dev Адрес устройства на шине RNet
   * \param cha Номер канала
   * \param reg Номер регистра
   * \return Значение регистра
   */
  RegValue readReg (unsigned char dev, unsigned char cha, unsigned char reg) const;
  
  /**
   * Записать регистр
   * \param dev Адрес устройства на шине RNet
   * \param cha Номер канала
   * \param reg Номер регистра
   * \param value Новое значение регистра
   * \return Если true регистр записан
   */
  bool writeReg (unsigned char dev, unsigned char cha, unsigned char reg, const RegValue &value) const;
  
protected:
  /**
   * Расчитать CRC - контрольную сумму сообщения
   * \param data указатель на первый элемент в сообщении
   * \param datalen длина сообщения
   * \return контрольная сумма
   */
  char calculate_crc(const QByteArray& data) const;  
private:
  QIODevice* ioDevice;
};

#endif /*QLFRNET_H_*/
