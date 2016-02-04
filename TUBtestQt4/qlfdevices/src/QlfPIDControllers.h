/*
 * QlfPIDControllers.h
 *
 *  Created on: 26.11.2008
 *      Author: lexa
 */

#ifndef QLFPIDCONTROLLERS_H_
#define QLFPIDCONTROLLERS_H_

#include "PIDController.h"
#include <QIODevice>
#include "QlfRNet.h"
#include "QlfModbus.h"

class QlfTestPIDController: public PIDController
{
public:
	QlfTestPIDController()
		: m_setpoint(0.0), m_derivativeTerm(0.0),
		m_integralTerm(0.0), m_propartionalTerm(50.0) {}
	~QlfTestPIDController() {}

	double getProcessVariable() const;

	void setSetPoint(double setpoint) { m_setpoint = setpoint; }
	double getSetPoint() const { return m_setpoint; }

	void setDerivativeTerm(double derivativeTerm) { m_derivativeTerm = derivativeTerm; }
	double getDerivativeTerm() const { return m_derivativeTerm; }

	void setIntegralTerm(double integralTerm) { m_integralTerm = integralTerm; }
	double getIntegralTerm() const { return m_integralTerm; }

	void setProportionalTerm(double propartionalTerm) { m_propartionalTerm = propartionalTerm; }
	double getProportionalTerm() const { return m_propartionalTerm; }

	double getOutput() const;
private:
	double m_setpoint;
	double m_derivativeTerm;
	double m_integralTerm;
	double m_propartionalTerm;
};

class QlfMetakonPIDController: public PIDController
{
public:
	QlfMetakonPIDController(QIODevice* device, int dev, int cha);
	~QlfMetakonPIDController();

	double getProcessVariable() const;

	void setSetPoint(double setpoint);
	double getSetPoint() const;

	void setDerivativeTerm(double derivativeTerm);
	double getDerivativeTerm() const;

	void setIntegralTerm(double integralTerm);
	double getIntegralTerm() const;

	void setProportionalTerm(double propartionalTerm);
	double getProportionalTerm() const;

	double getOutput() const;
private:
	QlfRNet m_rnet;
	int m_dev;
	int m_cha;
};

class QlfEuroThermPIDController: public PIDController
{
public:
	QlfEuroThermPIDController(QIODevice* device, int dev);
	~QlfEuroThermPIDController();

	double getProcessVariable() const;

	void setSetPoint(double setpoint);
	double getSetPoint() const;

	void setDerivativeTerm(double derivativeTerm);
	double getDerivativeTerm() const;

	void setIntegralTerm(double integralTerm);
	double getIntegralTerm() const;

	void setProportionalTerm(double propartionalTerm);
	double getProportionalTerm() const;

	double getOutput() const;
private:
	QlfModbus m_modbus;
	int m_dev;
};

#endif /* QLFPIDCONTROLLERS_H_ */
