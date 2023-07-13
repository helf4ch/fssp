#include "signaldata.h"

namespace fssp {

SignalData::SignalData() {
  m_samplesNumber = 1000;
  m_rate = 1;
  m_timeForOne = 1 / m_rate;
  m_allTime = m_timeForOne * m_timeForOne * 1000;

  m_startTime = QDateTime{QDate{2000, 1, 1}, QTime{0, 0}};
  m_endTime = m_startTime.addMSecs(m_allTime);

  m_channelsNumber = 1;

  m_channelsName = std::vector<QString>(m_channelsNumber);
  m_data = std::vector<std::vector<double>>(
      m_channelsNumber, std::vector<double>(m_samplesNumber));

  m_leftArray = 0;
  m_rightArray = m_samplesNumber - 1;

  m_leftTime = 0;
  m_rightTime = m_allTime - 1;
}

SignalData::SignalData(const QDateTime &startTime, const QDateTime &endTime,
                       const double rate, const double timeForOne,
                       const size_t allTime,
                       std::vector<QString> &&channelsName,
                       std::vector<std::vector<double>> &&data) {
  m_startTime = startTime;
  m_endTime = endTime;

  m_rate = rate;
  m_timeForOne = timeForOne;
  m_allTime = allTime;

  m_channelsName = std::move(channelsName);
  m_data = std::move(data);

  m_channelsNumber = m_channelsName.size();
  m_samplesNumber = m_data[0].size();

  m_leftArray = 0;
  m_rightArray = m_samplesNumber - 1;

  m_leftTime = 0;
  m_rightTime = m_allTime - 1;
}

SignalData::SignalData(const SignalData &that) {
  m_startTime = that.m_startTime;
  m_endTime = that.m_endTime;

  m_rate = that.m_rate;
  m_timeForOne = that.m_timeForOne;
  m_allTime = that.m_allTime;

  m_channelsName = that.m_channelsName;
  m_data = that.m_data;

  m_channelsNumber = that.m_channelsNumber;
  m_samplesNumber = that.m_samplesNumber;

  m_visibleWaveforms = std::vector<bool>(m_channelsNumber, false);

  m_leftArray = that.m_leftArray;
  m_rightArray = that.m_rightArray;

  m_leftTime = that.m_leftTime;
  m_rightTime = that.m_rightTime;
}

SignalData::SignalData(SignalData &&that) {
  m_startTime = that.m_startTime;
  m_endTime = that.m_endTime;

  m_rate = that.m_rate;
  m_timeForOne = that.m_timeForOne;
  m_allTime = that.m_allTime;

  m_channelsName = std::move(that.m_channelsName);
  m_data = std::move(that.m_data);

  m_channelsNumber = that.m_channelsNumber;
  m_samplesNumber = that.m_samplesNumber;

  m_visibleWaveforms = std::move(that.m_visibleWaveforms);

  m_leftArray = that.m_leftArray;
  m_rightArray = that.m_rightArray;

  m_leftTime = that.m_leftTime;
  m_rightTime = that.m_rightTime;
}

SignalData &SignalData::operator=(SignalData that) {
  swap(*this, that);

  return *this;
}

void swap(SignalData &first, SignalData &second) {
  using std::swap;

  swap(first.m_startTime, second.m_startTime);
  swap(first.m_endTime, second.m_endTime);

  swap(first.m_rate, second.m_rate);
  swap(first.m_timeForOne, second.m_timeForOne);
  swap(first.m_allTime, second.m_allTime);

  swap(first.m_channelsName, second.m_channelsName);
  swap(first.m_data, second.m_data);

  swap(first.m_channelsNumber, second.m_channelsNumber);
  swap(first.m_samplesNumber, second.m_samplesNumber);

  swap(first.m_visibleWaveforms, second.m_visibleWaveforms);

  swap(first.m_leftArray, second.m_leftArray);
  swap(first.m_rightArray, second.m_rightArray);

  swap(first.m_leftTime, second.m_leftTime);
  swap(first.m_rightTime, second.m_rightTime);
}

QDateTime SignalData::startTime() const { return m_startTime; }

QDateTime SignalData::endTime() const { return m_endTime; }

double SignalData::rate() const { return m_rate; }

double SignalData::timeForOne() const { return m_timeForOne; }

size_t SignalData::allTime() const { return m_allTime; }

QString SignalData::unitOfTime() const { return m_unitOfTime; }

size_t SignalData::divisionBase() const { return m_divisionBase; }

int SignalData::leftArray() const { return m_leftArray; }

int SignalData::rightArray() const { return m_rightArray; }

size_t SignalData::leftTime() const { return m_leftTime; }

size_t SignalData::rightTime() const { return m_rightTime; }

bool SignalData::isGridEnabled() const { return m_isGridEnabled; }

bool SignalData::isGlobalScale() const { return m_isGlobalScale; }

bool SignalData::isSelected() const { return m_isSelected; }

const std::vector<QString> &SignalData::channelsName() const {
  return m_channelsName;
}

const std::vector<std::vector<double>> &SignalData::data() const {
  return m_data;
}

void SignalData::addData(const QString name, std::vector<double> data) {
  ++m_channelsNumber;
  m_channelsName.push_back(name);
  m_data.push_back(data);
  m_visibleWaveforms.push_back(false);

  emit dataAdded();
}

int SignalData::channelsNumber() const { return m_channelsNumber; }

int SignalData::samplesNumber() const { return m_samplesNumber; }

const std::vector<bool> &SignalData::visibleWaveforms() const {
  return m_visibleWaveforms;
}

void SignalData::setWaveformVisibility(int number, bool isVisible) {
  m_visibleWaveforms[number] = isVisible;
}

void SignalData::setGridEnabled(bool isGridEnabled) {
  m_isGridEnabled = isGridEnabled;
}

void SignalData::setGlobalScale(bool isGlobalScale) {
  m_isGlobalScale = isGlobalScale;
}

void SignalData::setSelected(bool isSelected) { m_isSelected = isSelected; }

void SignalData::setLeftArray(int leftArray) { m_leftArray = leftArray; }

void SignalData::setRightArray(int rightArray) { m_rightArray = rightArray; }

void SignalData::setLeftTime(size_t leftTime) { m_leftTime = leftTime; }

void SignalData::setRightTime(size_t rightTime) { m_rightTime = rightTime; }

void SignalData::setUnitOfTime(QString unitOfTime) {
  m_unitOfTime = unitOfTime;
}

void SignalData::setDivisionBase(size_t divisionBase) {
  m_divisionBase = divisionBase;
}

void SignalData::calculateArrayRange() {
  double dataPerTime =
      static_cast<double>(m_samplesNumber) / static_cast<double>(m_allTime);

  m_rightArray = dataPerTime * m_rightTime;
  m_leftArray = dataPerTime * m_leftTime;

  if ((m_rightArray - m_leftArray < 8) && (m_samplesNumber > 16)) {
    if (m_samplesNumber - m_rightArray > 8) {
      m_leftArray = m_rightArray;
      m_rightArray += 8;
    } else {
      m_rightArray = m_leftArray;
      m_leftArray -= 8;
    }
  }
}

}  // namespace fssp
