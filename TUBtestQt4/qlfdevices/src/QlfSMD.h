#ifndef QLFSMD_H_
#define QLFSMD_H_

#include <QObject>
#include <QIODevice>
#include "QlfSPDevice.h"

class QlfSMD : public QlfSPDevice
{
  Q_OBJECT
public:
  QlfSMD (QIODevice* ioDevice);
  
  enum State
  {
    MotorRotating,
    MotorDisable,
    MotorStoped,
    UnknownState
  };
  
  enum Direction
  {
    CW, // по часовой стрелке
    CCW // против часовой стрелке
  };
  
  enum Conformity
  {
    /**
     * Пуансон перемещается вверх
     * при врещении двигателя по часовой стрелке 
     */
    MoveUpForCW = 1,
    /**
     * Пуансон перемещается вверх
     * при врещении двигателя против часовой стрелки 
     */
    MoveUpForCCW = 0
  };
    
public:
  /** Получить состояние ШД
   * \return 
   */
  State state ();
  
  
  /** Получить направление вращения ШД
   * \return направление вращения
   */
  Direction getDirection ();
  
  /** Получить частоту шагов 
   */
  double getFrequency ();
  
  /**
   * Проверить подключение
   * @return true БШД подключен
   */
  bool testConnection ();
  
  void getLimits (bool& forwardLimit, bool& reverseLimit);
  /**
   * Перегрузка
   */
  bool getOverloading();
  /**
   * Перегрев
   */
  bool getOverheat();
  
  Conformity conformity();
  
  /** Получить состояние счетчика циклов
   */
  int getStepCounter ();
  
public Q_SLOTS:
  /**
   * Запустить ШД
   * \param stepCount число шагов, 
   * если stepCount == 0 число шагов неограничено
   */
  void run (int stepCount = 0);
  
  /** Остановить ШД
   */
  void stop ();
  
  /**
   * Включить ШД
   */
  void enable ();
  
  /** Выключить ШД
   */
  void disable ();
  
  /**
   * Задать соответствие между направлением движения пуансона
   * и направлением движения двигателя
   */
  void setConformity (Conformity conformity);
  
  /** Задать напрвление вращения ШД
   * \param direction направление вращения
   */
  void setDirection (Direction direction);
  
  /** Установить счетчик циклов
   */
  void setStepCounter (int counterValue);
  
  /** Задать частоту шагов
   */
  void setFrequency (double frequency);
Q_SIGNALS:
  void runDone ();
  /**
   * Перегрузка
   */
  void overload ();
  
  /**
   * Двигатель перегрелся
   */
  void overheat ();

  /**
   * Нажат верхний концевик
   */
  void forwardTrailerDown ();
  
  /**
   * Отпущен верхний концевик
   */
  void forwardTrailerUp ();
  
  /**
   * Нажат нижний концевик
   */
  void reverseTrailerDown ();
  
  /**
   * Отпущен нижний концевик
   */
  void reverseTrailerUp ();
protected:
  bool lineFilter (QByteArray& byteArray);
};

#endif /*QLFSMD_H_*/
