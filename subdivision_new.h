#ifndef TESTSHAPE_H
#define TESTSHAPE_H

#include <QtWidgets/QMainWindow>
#include <vector>
#include <fstream>
#include <sstream>
#include "ui_subDivision_new.h"
//#include "draw.h"
#include "draw_gl.h"
#include "block.h"
//#include "texture.h"
//#include "Vector2.h"
using namespace std;

class QAction;
class QHBoxLayout;
class QVBoxLayout;
class QGroupBox;
class QComboBox;
class QRadioButton;
class QButtonGroup;
class QPushButton;
class QLineEdit;
class QSlider;
class QLabel;

class subDivision_new : public QMainWindow
{
	Q_OBJECT

public:
	subDivision_new(QWidget *parent = 0);
	~subDivision_new();

	GLWidget *glwidget;
	block *qgl;
	//texture *tex;

	QHBoxLayout *horizonBox;
	QVBoxLayout *optionsBox;
	QVBoxLayout *drawWindowsBox;
	QHBoxLayout *drawOpenglBox;

	QWidget *widget;

	//components
	QComboBox *shapeChoice;
	QComboBox *markColorChoice;
	QComboBox *lineColorChoice;
	QButtonGroup *vectorStyle;
	QLineEdit *markSizeEdit;
	QLineEdit *lineSizeEdit;
	QPushButton *button;

	//QStringList fileNames;
	//QString openFile;

	QRadioButton *data1;
	QRadioButton *data2;

	void editData(QString openfile);

	private slots:
	void addData();
	void save();
	void refresh();
	void radioChange1();
	void radioChange2();


private:
	Ui::subdivision_newClass ui;

	int dataIndex;

};

#endif // TESTSHAPE_H
