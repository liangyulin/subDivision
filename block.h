#ifndef BLOCK_H
#define BLOCK_H

#include <QGLWidget>

class block : public QGLWidget
{
	Q_OBJECT

public:
	explicit block(QWidget *parent = 0);



protected:
	void initializeGL();
	void paintGL();

};

#endif // GLWIDGET_H
