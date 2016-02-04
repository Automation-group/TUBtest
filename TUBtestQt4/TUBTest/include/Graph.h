#ifndef GRAPH_H
#define GRAPH_H

#include <qwt_plot.h>
#include <qwt_plot_canvas.h>
#include <qwt_legend.h>
#include <qwt_plot_grid.h>
#include <qwt_plot_curve.h>
#include <qwt_symbol.h>

#include <QSettings>
#include <QtGui>
#include "../build/ui_Graph.h"

class Graph : public QWidget {
  Q_OBJECT
  
public:
  explicit Graph(QWidget *parent = 0);
  ~Graph();
  
  double dataXtime[65535];
  double dataYforce[65535];
  double dataForceADC[65535]; // сила в единицах АЦП 
  long nPoints;
  double coeff_a;
  double coeff_b;
  
  void graphRelot(double timeXmc, double forceYadc, double coeffA, double coeffB);
  void graphClear();
  void conversionValueForce(); 
  void saveSettings();
  void loadSettings();
  
public slots:
  void rbAdcSet();
  void rbNewtonSet();
  void rbKgfSet();
  
private:
  Ui_graphView ui;
  QwtPlotCurve* curveTimeForce;
  QwtPlotGrid* grid;
    
  QSettings *settings_read;
  QSettings *settings_write;
};

#endif
