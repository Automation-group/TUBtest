#ifndef MAINDLG_H_
#define MAINDLG_H_

#include <QSettings>
#include <QMainWindow>
#include <qextserialport.h>
#include <QWidget>
#include <QlfTUB.h>
#include <QTimer>

#include "../build/ui_MainWindow.h"
#include "../include/Graph.h"

class MainWindow : public QMainWindow
{
	Q_OBJECT
public:
	MainWindow ();
	~MainWindow ();
	
protected:
	void closeEvent(QCloseEvent *event);
	
private Q_SLOTS:
	void on_pbOpen_clicked();
	void on_pbClose_clicked();
	void on_pbReadRange_clicked();
	void on_pbWriteRange_clicked();
	void on_pbMeasure_clicked();
	void on_pbStart_clicked();
	void on_pbStop_clicked();
	void dataReady (int code, int msTime);
	void on_pbClear_clicked();
	void on_pbSave_clicked();
	void on_tbAddFolder_clicked();
	void on_pbCalibrationMode_clicked();
	void on_pbVerificationMode_clicked();
	void on_pbContiniousMode_clicked();
	void on_pbGraph_clicked();
	void on_pbSearchDevices_clicked();
	
private:
	Ui_MainWindow ui;
	QextSerialPort tubSP;
	QlfTUB* tub;
	Graph *graphView;
  
	void resultsBrowserVerificationInfo();
	void resultsBrowserCalibrationInfo();
	void resultsBrowserContiniousInfo();
	bool messageQuestion();
	
	QSettings *settings_read;
	QSettings *settings_write;
	void saveSettings();
	void loadSettings();

};
#endif /*MAINDLG_H_*/