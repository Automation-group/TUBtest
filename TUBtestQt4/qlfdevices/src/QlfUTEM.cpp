#include "QlfUTEM.h"
#include "QlfThread.h"
#include <QMutexLocker>

QlfUTEM::QlfUTEM(QIODevice* ioDevice) :
	ioDevice(ioDevice), mutex(), waitCondition(), terminated(false), tubThread(
			this, &QlfUTEM::runThread), readLineFlag(false)
{
	timer.start();
	tubThread.start();
}

QlfUTEM::~QlfUTEM()
{
	terminated = true;
	tubThread.wait();
	tubThread.terminate();
}

void QlfUTEM::run(int msTimeout)
{
	QByteArray buf("GC");
	buf.append(QByteArray::number(msTimeout));
	buf.append('\n');
	ioDevice -> write(buf);
}

void QlfUTEM::stop()
{
	ioDevice -> write("SC\n", 3);
}

char QlfUTEM::testConnection()
{
	ioDevice -> write("ID\n", 3);
	QByteArray buf = readLine();
	if ( buf=="ID8e7bf390e3969acc2136d04e9efc470c\n" )
		return 1;
	return 0;
}

QByteArray QlfUTEM::readLine()
{
	QMutexLocker locker(&mutex);
	if (buffer.isEmpty())
	{
		readLineFlag = true;
		if (!waitCondition.wait(&mutex) )
		{
			readLineFlag = false;
			return QByteArray();
		}
	}

	QByteArray buf = buffer.front();
	buffer.pop_front();
	readLineFlag = false;
	return buf;
}

void QlfUTEM::resetTimer()
{
	timer.restart();
}

void QlfUTEM::onCode(QByteArray& data)
{
	data = data.mid(2);
	data.chop(1);
	
	QList<QByteArray> res = data.split(' ');

	dataReady(res[0].toLong(), res[1].toLong());
}

void QlfUTEM::runThread(TUB_Thread* thread)
{
	while (!terminated)
	{
		if ( !ioDevice -> isOpen())
			continue;

		QByteArray buf = ioDevice -> readLine(100);

		if (buf.isEmpty() && !readLineFlag)
			continue;

		if (buf.startsWith("GD"))
		{
			onCode(buf);
			continue;
		}

		QMutexLocker locker(&mutex);

		buffer.push_back(buf);
		waitCondition.wakeOne();
	}
}
