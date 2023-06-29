#pragma once

#include <QMenu>
#include <QMouseEvent>
#include <QPainter>

#include "basewaveform.h"

namespace fssp {

class NavigationWaveform : public BaseWaveform {
  Q_OBJECT
 public:
  explicit NavigationWaveform(std::shared_ptr<SignalData> signalData,
                              int number, QWidget *parent = nullptr);

  void drawWaveform() override;

 protected:
  void mousePressEvent(QMouseEvent *event) override;
  void showContextMenu(const QPoint &pos);

 private:
  bool m_isVisible;
};

}  // namespace fssp
