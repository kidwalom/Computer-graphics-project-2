
// Ensure the header file is included only once in multi-file projects
#ifndef APP_WINDOW_H
#define APP_WINDOW_H

# include <gsim/gs_color.h>
# include <gsim/gs_array.h>
# include <gsim/gs_light.h>
# include <gsim/gs_vec.h>
# include "ogl_tools.h"
# include "glut_window.h"
# include "so_axis.h"
# include "so_model.h"
# include "material.h"
# include "light.h"
#include "so_textured_quad.h"


// The functionality of your application should be implemented inside AppWindow
class AppWindow : public GlutWindow
{
private:
	// OpenGL shaders and programs:
	//    GlShader _vshflat, _fshflat, _vshgou, _fshgou, _vshphong, _fshphong;
	//  GlProgram _progflat, _proggou, _progphong;

	// My scene objects:
	SoAxis _axis;
	SoModel _model;
	SoModel _space;
	SoModel _floorball;
	SoModel _ballleft;
	SoModel _ballright;
	SoModel _world;
	const char* _file; /* { "../models/untitled.obj", "../models/ball1.obj", "../models/ufoball1.obj", "../models/untitled.obj" }; */
	const char* _file2;
	const char* _file3;
	const char* _file4;
	const char* _file5;
	const char* _file6;
	light l;
	SoTexturedQuad ground;
	// Scene data:
	bool  _viewaxis;
	GsModel _gsm;
	GsModel _gsm2;
	GsModel _gsm3;
	GsModel _gsm4;
	GsModel _gsm5;
	GsModel _gsm6;
	GsLight _light;
	virtual GsMat shadowMatrix(float x, float y, float z);
	// App data:
	enum MenuEv { evOption0, evOption1 };
	float _rotx, _roty, _fovy, _rotxnew, _rotynew, _rotglobalx;
	int _w, _h;
	double spaceships;
	double spaceyy;
	double spacexx;
	int selection;
	bool cam_view;


public:
	AppWindow(const char* label, int x, int y, int w, int h);
	void initPrograms();
	void loadModel(int model);
	GsVec2 windowToScene(const GsVec2& v);

private: // functions derived from the base class
	virtual void glutMenu(int m);
	virtual void glutKeyboard(unsigned char key, int x, int y);
	virtual void glutSpecial(int key, int x, int y);
	virtual void glutMouse(int b, int s, int x, int y);
	virtual void glutMotion(int x, int y);
	virtual void glutDisplay();
	virtual void glutReshape(int w, int h);
	virtual void glutIdle();
	
};

#endif // APP_WINDOW_H
