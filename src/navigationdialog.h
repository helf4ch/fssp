#pragma once

#include <QGroupBox>
#include <QScrollArea>
#include <QVBoxLayout>
#include <QWidget>

#include "navigationwaveform.h"

namespace fssp {

class NavigationDialog : public QGroupBox {
  Q_OBJECT
 public:
  explicit NavigationDialog(std::shared_ptr<SignalData> data,
                            QWidget *parent = nullptr);

  void drawWaveforms();

 private:
  std::shared_ptr<SignalData> m_signalData;
  std::vector<NavigationWaveform *> m_waveforms;

  QWidget *scrollContent;
};

}  // namespace fssp
