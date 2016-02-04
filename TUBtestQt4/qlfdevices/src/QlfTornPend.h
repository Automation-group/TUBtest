#ifndef QLFTORNPEND_H_
#define QLFTORNPEND_H_

#include <QObject>
#include <QIODevice>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

#include "QlfThread.h"

/**
 * \brief Интерфейс крутильного маятника
 */
class QlfTornPend: public QObject 
{
  Q_OBJECT
public:
  /** 
   * Состояние маятника
   */
  enum State
  {
    Running, // маятник запущен
    Stoped, // маятник остановлен
    InADCMode, // маятник в режиме АЦП
    InTestMode, // маятник в режиме настройки
    Unknown, // неизвестное состояние
    Disconnected // неподключен
  };
  
public:
  
  /**
   * Коструктор
   * \param sPort Указатель на эземпляр класса SerialPort
   */
  QlfTornPend (QIODevice* ioDevice);
  
  /**
   * Деструктор
   */
  ~QlfTornPend ();
  
public Q_SLOTS:  
  /**
   *  Запустить крутильный маятник 
   */
  void run ();
  
  /** Остановить крутильный маятник
   */
  void stop ();
  
  /** Запустить маятник в режиме настройки
   */
  void runInTestMode ();
  
  /** Запустить маятник в режиме АЦП
   */
  void runADC ();

  /** Остановить режим АЦП
   */
  void stopADC ();
  
  /** Включить магнит 
   */
  void enableMagnet ();
  
  /** Выключить магнит
   */
  void disableMagnet ();
  
  /** Установить подстроечный резистор
   * @param code код положения резистора (0 - мин., 255 - макс.)
   */
  void setResistorCode (int code);
  
  /**
   * Очистить экран крутильного маятника
   */
  void clearScreen ();  
public:  
  /** Получить состояние маятника
   */
  State state ();
  
  /** Получить состояние маятника
   */
  char symbolState ();    
  
  /** Прочитать положение подстроечного резистора
   * @return код положения резистора (0 - мин., 255 - макс.)
   */
  int getResistorCode ();
    
  /** Прочитать идентификатор прибора
   * \return идентификатор
   */
  QString getID ();
   
  /**
   * вывести сообщение на дисплей
   * \param message сообщение
   */
  void print (const char* message);
  
  /**
   * Получить код переключателя усиления
   * \return код переключателя усиления
   */
  int getGainCode ();
Q_SIGNALS:
  /**
   * Получен код АЦП
   * \param code код АЦП
   */
  void adcCodeReaded (int code);
  /**
   * Получены данные периода
   * \param number номер колебания
   * \param maxAmpl максимальная амплитуда в колебании
   * \param minAmpl минимальная амплитуда в колебании
   * \param period период колебания в ед.
   */
  void periodDataReaded (int number, int maxAmpl, int minAmpl, int period);
private:
  typedef QlfThread<QlfTornPend> TPThread;
  void runThread (TPThread* thread);
  QByteArray readLine ();
  void onADCCode (QByteArray& data);
  void onPeriod (QByteArray& data);
private:
  QIODevice* ioDevice;
  QList<QByteArray> buffer;
  TPThread tpThread;
  QMutex mutex;
  QWaitCondition waitCondition;
  bool terminated;
  bool readLineFlag;
};

#endif /*QLFTORNPEND_H_*/
