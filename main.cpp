#include "subdivision_new.h"
#include "subdivision_new.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
	QApplication a(argc, argv);
	subDivision_new w;
	w.setFixedSize(700, 600);
	w.show();
	return a.exec();
}
