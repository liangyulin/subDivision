#include <math.h>
#include <QtOpenGL>
#include <GL/GLU.h>
#include <GL/glut.h>
#include "draw_gl.h"


GLWidget::GLWidget(QWidget *parent) :QGLWidget(parent)
{
	thetaY = 0;
	thetaX = 0;
	scale = 1;
	lastX = 0;
	lastY = 0;
	maxNum = 0;
}

void GLWidget::initializeGL()
{
	// 启用阴影平滑
	glShadeModel(GL_SMOOTH);
	// 黑色背景
	glClearColor(1.0, 1.0, 1.0, 1.0);
	// 设置深度缓存
	glClearDepth(1.0);
	glEnable(GL_BLEND);
	// 启用深度测试
	glEnable(GL_DEPTH_TEST);
	// 所作深度测试的类型
	glDepthFunc(GL_LEQUAL);
	// 告诉系统对透视进行修正
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void GLWidget::paintGL()
{
	// 清除屏幕和深度缓存
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	//glViewport(0, 0, 300, 300);//注意，后面这两个参数是高度和宽度，而不是坐标  
	glLoadIdentity();
	//坐标转移
	//glTranslatef(-1.5f, 0.0f, -6.0f);
	//设置颜色
	glColor3f(0.0, 0.0, 0.0);
	glLineWidth(1);
	
	glRotatef(thetaX, 0, 1, 0);
	glRotatef(thetaY, 1, 0, 0);
	glScalef(scale, scale, scale);


	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	glBegin(GL_TRIANGLES);
	for (int i = 0; i<he_faces.size(); i++){

		glVertex3f(he_vertexs.at(he_halfedges.at(he_faces.at(i).edge).origin).x, he_vertexs.at(he_halfedges.at(he_faces.at(i).edge).origin).y, he_vertexs.at(he_halfedges.at(he_faces.at(i).edge).origin).z);
		glVertex3f(he_vertexs.at(he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).next).origin).x, he_vertexs.at(he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).next).origin).y, he_vertexs.at(he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).next).origin).z);
		glVertex3f(he_vertexs.at(he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).prev).origin).x, he_vertexs.at(he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).prev).origin).y, he_vertexs.at(he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).prev).origin).z);
	}
	glEnd();

}

// 重置OpenGL窗口大小
void GLWidget::resizeGL(int width, int height)
{
	// 防止窗口大小变为0
	if (height == 0)
	{
		height = 1;
	}
	// 重置当前的视口
	glViewport(0, 0, (GLint)width, (GLint)height);
	// 选择投影矩阵
	glMatrixMode(GL_PROJECTION);
	// 重置投影矩阵
	glLoadIdentity();

	float rad = 1.6*maxNum;
	//qDebug() << maxNum;
	glOrtho(-rad, rad, -rad, rad, -rad, rad);

	// 选择模型观察矩阵
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//开启反走样  
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
	glEnable(GL_POLYGON_SMOOTH);
	glEnable(GL_POINT_SMOOTH);
	glEnable(GL_LINE_SMOOTH);
}

void GLWidget::mousePressEvent(QMouseEvent *event)
{
	lastPos = event->pos();

}

void GLWidget::mouseMoveEvent(QMouseEvent *event)
{
	float dx = lastPos.x() - event->x();
	float dy = lastPos.y() - event->y();
	//qDebug() << dx;
	//qDebug() << dy;

	thetaX = lastX + dx;
	thetaY = lastY + dy;

	//lastPos = event->pos();
	updateGL();
}

void GLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	//lastPos = event->pos();
	lastX = thetaX;
	lastY = thetaY;
}

void GLWidget::wheelEvent(QWheelEvent *event)
{
	if (event->delta() > 0)
	{
		scale += 0.1;
	}
	else if (event->delta() < 0)
	{
		scale -= 0.1;
	}

	updateGL();
}

//数据清空
void GLWidget::dataRepaint()
{
	he_vertexs.clear();
	he_faces.clear();
	he_halfedges.clear();

	vertexs_out.clear();
	faces_out.clear();

}

//打开文件
void GLWidget::open(QString openFile){
	//读入数据
	std::string ba = openFile.toStdString();
	const char* conf = ba.c_str();
	//const char *filename = "C:\\Users\\Bin\\Desktop\\\\data\\test.wrl";
	//qDebug() << conf;
	wrl_read(conf, vertexs_wrl, faces_wrl);

	//获取最大值
	maxNum = 0;
	for (int i = 0; i<vertexs_wrl.size(); i++){
		float maxTem = max(max(abs(vertexs_wrl.at(i).x), abs(vertexs_wrl.at(i).y)), abs(vertexs_wrl.at(i).z));
		if (maxNum<maxTem){
			maxNum = maxTem;
			}
	}

	vertexs_out = vertexs_wrl;
	faces_out = faces_wrl;

	//转换到HE结构
	wrlToHE(vertexs_wrl, faces_wrl);

	updateGL();
	resizeGL(600,600);
}

//读入wrl文件
bool GLWidget::wrl_read(const char* fileName, vector<Vertexs>& vertexs_wrl, vector<Faces>& faces_wrl){

	char buffer[256];
	ifstream input(fileName);

	if (!input.is_open())
	{
		//cout << "Error reading file";
		return false;
		//exit(1);
	}

	while (!input.eof())
	{
		input.getline(buffer, 100);

		if (strstr(buffer, "point") != NULL){
			break;
		}
	}

	while (!input.eof())
	{
		input.getline(buffer, 100);
		float vx, vy, vz;
		if (strstr(buffer, "]") == NULL){
			sscanf_s(buffer, "%f %f %f", &vx, &vy, &vz);
			//cout << vx << "," << vy << "," << vz << endl;
			Vertexs vertex_tem;
			vertex_tem.x = vx;
			vertex_tem.y = vy;
			vertex_tem.z = vz;
			vertexs_wrl.push_back(vertex_tem);
			//cout << vertex_tem.x << endl;
		}
		else{
			break;
		}
	}

	while (!input.eof())
	{
		input.getline(buffer, 100);
		int v0, v1, v2;
		if (strstr(buffer, "-1") != NULL){
			sscanf_s(buffer, "%d %d %d -1", &v0, &v1, &v2);
			Faces face_tem;
			face_tem.vertex0 = v0;
			face_tem.vertex1 = v1;
			face_tem.vertex2 = v2;
			faces_wrl.push_back(face_tem);
		}
	}

	input.close();


	return true;
}

//写出wrl文件
bool GLWidget::wrl_write(const char* fileName){

	ofstream output(fileName);

	if (!output.is_open()) {
		return false;
		exit(1);
	}

	output << "#VRML V2.0 utf8 (Converted to ASCII)\n"
		<< "Shape {\n"
		<< "geometry IndexedFaceSet {\n"
		<< "coord Coordinate {\n"
		<< "point [\n";

	//..
	for (int i = 0; i<vertexs_out.size(); i++){
		output << vertexs_out.at(i).x << " " << vertexs_out.at(i).y << " " << vertexs_out.at(i).z << std::endl;
	}

	output << "]\n"
		<< "}\n"
		<< "coordIndex [\n";

	//..
	for (int i = 0; i<faces_out.size(); i++){
		output << faces_out.at(i).vertex0 << " " << faces_out.at(i).vertex0 << " " << faces_out.at(i).vertex0 << " " << -1 << std::endl;
	}

	output << "]\n"
		<< "}\n"
		<< "}\n";

	output.close();

	return true;

}

//wrl文件转换halfedge结构
void GLWidget::wrlToHE(vector<Vertexs>& vertexes, vector<Faces>& faces){
	//half edge转换
	for (int i = 0; i < faces.size(); i++){
		HEHalfEdge heHalfEdge0, heHalfEdge1, heHalfEdge2;

		heHalfEdge0.origin = faces.at(i).vertex0;
		heHalfEdge0.twin = -1;
		heHalfEdge0.incidentFace = i;
		heHalfEdge0.next = 3 * i + 1;
		heHalfEdge0.prev = 3 * i + 2;

		heHalfEdge1.origin = faces.at(i).vertex1;
		heHalfEdge1.twin = -1;
		heHalfEdge1.incidentFace = i;
		heHalfEdge1.next = 3 * i + 2;
		heHalfEdge1.prev = 3 * i;

		heHalfEdge2.origin = faces.at(i).vertex2;
		heHalfEdge2.twin = -1;
		heHalfEdge2.incidentFace = i;
		heHalfEdge2.next = 3 * i;
		heHalfEdge2.prev = 3 * i + 1;

		he_halfedges.push_back(heHalfEdge0);
		he_halfedges.push_back(heHalfEdge1);
		he_halfedges.push_back(heHalfEdge2);
	}

	//寻找twin
	for (int i = 0; i<3 * faces.size(); i++){
		int next_edge_id = he_halfedges.at(i).next;
		int v0 = he_halfedges.at(i).origin;
		int v1 = he_halfedges.at(next_edge_id).origin;

		for (int j = 0; j<3 * faces.size(); j++){
			int next_twin = he_halfedges.at(j).next;

			if (he_halfedges.at(j).origin == v1 &&he_halfedges.at(next_twin).origin == v0){
				he_halfedges.at(i).twin = j;
				break;
			}
		}
	}

	//vertex转换
	for (int i = 0; i<vertexes.size(); i++){
		for (int j = 0; j<3 * faces.size(); j++){
			if (he_halfedges.at(j).origin == i){
				HEVertex hevtx;
				hevtx.x = vertexes.at(i).x;
				hevtx.y = vertexes.at(i).y;
				hevtx.z = vertexes.at(i).z;
				hevtx.incidentEdge = j;
				he_vertexs.push_back(hevtx);
				break;
			}
		}
	}

	//face转换
	for (int i = 0; i<faces.size(); i++){
		HEFace heFace;
		heFace.edge = 3 * i;
		he_faces.push_back(heFace);
	}
}

//Loop
void GLWidget::loopSubdivision(){
	vector<Vertexs> new_vertexes;
	vector<Faces> new_faces;
	vector<Vertexs> old_vertexes;

	//old vertexs赋值
	for (int i = 0; i<he_vertexs.size(); i++){
		Vertexs vtx = { he_vertexs.at(i).x, he_vertexs.at(i).y, he_vertexs.at(i).z };
		old_vertexes.push_back(vtx);
	}

	//even vertexes
	for (int i = 0; i<he_vertexs.size(); i++){
		if (isOnBoundary(i)){
			Vertexs new_vtx;
			vector<int> neighbor_vtx;
			boundaryNeighVertexes(i, neighbor_vtx);

			new_vtx.x = (old_vertexes.at(neighbor_vtx[0]).x + old_vertexes.at(neighbor_vtx[1]).x) / 8 + 0.75 * (old_vertexes.at(i).x);
			new_vtx.y = (old_vertexes.at(neighbor_vtx[0]).y + old_vertexes.at(neighbor_vtx[1]).y) / 8 + 0.75 * (old_vertexes.at(i).y);
			new_vtx.z = (old_vertexes.at(neighbor_vtx[0]).z + old_vertexes.at(neighbor_vtx[1]).z) / 8 + 0.75 * (old_vertexes.at(i).z);
			new_vertexes.push_back(new_vtx);
		}
		else{
			Vertexs new_vtx;
			vector<int> neighbor_vtx;
			neighborVertexes(i, neighbor_vtx);

			if (neighbor_vtx.size() == 3){
				new_vtx.x = (3 * (old_vertexes.at(neighbor_vtx[0]).x + old_vertexes.at(neighbor_vtx[1]).x + old_vertexes.at(neighbor_vtx[2]).x) + 7 * old_vertexes.at(i).x) / 16;
				new_vtx.y = (3 * (old_vertexes.at(neighbor_vtx[0]).y + old_vertexes.at(neighbor_vtx[1]).y + old_vertexes.at(neighbor_vtx[2]).y) + 7 * old_vertexes.at(i).y) / 16;
				new_vtx.z = (3 * (old_vertexes.at(neighbor_vtx[0]).z + old_vertexes.at(neighbor_vtx[1]).z + old_vertexes.at(neighbor_vtx[2]).z) + 7 * old_vertexes.at(i).z) / 16;
				new_vertexes.push_back(new_vtx);
			}
			else if (neighbor_vtx.size()>3){
				float beta = 3 / (8*neighbor_vtx.size());
				float x_tem = 0;
				float y_tem = 0;
				float z_tem = 0;

				for (int j = 0; j<neighbor_vtx.size(); j++){
					x_tem += beta*old_vertexes.at(neighbor_vtx[j]).x;
					y_tem += beta*old_vertexes.at(neighbor_vtx[j]).y;
					z_tem += beta*old_vertexes.at(neighbor_vtx[j]).z;

				}
				new_vtx.x = x_tem + 5 * old_vertexes.at(i).x / 8;
				new_vtx.y = y_tem + 5 * old_vertexes.at(i).y / 8;
				new_vtx.z = z_tem + 5 * old_vertexes.at(i).z / 8;
				new_vertexes.push_back(new_vtx);
			}
		}
	}

	//odd vertexes
	for (int i = 0; i < he_halfedges.size(); i++){
		if (he_halfedges.at(i).twin == -1){
			Vertexs new_vtx;
			new_vtx.x = (old_vertexes.at(he_halfedges.at(i).origin).x + old_vertexes.at(he_halfedges.at(he_halfedges.at(i).next).origin).x) / 2;
			new_vtx.y = (old_vertexes.at(he_halfedges.at(i).origin).y + old_vertexes.at(he_halfedges.at(he_halfedges.at(i).next).origin).y) / 2;
			new_vtx.z = (old_vertexes.at(he_halfedges.at(i).origin).z + old_vertexes.at(he_halfedges.at(he_halfedges.at(i).next).origin).z) / 2;
			new_vertexes.push_back(new_vtx);
		}
		else{
			Vertexs new_vtx;
			new_vtx.x = 3 * (old_vertexes.at(he_halfedges.at(i).origin).x + old_vertexes.at(he_halfedges.at(he_halfedges.at(i).next).origin).x) / 8 + ((he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).x) + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).x) / 8;
			new_vtx.y = 3 * (old_vertexes.at(he_halfedges.at(i).origin).y + old_vertexes.at(he_halfedges.at(he_halfedges.at(i).next).origin).y) / 8 + ((he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).y) + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).y) / 8;
			new_vtx.z = 3 * (old_vertexes.at(he_halfedges.at(i).origin).z + old_vertexes.at(he_halfedges.at(he_halfedges.at(i).next).origin).z) / 8 + ((he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).z) + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).z) / 8;
			new_vertexes.push_back(new_vtx);
		}
	}

	//生成faces
	for (int i = 0; i < he_faces.size(); i++){
		int newVertex0 = he_halfedges.at(he_faces.at(i).edge).origin;
		int newVertex1 = he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).next).origin;
		int newVertex2 = he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).prev).origin;

		int index0 = old_vertexes.size() + he_faces.at(i).edge;
		int index1 = old_vertexes.size() + he_halfedges.at(he_faces.at(i).edge).next;
		int index2 = old_vertexes.size() + he_halfedges.at(he_faces.at(i).edge).prev;

		Faces face0 = { newVertex0, index0, index2 };
		Faces face1 = { index0, newVertex1, index1 };
		Faces face2 = { index0, index1, index2 };
		Faces face3 = { index2, index1, newVertex2 };

		new_faces.push_back(face0);
		new_faces.push_back(face1);
		new_faces.push_back(face2);
		new_faces.push_back(face3);
	}


	dataRepaint();

	vertexs_out = new_vertexes;
	faces_out = new_faces;

	wrlToHE(new_vertexes, new_faces);

	updateGL();
}

//Butterfly
void GLWidget::butterflySubdivision(){
	vector<Vertexs> new_vertexes;
	vector<Faces> new_faces;

	//old vertexs赋值
	for (int i = 0; i<he_vertexs.size(); i++){
		Vertexs vtx = { he_vertexs.at(i).x, he_vertexs.at(i).y, he_vertexs.at(i).z };
		new_vertexes.push_back(vtx);
	}

	//插入新顶点
	for (int i = 0; i < he_halfedges.size(); i++){
		int vtx0 = he_halfedges.at(i).origin;
		int vtx1 = he_halfedges.at(he_halfedges.at(i).next).origin;
		vector<int> neighbor_vtx0;
		vector<int> neighbor_vtx1;
		if (he_halfedges.at(i).twin == -1){
			int v_tem1, v_tem2;
			Vertexs new_vtx;
			boundaryNeighVertexes(vtx0, neighbor_vtx0);
			boundaryNeighVertexes(vtx1, neighbor_vtx1);
			for (int k = 0; k<neighbor_vtx0.size(); k++){
				if (neighbor_vtx0.at(k) != vtx1){
					v_tem1 = neighbor_vtx0.at(k);
					break;
				}
			}
			for (int k = 0; k<neighbor_vtx1.size(); k++){
				if (neighbor_vtx1.at(k) != vtx0){
					v_tem2 = neighbor_vtx1.at(k);
					break;
				}
			}

			new_vtx.x = 9 * (he_vertexs.at(vtx0).x + he_vertexs.at(vtx1).x) / 16 - (he_vertexs.at(v_tem1).x + he_vertexs.at(v_tem2).x) / 16;
			new_vtx.y = 9 * (he_vertexs.at(vtx0).y + he_vertexs.at(vtx1).y) / 16 - (he_vertexs.at(v_tem1).y + he_vertexs.at(v_tem2).y) / 16;
			new_vtx.z = 9 * (he_vertexs.at(vtx0).z + he_vertexs.at(vtx1).z) / 16 - (he_vertexs.at(v_tem1).z + he_vertexs.at(v_tem2).z) / 16;
			new_vertexes.push_back(new_vtx);

		}
		else{
			neighborVertexes(vtx0, neighbor_vtx0);
			neighborVertexes(vtx1, neighbor_vtx1);

			if (neighbor_vtx0.size() == 6 && neighbor_vtx1.size() == 6)
			{
				Vertexs new_vtx;
				new_vtx.x = (he_vertexs.at(vtx0).x + he_vertexs.at(vtx1).x) / 2 + ((he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).x) + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).x) / 8 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).x + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).prev).twin).prev).origin).x + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).twin).prev).origin).x + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).next).twin).prev).origin).x) / 16;
				new_vtx.y = (he_vertexs.at(vtx0).y + he_vertexs.at(vtx1).y) / 2 + ((he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).y) + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).y) / 8 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).y + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).prev).twin).prev).origin).y + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).twin).prev).origin).y + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).next).twin).prev).origin).y) / 16;
				new_vtx.z = (he_vertexs.at(vtx0).z + he_vertexs.at(vtx1).z) / 2 + ((he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).z) + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).z) / 8 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).z + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).prev).twin).prev).origin).z + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).twin).prev).origin).z + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).next).twin).prev).origin).z) / 16;
				new_vertexes.push_back(new_vtx);
			}
			else if (neighbor_vtx0.size() == 6 && neighbor_vtx1.size() != 6)
			{
				if (neighbor_vtx1.size() == 3){
					Vertexs new_vtx;
					new_vtx.x = 0.75 * he_vertexs.at(vtx1).x + 5 * he_vertexs.at(vtx0).x / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).x + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).x) / 12.0;
					new_vtx.y = 0.75 * he_vertexs.at(vtx1).y + 5 * he_vertexs.at(vtx0).y / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).y + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).y) / 12.0;
					new_vtx.z = 0.75 * he_vertexs.at(vtx1).z + 5 * he_vertexs.at(vtx0).z / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).z + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).z) / 12.0;

					new_vertexes.push_back(new_vtx);
				}
				else if (neighbor_vtx1.size() == 4){
					Vertexs new_vtx;
					new_vtx.x = 0.75 * he_vertexs.at(vtx1).x + 3 * he_vertexs.at(vtx0).x / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).x / 8;
					new_vtx.y = 0.75 * he_vertexs.at(vtx1).y + 3 * he_vertexs.at(vtx0).y / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).y / 8;
					new_vtx.z = 0.75 * he_vertexs.at(vtx1).z + 3 * he_vertexs.at(vtx0).z / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).z / 8;

					new_vertexes.push_back(new_vtx);
				}
				else if (neighbor_vtx1.size() >= 5){
					int n = neighbor_vtx1.size();
					float tem_x = 0.75 * he_vertexs.at(vtx1).x;
					float tem_y = 0.75 * he_vertexs.at(vtx1).y;
					float tem_z = 0.75 * he_vertexs.at(vtx1).z;

					int edge_tem_id = i;

					for (int j = 0; j<n; j++){
						float beta = (0.25 + cos((2 * PI * j) / n) + 0.5 * cos((4 * PI * j) / n)) / n;

						tem_x += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).x*beta;
						tem_y += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).y*beta;
						tem_z += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).z*beta;

						edge_tem_id = he_halfedges.at(he_halfedges.at(edge_tem_id).next).twin;
					}

					Vertexs new_vtx;
					new_vtx.x = tem_x;
					new_vtx.y = tem_y;
					new_vtx.z = tem_z;

					new_vertexes.push_back(new_vtx);
				}
			}
			else if (neighbor_vtx0.size() != 6 && neighbor_vtx1.size() == 6)
			{
				if (neighbor_vtx0.size() == 3){
					Vertexs new_vtx;
					new_vtx.x = 0.75 * he_vertexs.at(vtx0).x + 5 * he_vertexs.at(vtx1).x / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).x + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).x) / 12;
					new_vtx.y = 0.75 * he_vertexs.at(vtx0).y + 5 * he_vertexs.at(vtx1).y / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).y + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).y) / 12;
					new_vtx.z = 0.75 * he_vertexs.at(vtx0).z + 5 * he_vertexs.at(vtx1).z / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).z + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).z) / 12;

					new_vertexes.push_back(new_vtx);
				}
				else if (neighbor_vtx0.size() == 4){
					Vertexs new_vtx;
					new_vtx.x = 0.75 * he_vertexs.at(vtx0).x + 3 * he_vertexs.at(vtx1).x / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).x / 8;
					new_vtx.y = 0.75 * he_vertexs.at(vtx0).y + 3 * he_vertexs.at(vtx1).y / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).y / 8;
					new_vtx.z = 0.75 * he_vertexs.at(vtx0).z + 3 * he_vertexs.at(vtx1).z / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).z / 8;

					new_vertexes.push_back(new_vtx);
				}
				else if (neighbor_vtx0.size() >= 5){
					int n = neighbor_vtx0.size();
					float tem_x = 0.75 * he_vertexs.at(vtx0).x;
					float tem_y = 0.75 * he_vertexs.at(vtx0).y;
					float tem_z = 0.75 * he_vertexs.at(vtx0).z;

					int edge_tem_id = i;

					for (int j = 0; j<n; j++){
						float beta = (0.25 + cos((2 * PI * j) / n) + 0.5 * cos((4 * PI * j) / n)) / n;

						tem_x += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).x*beta;
						tem_y += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).y*beta;
						tem_z += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).z*beta;

						edge_tem_id = he_halfedges.at(he_halfedges.at(edge_tem_id).next).twin;
					}

					Vertexs new_vtx;
					new_vtx.x = tem_x;
					new_vtx.y = tem_y;
					new_vtx.z = tem_z;

					new_vertexes.push_back(new_vtx);
				}
			}
			else if (neighbor_vtx0.size() != 6 && neighbor_vtx1.size() != 6)
			{
				float add_x0 = 0;
				float add_y0 = 0;
				float add_z0 = 0;

				if (neighbor_vtx0.size() == 3){
					add_x0 = 0.75 * he_vertexs.at(vtx0).x + 5 * he_vertexs.at(vtx1).x / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).x + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).x) / 12;
					add_y0 = 0.75 * he_vertexs.at(vtx0).y + 5 * he_vertexs.at(vtx1).y / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).y + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).y) / 12;
					add_z0 = 0.75 * he_vertexs.at(vtx0).z + 5 * he_vertexs.at(vtx1).z / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).z + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).z) / 12;
				}
				else if (neighbor_vtx0.size() == 4){
					add_x0 = 0.75 * he_vertexs.at(vtx0).x + 3 * he_vertexs.at(vtx1).x / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).x / 8;
					add_y0 = 0.75 * he_vertexs.at(vtx0).y + 3 * he_vertexs.at(vtx1).y / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).y / 8;
					add_z0 = 0.75 * he_vertexs.at(vtx0).z + 3 * he_vertexs.at(vtx1).z / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).z / 8;
				}
				else if (neighbor_vtx0.size() >= 5){
					int n = neighbor_vtx0.size();
					float tem_x = 0.75 * he_vertexs.at(vtx0).x;
					float tem_y = 0.75 * he_vertexs.at(vtx0).y;
					float tem_z = 0.75 * he_vertexs.at(vtx0).z;

					int edge_tem_id = i;

					for (int j = 0; j<n; j++){
						float beta = (0.25 + cos((2 * PI * j) / n) + 0.5 * cos((4 * PI * j) / n)) / n;

						tem_x += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).x*beta;
						tem_y += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).y*beta;
						tem_z += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).z*beta;

						edge_tem_id = he_halfedges.at(he_halfedges.at(edge_tem_id).next).twin;
					}

					add_x0 = tem_x;
					add_y0 = tem_y;
					add_z0 = tem_z;
				}

				float add_x1 = 0;
				float add_y1 = 0;
				float add_z1 = 0;

				if (neighbor_vtx1.size() == 3){
					add_x1 = 0.75 * he_vertexs.at(vtx1).x + 5 * he_vertexs.at(vtx0).x / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).x + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).x) / 12;
					add_y1 = 0.75 * he_vertexs.at(vtx1).y + 5 * he_vertexs.at(vtx0).y / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).y + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).y) / 12;
					add_z1 = 0.75 * he_vertexs.at(vtx1).z + 5 * he_vertexs.at(vtx0).z / 12 - (he_vertexs.at(he_halfedges.at(he_halfedges.at(i).prev).origin).z + he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).twin).prev).origin).z) / 12;
				}
				else if (neighbor_vtx1.size() == 4){
					add_x1 = 0.75 * he_vertexs.at(vtx1).x + 3 * he_vertexs.at(vtx0).x / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).x / 8;
					add_y1 = 0.75 * he_vertexs.at(vtx1).y + 3 * he_vertexs.at(vtx0).y / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).y / 8;
					add_z1 = 0.75 * he_vertexs.at(vtx1).z + 3 * he_vertexs.at(vtx0).z / 8 - he_vertexs.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(he_halfedges.at(i).next).twin).prev).origin).z / 8;
				}
				else if (neighbor_vtx1.size() >= 5){
					int n = neighbor_vtx1.size();
					float tem_x = 0.75 * he_vertexs.at(vtx0).x;
					float tem_y = 0.75 * he_vertexs.at(vtx0).y;
					float tem_z = 0.75 * he_vertexs.at(vtx0).z;

					int edge_tem_id = i;

					for (int j = 0; j<n; j++){
						float beta = (0.25 + cos((2 * PI * j) / n) + 0.5 * cos((4 * PI * j) / n)) / n;

						tem_x += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).x*beta;
						tem_y += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).y*beta;
						tem_z += he_vertexs.at(he_halfedges.at(edge_tem_id).origin).z*beta;

						edge_tem_id = he_halfedges.at(he_halfedges.at(edge_tem_id).next).twin;
					}

					add_x1 = tem_x;
					add_y1 = tem_y;
					add_z1 = tem_z;
				}

				Vertexs new_vtx;
				new_vtx.x = (add_x0 + add_x1) / 2;
				new_vtx.y = (add_y0 + add_y1) / 2;
				new_vtx.z = (add_z0 + add_z1) / 2;

				new_vertexes.push_back(new_vtx);
			}

		}
	}

	//生成faces
	for (int i = 0; i < he_faces.size(); i++){
		int newVertex0 = he_halfedges.at(he_faces.at(i).edge).origin;
		int newVertex1 = he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).next).origin;
		int newVertex2 = he_halfedges.at(he_halfedges.at(he_faces.at(i).edge).prev).origin;

		int index0 = he_vertexs.size() + he_faces.at(i).edge;
		int index1 = he_vertexs.size() + he_halfedges.at(he_faces.at(i).edge).next;
		int index2 = he_vertexs.size() + he_halfedges.at(he_faces.at(i).edge).prev;

		Faces face0 = { newVertex0, index0, index2 };
		Faces face1 = { index0, newVertex1, index1 };
		Faces face2 = { index0, index1, index2 };
		Faces face3 = { index2, index1, newVertex2 };

		new_faces.push_back(face0);
		new_faces.push_back(face1);
		new_faces.push_back(face2);
		new_faces.push_back(face3);
	}


	dataRepaint();

	vertexs_out = new_vertexes;
	faces_out = new_faces;

	wrlToHE(new_vertexes, new_faces);

	updateGL();
}

//判断点是否在边界
bool GLWidget::isOnBoundary(int vtx_id){
	for (int i = 0; i<he_halfedges.size(); i++){
		if (he_halfedges.at(i).origin == vtx_id&&he_halfedges.at(i).twin == -1){
			return true;
		}
	}
	return false;
}

//获取点的边界点
void GLWidget::boundaryNeighVertexes(int vertexIndex, vector<int>& neighbor_vtx){
	for (int i = 0; i<he_halfedges.size(); i++){
		int vtx_id = he_halfedges.at(i).origin;
		int next_edge_id = he_halfedges.at(i).next;
		int next_vtx_id = he_halfedges.at(next_edge_id).origin;

		if (vtx_id == vertexIndex&&he_halfedges.at(i).twin == -1){
			neighbor_vtx.push_back(next_vtx_id);
		}
		else if (next_vtx_id == vertexIndex&&he_halfedges.at(i).twin == -1){
			neighbor_vtx.push_back(vtx_id);
		}
	}
}

//获取点的相邻点
void GLWidget::neighborVertexes(int vertexIndex, vector<int>& neighbor_vtx){
	if (!isOnBoundary(vertexIndex)){
		for (int i = 0; i<he_halfedges.size(); i++){
			if (he_halfedges.at(i).origin == vertexIndex){
				int next_edge_id = he_halfedges.at(i).next;
				int next_vtx_id = he_halfedges.at(next_edge_id).origin;
				neighbor_vtx.push_back(next_vtx_id);
			}
		}
	}
	else{
		for (int i = 0; i<he_halfedges.size(); i++){

			int next_edge_id = he_halfedges.at(i).next;
			int next_vtx_id = he_halfedges.at(next_edge_id).origin;

			if (he_halfedges.at(i).origin == vertexIndex){
				neighbor_vtx.push_back(next_vtx_id);
			}
			else if (next_vtx_id == vertexIndex&&isOnBoundary(he_halfedges.at(i).origin)){
				neighbor_vtx.push_back(he_halfedges.at(i).origin);
			}
		}
	}
}
