/*
 * QlfSensors.cpp
 *
 *  Created on: 25.11.2008
 *      Author: lexa
 */

#include "QlfSensors.h"
#include <math.h>
#include <QTime>

TestSensor::TestSensor()
{
}

double TestSensor::getTemperature()
{
	static QTime time;
	static bool flag = true;
	if (flag)
	{
		time.start();
		flag = false;
	}
	double t = (double)time.elapsed()/1000.0; // секунды
	return log(t + 1.0) * 50.0 + 2.0*sin(2.0*M_PI*t/60.0 + M_PI*cos(2.0*M_PI*t/111.0));
}

TMBSensor::TMBSensor(QIODevice* device, int channel) :
	m_tmb(device), channel(channel), parent(0)
{

}

TMBSensor::TMBSensor(TMBSensor* parent, int channel) :
	m_tmb(0), channel(channel), parent(parent)
{

}

double TMBSensor::getTemperature()
{
	if (m_tmb.getDevice())
		cache = m_tmb.readTemperatures();
	if (parent)
		cache = parent -> cache;
	return cache[channel];
}

RNetSensor::RNetSensor(QIODevice* device, unsigned char dev, unsigned char cha,
		unsigned char reg) :
	m_rnet(device), dev(dev), cha(cha), reg(reg)
{
}

double RNetSensor::getTemperature()
{
	return m_rnet.readReg(dev, cha, reg);
}

EuroThermSensor::EuroThermSensor(QIODevice* device, unsigned short dev,
		unsigned short reg) :
	m_modbus(device), dev(dev), reg(reg)
{

}

double EuroThermSensor::getTemperature()
{
	return m_modbus.readWord(dev, reg);
}
