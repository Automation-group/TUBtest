/*
 * TemperatureSensor.h
 *
 *  Created on: 25.11.2008
 *      Author: lexa
 */

#ifndef TEMPERATURESENSOR_H_
#define TEMPERATURESENSOR_H_

class TemperatureSensor
{
public:
	TemperatureSensor();
	virtual double getTemperature() = 0;
	virtual ~TemperatureSensor();
};

#endif /* TEMPERATURESENSOR_H_ */
