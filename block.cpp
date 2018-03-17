#include "block.h"

#include <QtGui>
#include <QtOpenGL>

#include <math.h>


block::block(QWidget *parent) :
QGLWidget(parent)
{
}

void block::initializeGL()
{
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH);
}

void block::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glPushMatrix();
	glPopMatrix();
}