#pragma once

#include <QComboBox>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QHBoxLayout>
#include <QLabel>
#include <QMessageBox>
#include <QSpinBox>
#include <QString>
#include <QVBoxLayout>
#include <QWidget>

#include "signaldata.h"

namespace fssp {

class StatisticWindow : public QDialog {
  Q_OBJECT
 public:
  explicit StatisticWindow(std::shared_ptr<SignalData> data, int curSignal,
                           int intervalsNumber, QWidget *parent = nullptr);

 protected slots:
  void onGraphTimeRangeChange();

 private:
  void calculateStatistic();
  void showDialog();
  void setLabelsText();

 private:
  std::shared_ptr<SignalData> p_signalData;

  QGraphicsView *p_graph;

  QLabel *m_minValueLabel;
  QLabel *m_maxValueLabel;
  QLabel *m_avgValueLabel;

  QLabel *m_dispersionLabel;
  QLabel *m_standardDeviationLabel;

  QLabel *m_variationFactorLabel;
  QLabel *m_asymmetryFactorLabel;
  QLabel *m_kurtosisFactorLabel;

  QLabel *m_medianLabel;

  QLabel *m_minQuantileLabel;
  QLabel *m_maxQuantileLabel;

  double p_minValue;
  double p_maxValue;
  double p_avgValue = 0;

  double p_dispersion = 0;
  double p_standardDeviation;

  double p_variationFactor = 0;
  double p_asymmetryFactor = 0;
  double p_kurtosisFactor = 0;

  double p_median;

  double p_minQuantile;
  double p_maxQuantile;

  int p_intervalsNumber;
  int p_curSignal;
};

}  // namespace fssp
