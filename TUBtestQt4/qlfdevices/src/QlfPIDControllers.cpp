/*
 * QlfPIDControllers.cpp
 *
 *  Created on: 26.11.2008
 *      Author: lexa
 */

#include "QlfPIDControllers.h"
#include <QTime>
#include <math.h>

double QlfTestPIDController::getProcessVariable() const
{
	static QTime time;
	static bool flag = true;
	if (flag)
	{
		time.start();
		flag = false;
	}
	double t = (double)time.elapsed()/1000.0; // секунды
	return log(t + 1.0) * 47.8 + 1.6*sin(2.0*M_PI*t/45.0 + M_PI*cos(2.0*M_PI*t/167.0));
}

double QlfTestPIDController::getOutput() const
{
	return (m_setpoint - getProcessVariable()) / m_propartionalTerm * 100.0;
}

QlfMetakonPIDController::QlfMetakonPIDController(QIODevice* device, int dev, int cha)
	: m_rnet(device), m_dev(dev), m_cha(cha)
{

}

QlfMetakonPIDController::~QlfMetakonPIDController()
{

}

double QlfMetakonPIDController::getProcessVariable() const
{

}

void QlfMetakonPIDController::setSetPoint(double setpoint)
{

}

double QlfMetakonPIDController::getSetPoint() const
{

}

void QlfMetakonPIDController::setDerivativeTerm(double derivativeTerm)
{

}

double QlfMetakonPIDController::getDerivativeTerm() const
{

}

void QlfMetakonPIDController::setIntegralTerm(double integralTerm)
{

}

double QlfMetakonPIDController::getIntegralTerm() const
{

}

void QlfMetakonPIDController::setProportionalTerm(double propartionalTerm)
{

}

double QlfMetakonPIDController::getProportionalTerm() const
{

}

double QlfMetakonPIDController::getOutput() const
{

}

QlfEuroThermPIDController::QlfEuroThermPIDController(QIODevice* device, int dev)
	: m_modbus(device), m_dev(dev)
{

}

QlfEuroThermPIDController::~QlfEuroThermPIDController()
{

}

double QlfEuroThermPIDController::getProcessVariable() const
{

}

void QlfEuroThermPIDController::setSetPoint(double setpoint)
{

}

double QlfEuroThermPIDController::getSetPoint() const
{

}

void QlfEuroThermPIDController::setDerivativeTerm(double derivativeTerm)
{

}

double QlfEuroThermPIDController::getDerivativeTerm() const
{

}

void QlfEuroThermPIDController::setIntegralTerm(double integralTerm)
{

}

double QlfEuroThermPIDController::getIntegralTerm() const
{

}

void QlfEuroThermPIDController::setProportionalTerm(double propartionalTerm)
{

}

double QlfEuroThermPIDController::getProportionalTerm() const
{

}

double QlfEuroThermPIDController::getOutput() const
{

}
