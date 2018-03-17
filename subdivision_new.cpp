#include "subdivision_new.h"
#include <QtGui/qpixmap.h>
#include <QtWidgets/qgraphicsscene.h>
#include <QtWidgets/qgraphicsview.h>
#include <QtWidgets/qboxlayout.h>
#include <QtWidgets/qcombobox.h>
#include <QtWidgets/qpushbutton.h>
#include <QtWidgets/qradiobutton.h>
#include <QtWidgets/qgroupbox.h>
#include <QtWidgets/qmessagebox.h>
#include <QtWidgets/qlineedit.h>
#include <QtWidgets/qlabel.h>
#include <QtWidgets/qslider.h>
//#include <afxdlgs.h>
#include <QFileDialog>

subDivision_new::subDivision_new(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);

	glwidget = new GLWidget();
	qgl = new block();

	dataIndex = 0;

	//菜单项
	QAction *openAction = new QAction(tr("&addData"), this);
	openAction->setStatusTip(tr("Add a WRL"));
	connect(openAction, SIGNAL(triggered()), this, SLOT(addData()));

	QAction *openAction2 = new QAction(tr("&save"), this);
	openAction2->setStatusTip(tr("Save"));
	connect(openAction2, SIGNAL(triggered()), this, SLOT(save()));

	//添加菜单
	QMenu *file = menuBar()->addMenu(tr("&File"));
	QMenu *save = menuBar()->addMenu(tr("&Save"));

	file->addAction(openAction);
	save->addAction(openAction2);

	//布局
	horizonBox = new QHBoxLayout();
	drawWindowsBox = new QVBoxLayout();
	drawOpenglBox = new QHBoxLayout();
	optionsBox = new QVBoxLayout();

	//组件
	button = new QPushButton(tr("Subdivision"));
	button->setStatusTip(tr("Subdivision"));

	data1 = new QRadioButton(tr("Loop"));
	data2 = new QRadioButton(tr("Butterfly"));

	data1->setChecked(true);
	data1->setStatusTip(tr("Choose data_1"));
	data2->setStatusTip(tr("Choose data_2"));
	data1->setFixedSize(100, 20);
	data2->setFixedSize(100, 20);
	button->setFixedSize(100, 30);

	vectorStyle = new QButtonGroup();
	vectorStyle->addButton(data1, 0);
	vectorStyle->addButton(data2, 1);
	vectorStyle->setExclusive(true);

	//添加布局
	//drawWindowsBox->addWidget(glwidget);
	drawWindowsBox->addWidget(qgl);

	optionsBox->addWidget(data1);
	optionsBox->addWidget(data2);
	optionsBox->addWidget(button);

	//horizonBox->addLayout(drawOpenglBox);
	horizonBox->addLayout(drawWindowsBox);
	horizonBox->addLayout(optionsBox);


	widget = new QWidget();
	widget->setLayout(horizonBox);

	setCentralWidget(widget);

	connect(button, SIGNAL(clicked()), this, SLOT(refresh()));
	connect(data1, SIGNAL(clicked(bool)), this, SLOT(radioChange1()));
	connect(data2, SIGNAL(clicked(bool)), this, SLOT(radioChange2()));
}

subDivision_new::~subDivision_new()
{

}

void subDivision_new::addData(){
	QFileDialog dialog(this);
	dialog.setFileMode(QFileDialog::AnyFile);
	dialog.setNameFilter(tr("WRL File (*.wrl)"));
	dialog.setViewMode(QFileDialog::Detail);
	//QStringList fileNames;
	if (dialog.exec()){
		QStringList fileNames = dialog.selectedFiles();
		QString openFile = fileNames[0];
		QMessageBox::information(this, tr("tip"), "WRL choosed");
		editData(openFile);
	}
}

void subDivision_new::save(){
	QString fileName = QFileDialog::getSaveFileName(this,
		tr("Save as WRL"),
		"",
		tr("WRL Files (*.wrl)"));

	if (!fileName.isNull())
	{
		QByteArray ba = fileName.toLatin1();
		const char* filenameout = ba.data();
		glwidget->wrl_write(filenameout);
	}
	else
	{
		//点的是取消
	}
}

void subDivision_new::refresh(){
	if (dataIndex == 0){
		drawWindowsBox->removeWidget(qgl);
		glwidget->loopSubdivision();
		drawWindowsBox->addWidget(glwidget);
	}else if (dataIndex == 1){
		drawWindowsBox->removeWidget(qgl);
		glwidget->butterflySubdivision();
		drawWindowsBox->addWidget(glwidget);
	}

}

void subDivision_new::radioChange1(){
	//QMessageBox::information(this, tr("123"), "visbutton");
	dataIndex = 0;
}
void subDivision_new::radioChange2(){
	//QMessageBox::information(this, tr("123"), "noScaRadio");
	dataIndex = 1;
}

void subDivision_new::editData(QString openFile){
	drawWindowsBox->removeWidget(qgl);
	glwidget->vertexs_wrl.clear();
	glwidget->faces_wrl.clear();
	glwidget->dataRepaint();
	glwidget->open(openFile);
	drawWindowsBox->addWidget(glwidget);

}