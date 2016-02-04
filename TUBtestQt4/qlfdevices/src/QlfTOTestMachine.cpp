#include "QlfTOTestMachine.h"
#include <QByteArray>
#include <iostream>

QlfTOTestMachine::QlfTOTestMachine (QIODevice* ioDevice)
  : ioDevice (ioDevice)
{

}

QlfTOTestMachine::~QlfTOTestMachine ()
{

}

void QlfTOTestMachine::ReadLoadCellAndXhead (int& loadcell, int& xhead)
{

  ioDevice -> write ("RB\r", 3);
  loadcell = readLine ().toInt ();
  xhead = readLine ().toInt ();
  /*
  loadcell = ReadLoadCellValue ();
  xhead = ReadXheadValue();
  */
}

int QlfTOTestMachine::ReadLoadCellIdent ()
{
  ioDevice -> write ("RC\r", 3);
  return readLine ().toInt ();
}

QString QlfTOTestMachine::ReadMachineIdent ()
{
  ioDevice -> write ("RI\r", 3);
  return readLine ();
}

int QlfTOTestMachine::ReadLoadCellValue ()
{
  ioDevice -> write ("RL\r", 3);
  return readLine ().toInt ();
}

int QlfTOTestMachine::ReadXheadValue()
{
  ioDevice -> write ("RP\r", 3);
  return readLine ().toInt ();
}

int QlfTOTestMachine::ReadMachineStatus()
{
  ioDevice -> write ("RS\r", 3);
  return readLine ().toInt ();
}

double QlfTOTestMachine::ReadSoftwareXheadVelocity()
{
  ioDevice -> write ("RV\r", 3);
  return readLine ().toDouble ();
}

void QlfTOTestMachine::SetRollingAverageValue(int value)
{
  QByteArray buf = "WA";
  buf.append (QByteArray::number (value));
  buf.append ('\r');
  ioDevice -> write (buf);
  readLine ();
}

void QlfTOTestMachine::ClearRemoteMode()
{
  ioDevice -> write ("WC\r",3);
  readLine ();
}

void QlfTOTestMachine::RelativePositionalMove(int value)
{
  QByteArray buf = "WE";
  buf.append (QByteArray::number (value / 1000.0, 'f', 3));
  buf.append ('\r');
  ioDevice -> write (buf);
  readLine ();
}

void QlfTOTestMachine::MoveXheadForward()
{
  ioDevice -> write("WF\r", 3);
  readLine ();
}

void QlfTOTestMachine::ToggleTEST_LED(bool value)
{
  QByteArray buf = "WG";
  buf.append (QByteArray::number (value ? 1 : 0));
  buf.append ('\r');
  ioDevice -> write (buf);
  readLine ();
}

void QlfTOTestMachine::AbsolutePositionalMove(int value)
{
  QByteArray buf = "WM";
  buf.append (QByteArray::number (value / 1000.0, 'f', 3));
  buf.append ('\r');
  ioDevice -> write (buf);
  readLine ();
}

void QlfTOTestMachine::ZeroAuxChannel(int value)
{
  QByteArray buf = "WN";
  buf.append (QByteArray::number (value));
  buf.append ('\r');
  ioDevice -> write (buf);
  readLine ();
}

void QlfTOTestMachine::ZeroXhead()
{
  ioDevice -> write ("WP\r", 3);
  readLine ();
}

void QlfTOTestMachine::ReverseXhead()
{
  ioDevice -> write ("WR\r", 3);
  readLine ();
}

void QlfTOTestMachine::HaltXhead()
{
  ioDevice -> write ("WS\r", 3);
  readLine ();
}

void QlfTOTestMachine::AutoreverseXhead(bool value)
{
  QByteArray buf = "WT";
  buf.append (QByteArray::number (value ? 1 : 0));
  buf.append ('\r');
  ioDevice -> write (buf);
  readLine ();
}

void QlfTOTestMachine::SetSowtwareXheadVelocity(double value)
{
  QByteArray buf = "WV";
  buf.append (QByteArray::number (value, 'f', 4));
  buf.append ('\r');
  ioDevice -> write (buf);
  readLine ();
}

void QlfTOTestMachine::ZeroLoadCell()
{
  ioDevice -> write ("WZ\r",3);
  readLine ();
}

double QlfTOTestMachine::getLoadCellRange (int loadCellIdent) const
{
  switch (loadCellIdent)
  {
    case 0x53: return 250000.0;
    case 0x52: return 200000.0;
    case 0x51: return 100000.0;
    case 0x45: return 50000.0;
    case 0x44: return 30000.0;
    case 0x43: return 25000.0;
    case 0x42: return 20000.0;
    case 0x41: return 10000.0;
    case 0x35: return 5000.0;
    case 0x34: return 3000.0;
    case 0x33: return 2500.0;
    case 0x32: return 2000.0;
    case 0x31: return 1000.0;
    case 0x25: return 500.0;
    case 0x24: return 300.0;
    case 0x23: return 250.0;
    case 0x22: return 200.0;
    case 0x21: return 100.0;
    case 0x15: return 50.0;
    case 0x14: return 30.0;
    case 0x13: return 25.0;
    case 0x12: return 20.0;
    case 0x11: return 10.0;
    case 0x5: return 5.0;
  }
  return 0.0;
}

QByteArray QlfTOTestMachine::readLine ()
{
  // рабочий код
  QByteArray result;
  char c = '\0';
  while(ioDevice->getChar(&c) && c != '\r')
	result.push_back(c);
  return result;
}
