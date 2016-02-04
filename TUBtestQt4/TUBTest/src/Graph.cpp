#include <QDesktopServices>

#include "../include/Graph.h"

Graph::Graph(QWidget *parent) : QWidget(parent){
  ui.setupUi(this);
  
  // Задаём положение окна по центру
  /*QDesktopWidget desktop;
  QRect rect = desktop.availableGeometry(desktop.primaryScreen());
  QPoint center = rect.center();
  center.setX(center.x() - (this->width()/2));
  center.setY(center.y() - (this->height()/2));
  move(center);*/
    
  // Создаём окно
  this->setWindowTitle(QString::fromUtf8("График"));
  
  // Файл с настройками
  settings_read = new QSettings("settings.conf",QSettings::IniFormat);
  settings_write = new QSettings("settings.conf",QSettings::IniFormat);
  
  // Настройка отрисовки графика
  grid = new QwtPlotGrid;
  grid->enableXMin(true);
  grid->setMajPen(QPen(Qt::black,0,Qt::DotLine));
  grid->setMinPen(QPen(Qt::gray,0,Qt::DotLine));
  grid->attach(ui.qwtGraph);
  
  curveTimeForce = new QwtPlotCurve();
  curveTimeForce->attach(ui.qwtGraph);
  
  QPen curvePen;
  curvePen.setStyle(Qt::DashLine);
  curvePen.setBrush(Qt::blue);
  curveTimeForce->setPen(curvePen);
  
  QwtSymbol symbolPoint;
  symbolPoint.setStyle(QwtSymbol::Rect);
  symbolPoint.setPen(QColor(Qt::blue));
  symbolPoint.setSize(5);
  curveTimeForce->setSymbol(symbolPoint);
  
  // Установка единиц измерения силы
  connect( this->ui.rbAdc, SIGNAL( clicked() ), this, SLOT( rbAdcSet() ) );
  connect( this->ui.rbNewton, SIGNAL( clicked() ), this, SLOT( rbNewtonSet() ) );
  connect( this->ui.rbKgf, SIGNAL( clicked() ), this, SLOT( rbKgfSet() ) );
  nPoints = 0;
  ui.rbAdc->setChecked(1);
  ui.qwtGraph->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("F, ед. ацп"));
  ui.qwtGraph->setAxisTitle(QwtPlot::xBottom,QString::fromUtf8("t, секунды"));
  
  // Загрузка настроек
  loadSettings();
}

// Отслеживание событий
void Graph::rbAdcSet() {
  ui.qwtGraph->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("F, ед. ацп"));
  conversionValueForce();
}
void Graph::rbNewtonSet() {
  ui.qwtGraph->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("F, Ньютоны"));
  conversionValueForce();
}
void Graph::rbKgfSet() {
  ui.qwtGraph->setAxisTitle(QwtPlot::yLeft,QString::fromUtf8("F, кгс"));
  conversionValueForce();
}

// Пересчёт силы в другие единицы
void Graph::conversionValueForce() {
  for(unsigned int i=0; i<nPoints ;i++) {
    if(ui.rbAdc->isChecked()) dataYforce[i] = dataForceADC[i];
    if(ui.rbNewton->isChecked()) dataYforce[i] = coeff_a*dataForceADC[i]+coeff_b;
    if(ui.rbKgf->isChecked()) dataYforce[i] = (coeff_a*dataForceADC[i]+coeff_b)/9.81;
  }
  // Перерисовка графика
  curveTimeForce->setData(dataXtime, dataYforce, nPoints);
  ui.qwtGraph->replot();
}

// Добавление новых значений и перерисовка графика
void Graph::graphRelot(double timeXmc, double forceYadc, double coeffA, double coeffB) {
  dataXtime[nPoints] = timeXmc/1000.0; // секунды
  dataForceADC[nPoints] = forceYadc; // ед. ацп
  
  // Сохранение последний значений для пересчёта силы
  coeff_a = coeffA; 
  coeff_b = coeffB;
  
  // Пересчёт значений по оси Y
  if(ui.rbAdc->isChecked()) dataYforce[nPoints] = forceYadc;
  if(ui.rbNewton->isChecked()) dataYforce[nPoints] = coeffA*forceYadc+coeffB;
  if(ui.rbKgf->isChecked()) dataYforce[nPoints] = (coeffA*forceYadc+coeffB)/9.81;
  ui.lcdForce->display(dataYforce[nPoints]);
  
  nPoints++;
  if(nPoints >= 65535) nPoints = 0; // защита от переполнения
  curveTimeForce->setData(dataXtime, dataYforce, nPoints);
  ui.qwtGraph->replot();
}

// Очистка графика
void Graph::graphClear() {
  nPoints = 0;
  curveTimeForce->setData(dataXtime, dataYforce, nPoints);
  ui.qwtGraph->replot();
}

// Загрузка настроек
void Graph::loadSettings() {
  // Положение и размеры ui_MainWindow
  this->setGeometry(settings_read->value("settings/graph/graphGeometry").toRect());
  
  // Загрузка единиц измерения силы
  if(settings_read->value("settings/graph/graphYadc").value<bool>()) {
    ui.rbAdc->setChecked(true);
    rbAdcSet();
  }
  if(settings_read->value("settings/graph/graphYNewton").value<bool>()) {  
    ui.rbNewton->setChecked(true);
    rbNewtonSet();
  }
  if(settings_read->value("settings/graph/graphYkgf").value<bool>()) {
    ui.rbKgf->setChecked(true);
    rbKgfSet();
  }
}

// Сохранить настройки
void Graph::saveSettings() {
  // Сохранение размера и положения ui_Graph
  settings_write->setValue("settings/graph/graphGeometry", geometry());
  
  // Сохранение единиц измерения силы
  if(ui.rbAdc->isChecked()) settings_write->setValue("settings/graph/graphYadc", true);
  else settings_write->setValue("settings/graph/graphYadc", false);
  
  if(ui.rbNewton->isChecked()) settings_write->setValue("settings/graph/graphYNewton", true);
  else settings_write->setValue("settings/graph/graphYNewton", false);
  
  if(ui.rbKgf->isChecked()) settings_write->setValue("settings/graph/graphYkgf", true);
  else settings_write->setValue("settings/graph/graphYkgf", false);
}

Graph::~Graph() {
  saveSettings();
}
