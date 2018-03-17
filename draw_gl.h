#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QtGui>
#include <QGLWidget>
#include <QtOpenGL>

#include <vector>
#include <fstream>
#include <sstream>
#include "halfEdge.h"
using namespace std;
#define PI 3.1415926

class GLWidget : public QGLWidget
{
	Q_OBJECT

public:
	explicit GLWidget(QWidget *parent = 0);

	void dataRepaint();

	void open(QString openFile);

	bool wrl_read(const char* fileName, vector<Vertexs>& vertexes, vector<Faces>& faces);
	bool wrl_write(const char* fileName);

	void wrlToHE(vector<Vertexs>& vertexes, vector<Faces>& faces);

	void loopSubdivision();
	void butterflySubdivision();

	bool isOnBoundary(int vtx_index);
	void boundaryNeighVertexes(int vertexIndex, vector<int>& adjVtx_ids);
	void neighborVertexes(int vertexIndex, vector<int>& adjVtx_ids);

	vector<Vertexs> vertexs_wrl;
	vector<Faces> faces_wrl;
	vector<Vertexs> vertexs_out;
	vector<Faces> faces_out;


protected:
	void initializeGL();
	void resizeGL(int width, int height);
	void paintGL();
	void mousePressEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	QPoint lastPos;
	float thetaX;
	float thetaY;
	float scale;
	float lastX;
	float lastY;
	float maxNum;

	vector<HEVertex> he_vertexs;
	vector<HEFace> he_faces;
	vector<HEHalfEdge> he_halfedges;


};

#endif // GLWIDGET_H