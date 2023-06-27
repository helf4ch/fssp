#include "signalpage.h"

namespace fssp {

SignalPage::SignalPage(SignalData data, QWidget *parent) : QWidget{parent} {
  m_data = std::make_shared<SignalData>(std::move(data));

  m_isSelected = false;
  m_leftSelection = 0;
  m_rightSelection = 0;

  m_navDialog = new NavigationDialog(m_data);
  m_graphDialog = new GraphDialog(m_data);

  connect(m_data.get(), &SignalData::selectionEvent, this,
          &SignalPage::handleSelection);
  connect(m_data.get(), &SignalData::waveformVisibilityEvent, this,
          &SignalPage::handleWaveformVisibility);

  QHBoxLayout *hBox = new QHBoxLayout();
  hBox->addWidget(m_navDialog);
  hBox->setAlignment(m_navDialog, Qt::AlignLeft);
  hBox->addSpacing(20);
  hBox->addWidget(m_graphDialog);

  setLayout(hBox);
}

void SignalPage::handleSelection() {
  m_navDialog->drawWaveforms();
  m_graphDialog->drawWaveforms();
}

void SignalPage::handleWaveformVisibility() { m_graphDialog->drawWaveforms(); }

}  // namespace fssp
