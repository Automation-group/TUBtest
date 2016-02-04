#include <QMessageBox>
#include <QTextEdit>
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QDir>
#include <QMessageBox>
#include <QCloseEvent>
#include <QSettings>
#include <QWidget>

#include "../include/MainWindow.h"
#include "../include/Graph.h"
#include "../include/SearchDevices.h"

float timeS = 0; // msec

MainWindow::MainWindow() :
  tubSP(), tub(0) {
  
  ui.setupUi(this);
  this->setWindowTitle("TUBtest");
  
  settings_read = new QSettings("settings.conf",QSettings::IniFormat);
  settings_write = new QSettings("settings.conf",QSettings::IniFormat);
  
  ui.pbCalibrationMode->setEnabled(0);
  ui.pbVerificationMode->setEnabled(1);
  ui.pbContiniousMode->setEnabled(1);
  ui.gbStrength->setEnabled (1);
  ui.gbMeasure->setEnabled (1);
  ui.gbCoeff->setEnabled (0);
  ui.gbMinMax->setEnabled (1);
  ui.gbContinious->setEnabled (0);
  
  tubSP.setBaudRate(BAUD115200);
  tubSP.setDataBits(DATA_8);
  tubSP.setParity(PAR_NONE);
  tubSP.setStopBits(STOP_2);
  tubSP.setFlowControl(FLOW_OFF);
  tubSP.setTimeout(0, 500);
  //tubSP.setTimeout(100);
  //tubSP.setQueryMode(QextSerialPort::Polling);
  
  graphView = new Graph();
  
  loadSettings();
}

// Поиск потрта
void MainWindow::on_pbSearchDevices_clicked() {
    QString portTUB = "";

    while (portTUB == "") {

      SearchDevices shDevices;
      shDevices.SearchDev(&portTUB);

      qDebug() << "PortTUB: " << portTUB;
      if (portTUB == "") {
	QMessageBox msgBox;
	msgBox.setText(QString::fromUtf8("Устойство ACC не было найдено."));
        msgBox.setInformativeText(QString::fromUtf8("Провести повторный поиск устройства?"));
        msgBox.setStandardButtons(QMessageBox::Yes| QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Yes);
          
	int ret = msgBox.exec();
        if (ret==(QMessageBox::Cancel)) return;
	} else {
	  ui.leSerialPortName->setText(portTUB);
	  QMessageBox msgBox;
	  msgBox.setText(QString::fromUtf8("Устойство АСС найдено."));
	  msgBox.setInformativeText(portTUB);
	  msgBox.exec();
        }
    }
}

void MainWindow::loadSettings()  {
  // положение и размеры ui_MainWindow
  this->restoreGeometry(settings_read->value("settings/mainWindowGeometry").toByteArray());
  this->restoreState(settings_read->value("settings/mainWindowState").toByteArray());
  
  // положение и размры окна ui_Graph
  
  // порт
  QString port = settings_read->value("settings/port").value<QString>();
  if (port == "")
    ui.leSerialPortName->setText("/dev/ttyUSB0"); // for Linux
    //ui.leSerialPortName->setText("COM4"); // for Windows
  else ui.leSerialPortName->setText(port);
  
  // рабочая директория
  QString work_dir = settings_read->value("settings/work_dir").value<QString>();
  if (work_dir == "")
    ui.leToFolder->setText(QDir::homePath());
  else ui.leToFolder->setText(work_dir);
  
  // коэффициенты полинома
  ui.dsbCoefficientA->setValue(settings_read->value("settings/coefficient_a").value<double>());
  ui.dsbCoefficientB->setValue(settings_read->value("settings/coefficient_b").value<double>());
  
  // временной интервал для непрерывного режима
  ui.sbInterval->setValue(settings_read->value("settings/time_interval").value<double>());
  
  // режим работы повека/калибровка/неприрывный
  if(settings_read->value("settings/CalibrationMode").value<bool>()) {
    resultsBrowserCalibrationInfo();
    
    ui.pbCalibrationMode->setEnabled(0);
    ui.pbVerificationMode->setEnabled(1);
    ui.pbContiniousMode->setEnabled(1);
    
    ui.gbStrength->setEnabled (1);
    ui.gbMeasure->setEnabled (1);
    ui.gbCoeff->setEnabled (0);
    ui.gbMinMax->setEnabled (1);
    ui.gbContinious->setEnabled (0);
  }
  if(settings_read->value("settings/VerificationMode").value<bool>()) {
    resultsBrowserVerificationInfo();
    
    ui.pbCalibrationMode->setEnabled(1);
    ui.pbVerificationMode->setEnabled(0);
    ui.pbContiniousMode->setEnabled(1);
    
    ui.gbStrength->setEnabled (1);
    ui.gbMeasure->setEnabled (1);
    ui.gbCoeff->setEnabled (1);
    ui.gbMinMax->setEnabled (1);
    ui.gbContinious->setEnabled (0);
  }
  if(settings_read->value("settings/ContiniousMode").value<bool>()) {
    resultsBrowserContiniousInfo();
    
    ui.pbCalibrationMode->setEnabled(1);
    ui.pbVerificationMode->setEnabled(1);
    ui.pbContiniousMode->setEnabled(0);
    
    ui.gbStrength->setEnabled (0);
    ui.gbMeasure->setEnabled (0);
    ui.gbCoeff->setEnabled (1);
    ui.gbMinMax->setEnabled (0);
    ui.gbContinious->setEnabled (1);
  }
}

void MainWindow::saveSettings()  {
  // сохраниение настроек
  // положение и размеры ui_MainWindow
  settings_write->setValue("settings/mainWindowGeometry", saveGeometry());
  settings_write->setValue("settings/mainWindowState", saveState());
  
  // порт
  settings_write->setValue("settings/port",  ui.leSerialPortName->text());
  
  // рабочая директория
  settings_write->setValue("settings/work_dir",  ui.leToFolder->text());
  
  // коэффициенты полинома
  settings_write->setValue("settings/coefficient_a",  ui.dsbCoefficientA->value());
  settings_write->setValue("settings/coefficient_b",  ui.dsbCoefficientB->value());
  
  // временной интервал для непрерывного режима
  settings_write->setValue("settings/time_interval",  ui.sbInterval->value());
  
  // режим работы повека/калибровка/неприрывный
  if(!(ui.pbCalibrationMode->isEnabled())) 
  settings_write->setValue("settings/CalibrationMode", true);
  else settings_write->setValue("settings/CalibrationMode", false);
  
  if(!(ui.pbVerificationMode->isEnabled())) 
  settings_write->setValue("settings/VerificationMode",  true);
  else settings_write->setValue("settings/VerificationMode", false);
  
  if(!(ui.pbContiniousMode->isEnabled())) 
  settings_write->setValue("settings/ContiniousMode",  true);
  else settings_write->setValue("settings/ContiniousMode", false);
}
 
// Открыть порт устройста
void MainWindow::on_pbOpen_clicked() {
  
  tubSP.setPortName(ui.leSerialPortName->text());
  
  if (!tubSP.open(QIODevice::ReadWrite | QIODevice::Unbuffered)) {
    QMessageBox::critical (this, QString::fromUtf8("Ошибка"),
			   QString::fromUtf8("Ошибка при инициализации\n последовательнго порта"));
    return;
  }

  ui.pbOpen->setEnabled (false);
  ui.pbClose->setEnabled (true);

  tub = new QlfTUB (&tubSP);

  if (!tub->symbolState()) {
    on_pbClose_clicked();
    QMessageBox::critical (this, tr("Ошибка подключения"),
			   tr("Проверьте подключение устройства"));
    return;
  }
  
  // Чтение данных с устройста
  connect(tub, SIGNAL(dataReady (int,int)), this, SLOT(dataReady (int,int)));
}

// Вывод данных
void MainWindow::dataReady (int code, int msTime) {	
  ui.lcdnMeasure->display(code);
  ui.resultsBrowser->append(QString::number(code));
  
  // Запись данных для построения
  graphView->graphRelot(timeS, code, ui.dsbCoefficientA->value(), ui.dsbCoefficientB->value());
  timeS += (ui.sbInterval->value());
}

// Единичное измерение данных
void MainWindow::on_pbMeasure_clicked() {
  if (!tub) return;
  
  int adc = tub->measure();
  ui.lcdnMeasure->display(tub->measure());
  
  if (!(ui.pbVerificationMode->isEnabled())){
    int adc = tub->measure();
    double strengthGive = ui.dsbStrength->value();
    double strengthMeasured = ui.dsbCoefficientA->value()*adc+ui.dsbCoefficientB->value();
    double accuracy = ((strengthMeasured/strengthGive)-1)*100;
    ui.resultsBrowser->append(QString::number(strengthGive)+QString::fromUtf8(",           ")+
			      QString::number(adc)+QString::fromUtf8(",           ")+
			      QString::number(strengthMeasured)+QString::fromUtf8(",           ")+
			      QString::number(accuracy));
  }
  if (!(ui.pbCalibrationMode->isEnabled())){
    double strengthGive = ui.dsbStrength->value();
    ui.resultsBrowser->append(QString::number(strengthGive)+QString::fromUtf8(",          ")+
			      QString::number(adc));
  }
}

void MainWindow::on_pbClear_clicked() {
  if(messageQuestion()) {
    // Восстановление заголовков
    if (!ui.pbVerificationMode->isEnabled()) resultsBrowserVerificationInfo();
    if (!ui.pbCalibrationMode->isEnabled()) resultsBrowserCalibrationInfo();
    if (!ui.pbContiniousMode->isEnabled()) resultsBrowserContiniousInfo();
  }
}

void MainWindow::on_pbReadRange_clicked() {
  if (!tub) return;
  QlfTUB::Range range = tub->getRange();
  ui.sbMinimum->setValue(range.minimum);
  ui.sbMaximum->setValue(range.maximum);
}

void MainWindow::on_pbWriteRange_clicked() {
  if (!tub) return;
  tub->setRange(
  QlfTUB::Range(ui.sbMinimum->value(), ui.sbMaximum->value()));
}

void MainWindow::on_pbStart_clicked() {
  if (tub) tub->run(ui.sbInterval->value());
}

void MainWindow::on_pbStop_clicked() {
  if (tub) tub->stop();
}

void MainWindow::on_pbClose_clicked() {
  if (tub)
  {
    QlfTUB* temp = tub;
    tub = 0;
    delete temp;
  }
  
  tubSP.close();
  ui.pbOpen->setEnabled (true);
  ui.pbClose->setEnabled (false);
}

void MainWindow::on_pbGraph_clicked() {
  graphView->loadSettings();
  graphView->show();
}

void MainWindow::on_tbAddFolder_clicked() {
  QString dir = QFileDialog::getExistingDirectory(this, tr("Открыть каталог"),
						  ui.leToFolder->text(),
						  QFileDialog::ShowDirsOnly
						  | QFileDialog::DontResolveSymlinks);;
  if (dir != "") ui.leToFolder->setText(dir);
}

void MainWindow::on_pbSave_clicked() {	
  QString fileName = QFileDialog::getSaveFileName(this, tr("Сохранить файл"),
						  ui.leToFolder->text(),
						  tr("Text Files (*.txt)"));
  if (fileName != "")
  {
    QFile file(fileName);
    if (!file.open(QIODevice::WriteOnly))
    {
	QMessageBox::critical(this, tr("Ошибка"),
	tr("Не удалось сохранить файл.\n"));
    }
    else
      {
	QTextStream stream(&file);
	stream << ui.resultsBrowser->toPlainText();
	stream.flush();
	file.close();
      }
  }
}

// Выбран режим калибровки
void MainWindow::on_pbCalibrationMode_clicked() {
  if(messageQuestion()) {
    resultsBrowserCalibrationInfo();
    
    ui.pbCalibrationMode->setEnabled(0);
    ui.pbVerificationMode->setEnabled(1);
    ui.pbContiniousMode->setEnabled(1);
    
    ui.gbStrength->setEnabled (1);
    ui.gbMeasure->setEnabled (1);
    ui.gbCoeff->setEnabled (0);
    ui.gbMinMax->setEnabled (1);
    ui.gbContinious->setEnabled (0);
  }
}

// Выбран режим поверки
void MainWindow::on_pbVerificationMode_clicked() {
  if(messageQuestion()) {
    resultsBrowserVerificationInfo();
    
    ui.pbCalibrationMode->setEnabled(1);
    ui.pbVerificationMode->setEnabled(0);
    ui.pbContiniousMode->setEnabled(1);
    
    ui.gbStrength->setEnabled (1);
    ui.gbMeasure->setEnabled (1);
    ui.gbCoeff->setEnabled (1);
    ui.gbMinMax->setEnabled (1);
    ui.gbContinious->setEnabled (0);
  }
}

// Выбран непрерывный режим
void MainWindow::on_pbContiniousMode_clicked() {
  if(messageQuestion()) {
    resultsBrowserContiniousInfo();
    
    ui.pbCalibrationMode->setEnabled(1);
    ui.pbVerificationMode->setEnabled(1);
    ui.pbContiniousMode->setEnabled(0);
    
    ui.gbStrength->setEnabled (0);
    ui.gbMeasure->setEnabled (0);
    ui.gbCoeff->setEnabled (1);
    ui.gbMinMax->setEnabled (0);
    ui.gbContinious->setEnabled (1);
  }
}

void MainWindow::resultsBrowserVerificationInfo() {
  ui.resultsBrowser->append(QString::fromUtf8("Заданая        ")+QString::fromUtf8("Код,      ")+
			    QString::fromUtf8("Измеренная    ")+QString::fromUtf8("Погрешность, %"));
  ui.resultsBrowser->append(QString::fromUtf8(" сила , Н      ")+QString::fromUtf8("ед.ацп,   ")+
			    QString::fromUtf8("    сила Н\n"));
}

void MainWindow::resultsBrowserCalibrationInfo(){
  ui.resultsBrowser->append(QString::fromUtf8("Заданая        ")+QString::fromUtf8("Код,       "));
  ui.resultsBrowser->append(QString::fromUtf8(" сила , Н      ")+QString::fromUtf8("ед.ацп\n"));
}

void MainWindow::resultsBrowserContiniousInfo() {
  ui.resultsBrowser->append(QString::fromUtf8("  Код,"));
  ui.resultsBrowser->append(QString::fromUtf8("ед.ацп\n"));
}

bool MainWindow::messageQuestion() {
  int ret = QMessageBox::question(this, tr("Вопрос"),
				  tr("Удалить все данные из текстового просмотрщика?"),
				  QMessageBox::Yes | QMessageBox::No);
  if (ret==(QMessageBox::Yes)) {
    graphView->graphClear(); // очистак графика
    timeS = 0; // обнуление времени измерения
    ui.resultsBrowser->clear(); // очистка просмотрщика
    return true;
  }
  else return false;
}

// Выход из программы
void MainWindow::closeEvent(QCloseEvent *event) {
  int ret = QMessageBox::question(this, tr("Вопрос"),
				  tr("Вы действительно хотите выйти из программы?"),
				  QMessageBox::Yes | QMessageBox::No);
  if (ret==(QMessageBox::Yes)) {
    // Сохранение настроек
    saveSettings();
    
    // Закрытие окон и портов (выполняется раньше деструктора класса)
    delete graphView;
    if (tub) on_pbClose_clicked();
    event->accept();
  }
  else event->ignore();
}

// Деструктор класса
MainWindow::~MainWindow () {
}