#include "QlfTUB.h"
#include "QlfThread.h"
#include <QMutexLocker>

QlfTUB::QlfTUB(QIODevice* ioDevice) :
	ioDevice(ioDevice), mutex(), waitCondition(), terminated(false), tubThread(
			this, &QlfTUB::runThread), readLineFlag(false)
{
	timer.start();
	tubThread.start();
}

QlfTUB::~QlfTUB()
{
	terminated = true;
	tubThread.wait();
	tubThread.terminate();
}

char QlfTUB::testConnection()
{
	ioDevice -> write("ID\n", 3);
	QByteArray buf = readLine();
	if ( buf=="ID2f8771d5ebf55e0983210304c6d5197e\n" )
		return 1;
	return 0;
}

void QlfTUB::run(int msTimeout)
{
	QByteArray buf("RC");
	buf.append(QByteArray::number(msTimeout));
	buf.append('\n');
	ioDevice -> write(buf);
}

void QlfTUB::stop()
{
	ioDevice -> write("SC\n", 3);
}

int QlfTUB::measure()
{
	ioDevice -> write("GM\n", 3);
	QByteArray buf = readLine();
	buf.remove(0, 2);
	buf.chop(1);
	return buf.toInt();
}

void QlfTUB::setRange(Range range)
{
	QByteArray buf("SO");
	buf.append(QByteArray::number(range.minimum));
	buf.append(',');
	buf.append(QByteArray::number(range.maximum));
	buf.append('\n');
	ioDevice -> write(buf);

	buf = readLine();
}

QlfTUB::Range QlfTUB::getRange()
{
	QByteArray buf("SO\n");
	ioDevice -> write(buf);
	buf = readLine();
	buf = buf.mid(2);
	buf.chop(1);
	
	QList<QByteArray> res = buf.split(',');
	if (res.size() < 2)
		return Range(0, 0);
	
	return Range(res[0].toInt(), res[1].toInt());
}

char QlfTUB::symbolState()
{
	ioDevice -> write("GS\n", 3);
	QByteArray buf = readLine();
	//if (buf.isEmpty())
	//	return '\0';
	//return buf[2];
	if (buf[2]=='D' || buf[2]=='C' || buf[2]=='W')
		return buf[2];
	return '\0';
}

QByteArray QlfTUB::readLine()
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

void QlfTUB::resetTimer()
{
	timer.restart();
}

void QlfTUB::onADCCode(QByteArray& data)
{
	data.remove(0, 2);
	data.chop(1);

	dataReady(data.toInt(), timer.elapsed());
}

void QlfTUB::runThread(TUB_Thread* thread)
{
	while (!terminated)
	{
		if ( !ioDevice -> isOpen())
			continue;

		QByteArray buf = ioDevice -> readLine(100);

		if (buf.isEmpty() && !readLineFlag)
			continue;

		if (buf.startsWith("GR"))
		{
			onADCCode(buf);
			continue;
		}

		QMutexLocker locker(&mutex);

		buffer.push_back(buf);
		waitCondition.wakeOne();
	}
}
