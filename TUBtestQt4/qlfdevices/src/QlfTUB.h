#ifndef QLFTUB_H_
#define QLFTUB_H_

#include <QObject>
#include <QIODevice> 
#include "QlfThread.h"
#include <QList>
#include <QMutex>
#include <QWaitCondition>
#include <QTime>

class QlfTUB: public QObject
{
	Q_OBJECT
public:

	struct Range
	{
		int minimum;
		int maximum;
		Range (int min, int max)
			: minimum(min), maximum(max) {}
	};
	
	QlfTUB (QIODevice* ioDevice);

	~QlfTUB ();

	/**
	 * Состояние прибора
	 * \return 'D' - тензодатчик не подключен;
	 * 'С' - запущен непрерывный режим;
	 * 'W' - прибор готов к работе
	 * '\0' - Прибор не отвечает
	 */
	char symbolState ();
	char testConnection ();
	
	/**
	 * Провести измерение
	 * \return код АЦП
	 */
	int measure ();

	Range getRange();
public Q_SLOTS:

	void resetTimer ();

	/**
	 * Запустить измерения в непрерывном режиме
	 * \param msTimeout интервал измерений, мс
	 */
void run(int msTimeout = 100);

/**
 * Остановить измерения
 */
void stop();

/**
 * Установить допустимый диапазон сил
 * \param min - минимальный код АЦП (соответствует минимально допустимой силе)
 * \param max - максимальныйй код АЦП (соответствует максимально допустимой силе)
 */
void setRange(Range);

Q_SIGNALS:
/**
 * Результат измерения в непрерывном режиме
 * \param code код АЦП
 * \param msTime время прихода данных в мс
 */
void dataReady (int code, int msTime);
private:
typedef QlfThread<QlfTUB> TUB_Thread;
void runThread(TUB_Thread* thread);

void onADCCode(QByteArray& data);
QByteArray readLine();
private:
QIODevice* ioDevice;
QList<QByteArray> buffer;
TUB_Thread tubThread;
QMutex mutex;
QWaitCondition waitCondition;
bool terminated;
bool readLineFlag;
QTime timer;
};

#endif /*QLFTUB_H_*/
