#ifndef QLFTOTESTMACHINE_H_
#define QLFTOTESTMACHINE_H_

#include <QObject>
#include <QIODevice>
#include <QString>
#include <QList>
#include <QMutex>
#include <QWaitCondition>

#include "QlfThread.h"

/** \class TOTestMachine
 * \brief Интерфейс к Tinius Olsen
 * 
 * Класс управления разрывной машиной Tinius Olsen.
 */
class QlfTOTestMachine : public QObject
{
  Q_OBJECT
  public:
    /**
     * \brief Конструктор
     * \param serialPort Указатель на эземпляр класса SerialPort
     */
    QlfTOTestMachine (QIODevice* ioDevice);
    
    /**
     * Деструктор
     */
    ~QlfTOTestMachine ();
    
    /**
     * Читать значения силы (в кодах АЦП) и положение траверсы (в мкм).
     * \param loadcell сила в кодах АЦП.
     * \param xhead положение траверсы в мкм.
     */
    void ReadLoadCellAndXhead (int& loadcell, int& xhead);
    
    /** \brief Прочитать код тензодатчика (LoadCell)
     * \return Код тензодатчика (LoadCell)
     * 
     * <table>
     * <tr><td><b>Код тензодатчика</b></td><td><b>Нагрузка, Н</b></td></tr>
     * <tr><td>(DEC)</td><td>значение</td></tr>
     * <tr><td>83</td><td>250000</td></tr>
     * <tr><td>82</td><td>200000</td></tr>
     * <tr><td>81</td><td>100000</td></tr>
     * <tr><td>69</td><td>50000</td></tr>
     * <tr><td>68</td><td>30000</td></tr>
     * <tr><td>67</td><td>25000</td></tr>
     * <tr><td>66</td><td>20000</td></tr>
     * <tr><td>65</td><td>10000</td></tr>
     * <tr><td>53</td><td>5000</td></tr>
     * <tr><td>52</td><td>3000</td></tr>
     * <tr><td>51</td><td>2500</td></tr>
     * <tr><td>50</td><td>2000</td></tr>
     * <tr><td>49</td><td>1000</td></tr>
     * <tr><td>37</td><td>500</td></tr>
     * <tr><td>36</td><td>300</td></tr>
     * <tr><td>35</td><td>250</td></tr>
     * <tr><td>34</td><td>200</td></tr>
     * <tr><td>33</td><td>100</td></tr>
     * <tr><td>21</td><td>50</td></tr>
     * <tr><td>20</td><td>30</td></tr>
     * <tr><td>19</td><td>25</td></tr>
     * <tr><td>18</td><td>20</td></tr>
     * <tr><td>17</td><td>10</td></tr>
     * <tr><td>05</td><td>5</td></tr>
     * </table> 
     */
    int ReadLoadCellIdent();
    
    /** Прочитать идентификатор испытатальной машины
     * \return Идентификатор испытатальной машины
     */
    QString ReadMachineIdent();
    
    /** Прочитать значение нагрузки
     * \return Нагрузка в кодах АЦП
     */
    int ReadLoadCellValue();
    
    /** Положение траверсы
     * \return Положение траверсы, мкм
     */
    int ReadXheadValue();
    
    /** Прочитать статус испытательной машины
     * \return Статус
     * 
     * <table>
     * <tr><td><b>Статус</b></td><td><b>Если 0</b></td><td><b>Если 1</b></td></tr>
     * <tr><td>Бит</td><td>Описание</td><td>Описание</td></tr>
     * <tr><td>0</td><td>Вверх</td><td>Вниз</td></tr>
     * <tr><td>1</td><td>Запущена</td><td>Остановлена</td></tr>
     * <tr><td>2</td><td>На позиции</td><td>Движется на позицию</td></tr>
     * <tr><td>3</td><td>-</td><td>Перегрузка Мотор/Питание</td></tr>
     * <tr><td>4</td><td>-</td><td>Превышение диапазона движения траверсы вперед</td></tr>
     * <tr><td>5</td><td>-</td><td>Превышение диапазона движения траверсы назад</td></tr>
     * <tr><td>6</td><td>-</td><td>Перегрузка тензодатчика на растяжение</td></tr>
     * <tr><td>7</td><td>-</td><td>Перегрузка тензодатчика на сжатие</td></tr>
     * </table>      
     */
    int ReadMachineStatus();
    
    /** Получить програмно установленную скорость движения траверсы
     * \return Скорость движения траверсы, мм/мин
     */
    double ReadSoftwareXheadVelocity();
    
    /**
     * Смотри описание к разр. машине
     */
    void SetRollingAverageValue(int value);
    
    /**
     * Смотри описание к разр. машине
     */     
    void ClearRemoteMode();
    
    /**
     * Смотри описание к разр. машине
     */     
    void RelativePositionalMove(int value);
    
    /** Включить движение травесы вперед
     */
    void MoveXheadForward();
    
    /** Включить/выключить диод на кнопке TEST
     * \param value true Включить диод.
     *  false  Выключить диод.
     */
    void ToggleTEST_LED(bool value);
    
    /** Переместить траверсу в заданное положение
     * \param value Координата нового положения траверсы в мкм.
     */
    void AbsolutePositionalMove(int value);

    /**
     * Смотри описание к разр. машине
     */           
    void ZeroAuxChannel(int value);
    
    /** Обнулить датчик положения траверсы
     */
    void ZeroXhead();
    
    /** Включить движение травесы назад
     */     
    void ReverseXhead();

    /** Остановить движение травесы
     */       
    void HaltXhead();
    
    /**
     * Смотри описание к разр. машине
     */     
    void AutoreverseXhead(bool value);
    
    /** Установить скорость движения траверсы
     * \param value Скорость движения траверсы, мм/мин
     */
    void SetSowtwareXheadVelocity(double value);
    
    // void SelectAuxChannelsToRead(int value);
    
    /** Обнулить датчик силы
     */
    void ZeroLoadCell();
    
    /** Получить диапазон нагрузок тензодатчика по его коду
     * \see ReadLoadCellIdent()
     * \param loadCellIdent Код тензодатчика
     * \return Диапазон силы, Ньютон. Соответствует коду АЦП = 30000.
     */
    double getLoadCellRange (int loadCellIdent) const;
  Q_SIGNALS:

    void pressTestButton ();
  private:    
    //    typedef QlfThread<QlfTOTestMachine> TOThread;
    //    void runThread (TOThread* thread);
    QByteArray readLine ();    
  private:
    QIODevice* ioDevice;
    //QList<QByteArray> buffer;
    //TOThread thread;
    //QMutex mutex;
    //QWaitCondition waitCondition;
    //bool terminated;
    //bool readLineFlag;    
};

#endif /*QLFTOTESTMACHINE_H_*/
