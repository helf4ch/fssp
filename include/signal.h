#pragma once

#include <QVBoxLayout>
#include <QWidget>

#include "filereader.h"

class Signal : public QWidget {
	Q_OBJECT
public:
	explicit Signal(QString fileName, QWidget *parent = nullptr);

	void drawWaveform(const QVector<double> &channel, QImage &image);
	void drawAxes(QImage &image, const QVector<double> &channel);

	FileReader *reader;

	QVector<QImage> images;
	const int WIDTH = 600, HEIGHT = 300;
	QVBoxLayout *layout;
};
