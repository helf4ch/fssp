#include "graphsdialog.h"

GraphsDialog::GraphsDialog(FileReader *reader_, QWidget *parent) :
		QDialog(parent), reader(reader_) {
	setupUI();
	setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
	setMinimumSize(660, 650);
	setMaximumSize(660, 650);

	leftTime = 0;
	rightTime = reader->all_time;

	leftArray = 0;
	rightArray = reader->data_num - 1;
}

void GraphsDialog::setupUI() {
	QVBoxLayout *mainLayout = new QVBoxLayout(this);

	QMenuBar *menuBar = new QMenuBar(this);
	QMenu *menu = menuBar->addMenu(tr("Options"));
	QAction *action = menu->addAction(tr("Scale"));

connect(action, &QAction::triggered, this, &GraphsDialog::handleButtonClicked);

	mainLayout->setMenuBar(menuBar);

	scrollArea = new QScrollArea(this);
	scrollArea->setWidgetResizable(true);

	scrollContent = new QWidget(this);

	graphLayout = new QVBoxLayout(scrollContent);
	graphLayout->setAlignment(Qt::AlignTop);

	scrollArea->setWidget(scrollContent);
	scrollArea->setWidgetResizable(true);

	mainLayout->addWidget(scrollArea);

	setWindowTitle(tr("Осциллограмма"));
	setLayout(mainLayout);
}

void GraphsDialog::handleButtonClicked() {
	QPushButton *acceptScale = new QPushButton(tr("Accept"));
	QPushButton *denyScale = new QPushButton(tr("Deny"));

	scaleFromValue = new QLineEdit(QString::number(leftTime));
	scaleToValue = new QLineEdit(QString::number(rightTime));

	connect(acceptScale, &QPushButton::clicked, this, &GraphsDialog::pushAcceptButton);
	connect(denyScale, &QPushButton::clicked, this, &GraphsDialog::pushDenyButton);

	QFormLayout *formLayout = new QFormLayout(this);

	formLayout->addRow(tr("&From:"), scaleFromValue);
	formLayout->addRow(tr("&To:"), scaleToValue);

	QHBoxLayout *buttons = new QHBoxLayout();
	buttons->addWidget(acceptScale);
	buttons->addWidget(denyScale);
	formLayout->addRow(buttons);

	scaleForm = new QWidget();
	scaleForm->setLayout(formLayout);
	scaleForm->setWindowTitle(tr("Scale Graph"));
	scaleForm->show();
}

void GraphsDialog::pushDenyButton() {
	if (!scaleForm)
		return;
	scaleForm->close();
	delete scaleForm;
	scaleForm = nullptr;
}

void GraphsDialog::pushAcceptButton() {
	QTextStream ts(stdout);
	bool ok;

	double from = scaleFromValue->text().toDouble(&ok);
	if (!ok) {
		ts << "1\n";
		return;
	}

	double to = scaleToValue->text().toDouble(&ok);
	if (!ok) {
		ts << "2\n";
		return;
	}

	if (from < 0 || to < 0) {
		ts << "3\n";
		return;
	}

	if (from > reader->all_time || to > reader->all_time) {
		ts << "4\n";
		return;
	}

	if (from >= to) {
		ts << "5\n";
		return;
	}

	leftTime = from;
	rightTime = to;

	pushDenyButton();

	redrawAllGraphs();
}

void GraphsDialog::closeEvent(QCloseEvent *event) {
	CustomLabel::disableCheckedLabelsMenu();
	QDialog::closeEvent(event);
}

void GraphsDialog::onSelectionFinished(const QRect rect) {
	double timePerPixel = (rightTime - leftTime) / GRAPH_WIDTH;

	rightTime = leftTime + (rect.topRight().x() - OFFSET_START_X) * timePerPixel;
	leftTime += (rect.topLeft().x() - OFFSET_START_X) * timePerPixel;

	redrawAllGraphs();
}

void GraphsDialog::redrawAllGraphs() {
	QList<int> numbers = GraphLabel::getNumbers();

	for (int i = 0; i < numbers.size(); ++i) {
		GraphLabel::deleteGraph(numbers[i]);
	}

	int elementsPerTime = reader->data_num / reader->all_time;

	rightArray = elementsPerTime * rightTime;
	leftArray = elementsPerTime * leftTime;

	for (int i = 0; i < numbers.size(); ++i) {
		int number = numbers[i];

		QImage image(WIDTH, HEIGHT, QImage::Format_ARGB32);
		image.fill(Qt::white);
		drawGraph(reader->data[number], image, reader, number, rect);
	}
}

void GraphsDialog::drawGraph(const QVector<double> &channel, QImage &image,
		FileReader *reader, int number, QRect rect) {
	if (GraphLabel::containsGraph(number)) {
		GraphLabel::deleteGraph(number);
	}

	QPainter painter(&image);
	painter.setPen(Qt::black);

	// Значения для вычисления меток осей координат
	double minVal, maxVal, rangeY, labelStepY, rangeX = rightTime - leftTime, labelStepX;
	const int numLabelsX = 6, numLabelsY = 6;
	QVector<double> labelValuesY(numLabelsY), labelValuesX(numLabelsX);

	// Координаты для отрисовки осей x и y
	const int axisXStart = OFFSET_START_X;
	const int axisYStart = OFFSET_START_Y;
	const int axisXEnd = WIDTH - OFFSET_END_X;
	const int axisYEnd = HEIGHT - OFFSET_END_Y;

	// Вычисляем значения меток на осях координат
	labelStepX = (rightTime - leftTime) / (numLabelsX - 1);

	for (int i = 0; i < numLabelsX; ++i) {
		labelValuesX[i] = i * labelStepX + leftTime;
	}

	minVal = *std::min_element(channel.begin() + leftArray, channel.begin() + rightArray);
	maxVal = *std::max_element(channel.begin() + leftArray, channel.begin() + rightArray);

	rangeY = maxVal - minVal;
	labelStepY = rangeY / (numLabelsY - 1);

	for (int i = 0; i < numLabelsY; ++i) {
		labelValuesY[i] = i * labelStepY + minVal;
	}

	// Рисуем координатные оси x и y
	painter.drawLine(QPointF(axisXStart, axisYEnd), QPointF(axisXEnd, axisYEnd));
	painter.drawLine(QPointF(axisXStart, axisYStart), QPointF(axisXStart, axisYEnd));

	// Подписываем оси координат
	QFont font("Times New Roman", 10);
	painter.setFont(font);

	QString xLabel = "Time (s)";
	QRectF xLabelRect = QRectF(axisXStart + 300, axisYEnd + 30, 50, 20);
	painter.drawText(xLabelRect, Qt::AlignCenter, xLabel);

	QString yLabel = "Values";
	QRectF yLabelRect = QRectF(-axisYEnd + 120, axisXStart - 60, 50, 20);
	painter.save();
	painter.rotate(-90);
	painter.drawText(yLabelRect, Qt::AlignCenter, yLabel);
	painter.restore();

	// Рисуем сетку на графике
	QPen gridPen(Qt::black, 1, Qt::DotLine);
	painter.setPen(gridPen);

	double stepY = (axisYEnd - axisYStart) / (numLabelsY - 1);
	for (int i = 0; i < numLabelsY; ++i) {
		double y = axisYEnd - i * stepY;
		painter.drawLine(QPointF(axisXStart, y), QPointF(axisXEnd, y));
	}

	double stepX = (axisXEnd - axisXStart) / (numLabelsX - 1);
	for (int i = 0; i < numLabelsX; ++i) {
		double x = axisXStart + i * stepX;
		painter.drawLine(QPointF(x, axisYStart), QPointF(x, axisYEnd));
	}

	// Наносим значения на оси
	painter.setPen(Qt::black);
	for (int i = 0; i < numLabelsY; ++i) {
		double y = axisYEnd - i * stepY;
		QString label;
		if (labelValuesY[i] < 10) {
			label = QString::number(labelValuesY[i], 'f', 2);
		} else {
			label = QString::number(labelValuesY[i], 'f', 0);
		}
		QRectF labelRect = QRectF(axisXStart - 60, y - 10, 50, 20);
		painter.drawText(labelRect, Qt::AlignRight | Qt::AlignVCenter, label);
	}

	for (int i = 0; i < numLabelsX; ++i) {
		double x = axisXStart + i * stepX;
		QString label;
		if (labelValuesY[i] < 10) {
			label = QString::number(labelValuesX[i], 'f', 2);
		} else {
			label = QString::number(labelValuesX[i], 'f', 0);
		}
		QRectF labelRect = QRectF(x - 25, axisYEnd + 10, 50, 20);
		painter.drawText(labelRect, Qt::AlignHCenter | Qt::AlignTop, label);
	}

	// Рисуем график
	double scaleX = (axisXEnd - axisXStart) / rangeX;
	double scaleY = (axisYEnd - axisYStart) / rangeY;

	QPen graphPen(QColor(0, 127, 255), 1.5);
	painter.setPen(graphPen);

	QPointF lastPoint;
	for (int i = 0; i < rightArray - leftArray; ++i) {
		double x = axisXStart + i / reader->rate * scaleX;
		double y = axisYEnd - (channel[i + leftArray] - minVal) * scaleY;

		QPointF point(x, y);
		if (i == 0) {
			lastPoint = point;
			continue;
		}

		painter.drawLine(lastPoint, point);
		lastPoint = point;
	}

	GraphLabel *graphLabel = new GraphLabel(scrollContent);
	graphLabel->setNumber(number);
	graphLabel->setPixmap(QPixmap::fromImage(image));

	connect(graphLabel, &GraphLabel::selectionFinished, this, &GraphsDialog::onSelectionFinished);

	graphLayout->addWidget(graphLabel);
}
