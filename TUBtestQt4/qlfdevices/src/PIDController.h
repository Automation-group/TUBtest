/*
 * PIDController.h
 *
 *  Created on: 26.11.2008
 *      Author: lexa
 */

#ifndef PIDCONTROLLER_H_
#define PIDCONTROLLER_H_

class PIDController
{
public:
	PIDController();
	virtual ~PIDController() = 0;

	virtual double getProcessVariable() const = 0;

	virtual void setSetPoint(double setpoint) = 0;
	virtual double getSetPoint() const = 0;

	virtual void setDerivativeTerm(double derivativeTerm) = 0;
	virtual double getDerivativeTerm() const = 0;

	virtual void setIntegralTerm(double integralTerm) = 0;
	virtual double getIntegralTerm() const = 0;

	virtual void setProportionalTerm(double propartionalTerm) = 0;
	virtual double getProportionalTerm() const = 0;

	/*!
	 * Выходная мощность (управляющий сигнал) в процентах
	 */
	virtual double getOutput() const = 0;
};

#endif /* PIDCONTROLLER_H_ */
