#include "basewaveform.h"

#include <QPainter>

namespace fssp {

BaseWaveform::BaseWaveform(std::shared_ptr<SignalData> signalData, int number,
                           QWidget *parent)
    : QLabel{parent} {
  p_signalData = signalData;
  p_number = number;

  p_width = p_minWidth;
  p_height = p_minHeight;

  p_image = QImage();

  p_leftArray = 0;
  p_rightArray = p_signalData->samplesNumber() - 1;

  updateRelative();
}

int BaseWaveform::number() const { return p_number; }

void BaseWaveform::updateRelative() {
  p_arrayRange = p_rightArray - p_leftArray + 1;

  p_timeRange = p_arrayRange * p_signalData->timeForOne();

  p_pixelPerTime = ((p_width - (p_offsetRight + p_paddingRight)) -
                    (p_offsetLeft + p_paddingLeft)) /
                   p_timeRange;

  p_minValue =
      *std::min_element(p_signalData->data()[p_number].begin() + p_leftArray,
                        p_signalData->data()[p_number].begin() + p_rightArray);
  p_maxValue =
      *std::max_element(p_signalData->data()[p_number].begin() + p_leftArray,
                        p_signalData->data()[p_number].begin() + p_rightArray);

  p_dataRange = std::abs(p_maxValue - p_minValue);

  p_xLabelsNumber = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                     (p_offsetTop + p_paddingTop)) /
                    p_maxTextHeight;

  if (p_xLabelsNumber > p_dataRange) {
    p_xLabelsNumber =
        std::floor(p_dataRange) >= 2 ? std::floor(p_dataRange) : 2;
  }

  p_yLabelsNumber = ((p_width - (p_offsetRight + p_paddingRight)) -
                     (p_offsetLeft + p_paddingLeft)) /
                    p_maxAxisTextWidth;

  p_pixelPerData = ((p_height - (p_offsetBottom + p_paddingBottom)) -
                    (p_offsetTop + p_paddingTop)) /
                   p_dataRange;
}

bool BaseWaveform::isImageNull() const { return p_image.isNull(); }

void BaseWaveform::fill(QColor color) { p_image.fill(color.toRgb()); }

void BaseWaveform::drawGrid() {}

void BaseWaveform::drawAxes(BaseWaveform::AxisType axisType) {
  if (isImageNull()) {
    throw BaseWaveform::ImageIsNull();
  }

  std::vector<int> multiples = {
      1,         2,         5,         10,        20,       50,
      100,       200,       500,       1000,      2000,     5000,
      10000,     20000,     50000,     100000,    200000,   500000,
      1000000,   2000000,   5000000,   10000000,  20000000, 50000000,
      100000000, 200000000, 500000000, 1000000000};

  QPainter painter(&p_image);
  painter.setPen(QPen(p_mainColor, p_lineWidth));
  painter.setFont(p_font);

  QPoint axisStart;
  QPoint axisEnd;

  int p1;
  int p2;

  switch (axisType) {
    case BaseWaveform::AxisType::AxisXLeft: {
      axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
      axisEnd = {p_offsetLeft + p_paddingLeft,
                 p_height - (p_offsetBottom + p_paddingBottom)};

      p1 = axisStart.x() - 5;
      p2 = axisStart.x();

      break;
    }

    case BaseWaveform::AxisType::AxisYTop: {
      axisStart = {p_offsetLeft + p_paddingLeft, p_offsetTop + p_paddingTop};
      axisEnd = {p_width - (p_offsetRight + p_paddingRight),
                 p_offsetTop + p_paddingTop};
      break;
    }

    case BaseWaveform::AxisType::AxisYBottom: {
      axisStart = {p_offsetLeft + p_paddingLeft,
                   p_height - (p_offsetBottom + p_paddingBottom)};
      axisEnd = {p_width - (p_offsetRight + p_paddingRight),
                 p_height - (p_offsetBottom + p_paddingBottom)};
      break;
    }
  }

  if (axisType == BaseWaveform::AxisType::AxisXLeft) {
    if (p_xLabelsNumber == 2) {
      painter.drawLine(QPoint{p1, axisStart.y()}, QPoint{p2, axisStart.y()});
      painter.drawLine(QPoint{p1, axisEnd.y()}, QPoint{p2, axisEnd.y()});

      QRect textRect{
          QPoint{p1 - (p_textMarginRight + p_maxAxisTextWidth),
                 axisStart.y() -
                     (p_maxTextHeight / 2 + p_maxTextHeight % 2 + p_lineWidth)},
          QPoint{p1 - p_textMarginRight, axisStart.y() + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(p_maxValue));

      textRect.setTopLeft(QPoint{
          p1 - (p_textMarginRight + p_maxAxisTextWidth),
          axisEnd.y() -
              (p_maxTextHeight / 2 + p_maxTextHeight % 2 + p_lineWidth)});

      textRect.setBottomRight(
          QPoint{p1 - p_textMarginRight, axisEnd.y() + p_maxTextHeight / 2});

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(p_minValue));

      if (p_minValue < 0 && p_maxValue > 0) {
        int y0 =
            p_offsetTop + p_paddingTop + p_pixelPerData * std::abs(p_maxValue);

        if (y0 - axisStart.y() < p_maxTextHeight * 1.5 ||
            axisEnd.y() - y0 < p_maxTextHeight * 1.5) {
          return;
        }

        textRect.setTopLeft(QPoint{
            p1 - (p_textMarginRight + p_maxAxisTextWidth),
            y0 - (p_maxTextHeight / 2 + p_maxTextHeight % 2 + p_lineWidth)});

        textRect.setBottomRight(
            QPoint{p1 - p_textMarginRight, y0 + p_maxTextHeight / 2});

        painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter, "0");

        painter.drawLine(QPoint{p1, y0}, QPoint{p2, y0});
      }

      return;
    }

    int curDelimitersNumber = 1;
    int delimiter = multiples[0];
    for (int i = 0; i < multiples.size(); ++i) {
      curDelimitersNumber = p_dataRange / multiples[i];

      if (curDelimitersNumber <= p_xLabelsNumber) {
        delimiter = multiples[i];
        break;
      }
    }

    int startValue = std::floor(p_maxValue);
    if (p_maxValue > 0) {
      while (startValue % delimiter) --startValue;
    } else {
      while (std::abs(startValue) % delimiter) ++startValue;
    }

    int step = std::round(p_pixelPerData * delimiter);
    int startY = p_offsetTop + p_paddingTop +
                 std::abs(p_pixelPerData * (p_maxValue - startValue));
    int curValue = startValue;
    for (int i = 0; i < curDelimitersNumber; ++i) {
      int y = startY + step * i;
      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});

      QRect textRect{
          QPoint{p1 - (p_textMarginRight + p_maxAxisTextWidth),
                 y - (p_maxTextHeight / 2 + p_maxTextHeight % 2 + p_lineWidth)},
          QPoint{p1 - p_textMarginRight, y + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(curValue));

      if (i != curDelimitersNumber - 1) {
        curValue -= delimiter;
      }
    }

    if (std::abs(p_minValue - curValue) >= delimiter) {
      int y = startY + step * curDelimitersNumber;
      curValue -= delimiter;

      painter.drawLine(QPoint{p1, y}, QPoint{p2, y});
      QRect textRect{
          QPoint{p1 - (p_textMarginRight + p_maxAxisTextWidth),
                 y - (p_maxTextHeight / 2 + p_maxTextHeight % 2 + p_lineWidth)},
          QPoint{p1 - p_textMarginRight, y + p_maxTextHeight / 2}};

      painter.drawText(textRect, Qt::AlignRight | Qt::AlignVCenter,
                       QString::number(curValue));
    }
  } else if (axisType == BaseWaveform::AxisType::AxisYTop ||
             axisType == BaseWaveform::AxisType::AxisYBottom) {
  }

  painter.drawLine(axisStart, axisEnd);
}

void BaseWaveform::drawName(BaseWaveform::NameType nameType) {}

void BaseWaveform::drawBresenham() {
  if (isImageNull()) {
    throw BaseWaveform::ImageIsNull();
  }

  int localWidth = p_width - (p_offsetLeft + p_offsetRight);
  int localHeight = p_height - (p_offsetTop + p_offsetBottom);

  double scale = localHeight / (p_dataRange + 1);

  for (int i = 0; i < p_arrayRange; ++i) {
    int x1 = std::round(i * localWidth / p_arrayRange) + p_offsetLeft;
    int x2 = std::round((i + 1) * localWidth / p_arrayRange) + p_offsetLeft;
    int y1 = localHeight -
             std::floor((p_signalData->data()[p_number][i + p_rightArray] -
                         p_minValue) *
                        scale);
    int y2 = localHeight -
             std::floor((p_signalData->data()[p_number][i + p_rightArray + 1] -
                         p_minValue) *
                        scale);

    int dx = std::abs(x2 - x1);
    int dy = std::abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;

    while (true) {
      int x = x1;
      int y = localHeight - y1 + p_offsetTop;

      QRgb *line = reinterpret_cast<QRgb *>(p_image.scanLine(y));

      QRgb &pixel = line[x];
      pixel = p_graphColor.rgb();

      if (x == x2 && y == (localHeight - y2 + p_offsetTop)) {
        break;
      }

      int err2 = 2 * err;

      if (err2 > -dy) {
        err -= dy;
        x1 += sx;
      }

      if (err2 < dx) {
        err += dx;
        y1 += sy;
      }
    }
  }
}

}  // namespace fssp