#pragma once

#include <QDateTime>

namespace fssp {

class SignalData : public QObject {
  Q_OBJECT
 public:
  explicit SignalData();

  explicit SignalData(const QDateTime &startTime, const QDateTime &endTime,
                      const double rate, const double timeForOne,
                      const size_t allTime, std::vector<QString> &&channelsName,
                      std::vector<std::vector<double>> &&data);

  SignalData(const SignalData &that);

  SignalData(SignalData &&that);

  SignalData &operator=(SignalData that);

  friend void swap(SignalData &first, SignalData &second);

  QDateTime startTime() const;
  QDateTime endTime() const;

  double rate() const;
  double timeForOne() const;
  size_t allTime() const;

  QString unitOfTime() const;
  size_t divisionBase() const;

  int leftArray() const;
  int rightArray() const;

  size_t leftTime() const;
  size_t rightTime() const;

  bool isGridEnabled() const;
  bool isGlobalScale() const;
  bool isSelected() const;

  void setLeftArray(int leftArray);
  void setRightArray(int rightArray);

  void setLeftTime(size_t leftTime);
  void setRightTime(size_t rightTime);

  void setGridEnabled(bool isGridEnabled);
  void setGlobalScale(bool isGlobalScale);
  void setSelected(bool isSelected);

  void setUnitOfTime(QString unitOfTime);
  void setDivisionBase(size_t divisionBase);

  void calculateArrayRange();

  const std::vector<QString> &channelsName() const;
  const std::vector<std::vector<double>> &data() const;

  int channelsNumber() const;
  int samplesNumber() const;

  const std::vector<bool> &visibleWaveforms() const;
  void setWaveformVisibility(int number, bool isVisible);

 signals:
  void changedGraphTimeRange();

  void changedWaveformVisibility();

  void changedEnableGrid();
  void changedGlobalScale();

 private:
  QDateTime m_startTime;
  QDateTime m_endTime;

  double m_rate;
  double m_timeForOne;

  size_t m_allTime;

  QString m_unitOfTime;
  size_t m_divisionBase;

  std::vector<QString> m_channelsName;
  std::vector<std::vector<double>> m_data;

  int m_channelsNumber;
  int m_samplesNumber;

  int m_leftArray;
  int m_rightArray;

  size_t m_leftTime;
  size_t m_rightTime;

  bool m_isGridEnabled;
  bool m_isGlobalScale;
  bool m_isSelected;

  std::vector<bool> m_visibleWaveforms;
};

}  // namespace fssp
