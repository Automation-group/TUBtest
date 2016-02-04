#ifndef QLFTMB_H_
#define QLFTMB_H_

#include <QObject>
#include <QIODevice>
#include <QVector>

/**
 * \brief Интерфейс к ИТБ
 */
class QlfTMB: public QObject
{
	Q_OBJECT
public:
	/**
	 * \brief Конструктор
	 * \param sPort Указатель на эземпляр класса SerialPort
	 */
	QlfTMB (QIODevice* ioDevice);

	QIODevice* getDevice() const
	{
		return ioDevice;
	}

	typedef QVector<double> TMBData;

	/**
	 * Прочитать значения температуры
	 * \return  от [0] до [3] значения температуры i-го канала и [4] - температура холодных спаев.
	 */
	TMBData readTemperatures();

	/**
	 * Прочитать значения кодов АЦП
	 * \return [0] до [3] код АЦП  i-го канала и [4] - код АЦП холодных спаев.
	 */
	TMBData readCodes();
private:
	TMBData readData();
private:
	QIODevice* ioDevice;
};

#endif /*QLFTMB_H_*/
