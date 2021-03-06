#include "../include/SearchDevices.h"
#include <qextserialport.h>
#include <QlfTUB.h>

#include <QtDebug>
#include <QString>
#include <QProcess>
#include <QRegExp>
#include <QThread>

// Функция задержки для поиска порта в windows
struct MyThread : QThread {
	static void sleep(int t) {
		QThread::sleep(t);
	}
};

// Определение операционной системы
QString SearchDevices::osName() {
#if defined(Q_OS_MAC)
    return QLatin1String("osx");
#elif defined(Q_OS_WIN)
    return QLatin1String("windows");
#elif defined(Q_OS_LINUX)
    return QLatin1String("linux");
#endif
}

void SearchDevices::SearchDev(QString *portTUB)
{ 
  *portTUB = "";
  
  // Поиск всех подключённых устройств
  QProcess process;
  process.setProcessChannelMode(QProcess::SeparateChannels);
  process.setReadChannel(QProcess::StandardOutput);
  if(osName() == "linux") process.start("ls /dev", QIODevice::ReadWrite);
  if(osName() == "windows") process.start("cmd.exe /k mode", QIODevice::ReadWrite);
  process.waitForReadyRead (500);
  
  QString ports;
  QString buffer;
  ports = QString::fromUtf8(process.readAllStandardOutput());
  process.close();
  process.waitForFinished(); // ожидание до полного завершения process
  
  // Задрежка для корректного поиска порта
  if(osName() == "windows") MyThread::sleep(1);
  
  // Определение портов отвечающих TUB
  int sizeBuffer = 0;
  for (int i = 0; i<ports.size(); i++)
  {
    buffer[i-sizeBuffer] = ports[i];
    if (ports[i]=='\n'){
      QString portBuf = buffer;
	  
      // Поиск портов в linux
      if (osName()=="linux" && portBuf.remove(QRegExp("[\n\t\r 0123456789]"))=="ttyUSB") {
	QString portDev = "/dev/" + buffer.remove(QRegExp("[\n\t\r ]"));
	//qDebug() << portDev;
	if (*portTUB == "" && SearchTUB(portDev)) *portTUB = portDev;
      }
      
      // Поиск портов в windows
      if (osName()=="windows") {
	QRegExp rx("(COM\\d+)");
	int pos = rx.indexIn(portBuf, 0);
	if (pos != -1) {
	  QString portDev = rx.cap(1); 
	  //qDebug() << portDev; 
	  if (*portTUB == "" && SearchTUB(portDev)) *portTUB = portDev;		  
	}
      }
	  
      sizeBuffer = i;
      buffer = "";
    }
  }
}

bool SearchDevices::SearchTUB(QString port)
{
  bool ret = 0; 
  QextSerialPort tubSP;
  
  tubSP.setBaudRate(BAUD115200);
  tubSP.setDataBits(DATA_8);
  tubSP.setParity(PAR_NONE);
  tubSP.setStopBits(STOP_2);
  tubSP.setFlowControl(FLOW_OFF);
  tubSP.setTimeout(0, 500); // for linux
  //tubSP.setTimeout(100); // for windows
  //tubSP.setQueryMode(QextSerialPort::Polling); // for windows
  tubSP.setPortName(port);
  
  //tubSP.open(QIODevice::ReadWrite | QIODevice::Unbuffered);
  if (!tubSP.open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
    //qDebug() << QString::fromUtf8("Ошибка при инициализации последовательнго порта TUB");
	return ret;
  }

  tub = new QlfTUB (&tubSP);
  if (tub->testConnection()) {
    //qDebug() << QString::fromUtf8("Ошибка подключения к TUB");
    ret = 1;
  }
  
  if (tub) {
    QlfTUB* temp = tub;
    tub = 0;
    delete temp;
  }
  tubSP.close();
  
  return ret;
}
