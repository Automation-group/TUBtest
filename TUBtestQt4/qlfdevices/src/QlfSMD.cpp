#include "QlfSMD.h"

QlfSMD::QlfSMD(QIODevice* ioDevice) :
	QlfSPDevice(ioDevice)
{

}

QlfSMD::State QlfSMD::state()
{
	write("GE\n");
	QByteArray buf = readLine();
	if (buf == "GED\n")
		return MotorDisable;
	if (buf == "GER\n")
		return MotorRotating;
	if (buf == "GES\n")
		return MotorStoped;
	return UnknownState;
}

QlfSMD::Direction QlfSMD::getDirection()
{
	write("GD\n");
	if (readLine() == "GDF\n")
		return CW;
	return CCW;
}

QlfSMD::Conformity QlfSMD::conformity()
{
	write("GU\n");
	QByteArray buf = readLine();
	if (buf.size()<4) return MoveUpForCW;
	
	return buf[2] == '1' ? MoveUpForCW : MoveUpForCCW;
}

void QlfSMD::getLimits (bool& forwardLimit, bool& reverseLimit)
{
	forwardLimit = true;
	reverseLimit = true;
	write("GT\n");
	QByteArray buf = readLine();
	if (buf.size()<5) return;
	
	forwardLimit = buf[2] == 'D';
	reverseLimit = buf[3] == 'D';
}

bool QlfSMD::getOverloading()
{
	write("GMT\n");
	return readLine() == "GMT1\n";	
}

bool QlfSMD::getOverheat()
{
	write("GMF\n");
	return readLine() == "GMF1\n";		
}

double QlfSMD::getFrequency()
{
	write("GF\n");
	QByteArray buf = readLine();
	buf = buf.mid(2);
	buf.chop(1);
	return buf.toDouble() / 1000.0;
}

bool QlfSMD::testConnection()
{
	write("GE\n");
	return !readLine ().isEmpty();
}

void QlfSMD::run(int stepCount)
{
	QByteArray buf("RM");
	if (stepCount > 0)
	{
		buf.append(QByteArray::number(stepCount));
	}
	buf.append('\n');
	write(buf);
	buf = readLine();
}

void QlfSMD::stop()
{
	write("SM\n");
	readLine();
}

void QlfSMD::enable()
{
	write("EM\n");
	readLine();
}

void QlfSMD::disable()
{
	write("DM\n");
	readLine();
}

void QlfSMD::setDirection(Direction direction)
{
	QByteArray buf("SD");
	switch (direction)
	{
	case CW:
		buf.append('F');
		break;
	case CCW:
		buf.append('B');
		break;
	}
	buf.append('\n');
	write(buf);
	buf = readLine();
}

void QlfSMD::setStepCounter(int counterValue)
{
	QByteArray buf("SC");
	buf.append(QByteArray::number(counterValue));
	buf.append('\n');
	write(buf);
	readLine();
}

int QlfSMD::getStepCounter()
{
	write("GC\n");
	QByteArray buf = readLine();
	buf = buf.mid(2);
	buf.chop(1);
	return buf.toInt();
}

void QlfSMD::setConformity(Conformity conformity)
{
	QByteArray buf("SU");
	switch (conformity)
	{
	case MoveUpForCW:
		buf.append('1');
		break;
	case MoveUpForCCW:
		buf.append('0');
		break;
	}

	buf.append('\n');
	write(buf);
	buf = readLine();
}

void QlfSMD::setFrequency(double frequency)
{
	QByteArray buf("SF");
	buf.append(QByteArray::number((int)(frequency * 1000.0)));
	buf.append('\n');
	write(buf);
	buf = readLine();
}

bool QlfSMD::lineFilter(QByteArray& byteArray)
{
	if (!byteArray.startsWith("EV"))
		return true;
		
	if (byteArray == "EVRD\n") runDone();
	
	if (byteArray == "EVDU\n") forwardTrailerDown();

	if (byteArray == "EVDD\n") reverseTrailerDown();

	if (byteArray == "EVUU\n") forwardTrailerUp();

	if (byteArray == "EVUD\n") reverseTrailerUp();

	if (byteArray == "EVEF\n") overheat();

	if (byteArray == "EVET\n") overload();
	
	return false;
}
