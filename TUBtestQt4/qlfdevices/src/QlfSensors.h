/*
 * QlfSensors.h
 *
 *  Created on: 25.11.2008
 *      Author: lexa
 */

#ifndef QLFSENSORS_H_
#define QLFSENSORS_H_

#include "QlfRNet.h"
#include "QlfTMB.h"
#include "QlfModbus.h"
#include "TemperatureSensor.h"

class TestSensor: public TemperatureSensor
{
public:
	TestSensor();
	double getTemperature();
};

class TMBSensor: public TemperatureSensor
{
public:
	TMBSensor(QIODevice* device, int channel);
	TMBSensor(TMBSensor* parent, int channel);
	double getTemperature();
private:
	QlfTMB m_tmb;
	QlfTMB::TMBData cache;
	int channel;
	TMBSensor* parent;
};

class RNetSensor: public TemperatureSensor
{
public:
	RNetSensor(QIODevice* device, unsigned char dev, unsigned char cha,
			unsigned char reg);
	double getTemperature();
private:
	QlfRNet m_rnet;
	unsigned char dev;
	unsigned char cha;
	unsigned char reg;
};

class EuroThermSensor: public TemperatureSensor
{
public:
	EuroThermSensor(QIODevice* device, unsigned short dev, unsigned short reg);
	double getTemperature();
private:
	QlfModbus m_modbus;
	unsigned short dev;
	unsigned short reg;
};

#endif /* QLFSENSORS_H_ */
