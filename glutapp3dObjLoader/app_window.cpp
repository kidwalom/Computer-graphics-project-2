
# include <iostream>
# include <gsim/gs.h>
# include "app_window.h"
#include <chrono>
using namespace std::chrono;
namespace {
	template <typename T>
	T lerp( const T& a, const T& b, float c) 
	{
		return a + c*(b - a);
	}
}
AppWindow::AppWindow(const char* label, int x, int y, int w, int h)
	:GlutWindow(label, x, y, w, h)
{
	initPrograms();
	addMenuEntry("Option 0", evOption0);
	addMenuEntry("Option 1", evOption1);
	_viewaxis = true;
	_fovy = GS_TORAD(60.0f);
	_rotx = _roty = _rotxnew = _rotynew =0, _rotglobalx=0;
	_w = w;
	_h = h;
	spaceships = 0.0;
	spacexx = 0.01;
	spaceyy = 0.01;
	selection = 0;
	jx = 0;
	jy = 0;
	jz = 0;
	bool run = false;
	bool joe = false;
	bool whole = true;
}

void AppWindow::initPrograms()
{
	// Init my scene objects:
	_axis.init();
		_model.init();
		_space.init();
		_floorball.init();
		_ballleft.init();
		_ballright.init();
		_world.init();
		ground.init("../stars.bmp");
		ground.build(2,60);
		_world.phong(true);
		PlaySound(TEXT("../starwars.wav"), NULL, SND_ASYNC);
	// set light:
	_light.set(GsVec(0, 10, 0), GsColor(90, 90, 90, 255), GsColor::white, GsColor::white);

	// Load demo model:
	loadModel(1);
}

static void printInfo(GsModel& m)
{
	std::cout << "V:  " << m.V.size() << "\n";
	std::cout << "F:  " << m.F.size() << "\n";
	std::cout << "N:  " << m.N.size() << "\n";
	std::cout << "M:  " << m.M.size() << "\n";
	std::cout << "Fn: " << m.Fn.size() << "\n";
	std::cout << "Fm: " << m.Fm.size() << "\n";
}

void AppWindow::loadModel(int model)
{
	float f = 0.15f;
	float f2 = 0.15f;
	float f3 = 0.15f;
	float f4 = 0.15f;
	float f5 = 15.00f;
	float f6 = 4.0f;

			f=0.20f; _file="../models/ufoballl1.obj"; 
			f2=0.15f; _file2="../models/ballfloorobj.obj"; 
			f3 = 0.10f; _file3 = "../models/ballleft.obj";
			f4 = 0.10f; _file4 = "../models/ballright.obj";
			_file5 = "../models/world.obj";
			_file6 = "../models/space.obj";
	
	std::cout << "Loading space ship " << _file << "...\n";
	if (!_gsm.load(_file))
		std::cout << "Error! you fucked up\n";
	printInfo(_gsm);
	_gsm.scale(f); // to fit our camera space
	_model.build(_gsm);

	std::cout << "Loading floor balls " << _file2 << "...\n";
	if (!_gsm2.load(_file2))
		std::cout << "Error! you fucked up\n";
	printInfo(_gsm2);
	_gsm2.scale(f2); // to fit our camera space
	_floorball.build(_gsm2);

	std::cout << "Loading ball left " << _file3 << "...\n";
	if (!_gsm3.load(_file3))
		std::cout << "Error! you fucked up\n";
	printInfo(_gsm3);
	_gsm3.scale(f3); // to fit our camera space
	_ballleft.build(_gsm3);

	std::cout << "Loading ball right " << _file4 << "...\n";
	if (!_gsm4.load(_file4))
		std::cout << "Error! you fucked up\n";
	printInfo(_gsm4);
	_gsm4.scale(f4); // to fit our camera space
	_ballright.build(_gsm4);

	std::cout << "Loading the world " << _file5 << "...\n";
	if (!_gsm5.load(_file5))
		std::cout << "Error! you fucked up\n";
	printInfo(_gsm5);
	_gsm5.scale(f5); // to fit our camera space
	_world.build(_gsm5);
	redraw();

	std::cout << "Loading space " << _file6 << "...\n";
	if (!_gsm6.load(_file6))
		std::cout << "Error! you fucked up\n";
	printInfo(_gsm6);
	_gsm6.scale(f6); // to fit our camera space
	_space.build(_gsm6);
	redraw();

}

GsMat AppWindow::shadowMatrix(float x, float y, float z) {
	float ground[4] = { 0, 1 , 0, 1 };
	float light[4] = { 0.0,10, 0 ,0 };

	float dot = ground[0] * light[0] + ground[1] * light[1] + ground[2] * light[2] + ground[3] * light[3];

	GsMat shadowMat(dot - ground[0] * light[0], 0 - ground[1] * light[0], 0 - ground[2] * light[0], 0 - ground[3] * light[0],
		0 - ground[0] * light[1], dot - ground[1] * light[1], 0 - ground[2] * light[1], 0 - ground[3] * light[1],
		0 - ground[0] * light[2], 0 - ground[1] * light[2], dot - ground[2] * light[2], 0 - ground[3] * light[2],
		0 - ground[0] * light[3], 0 - ground[1] * light[3], 0 - ground[2] * light[3], dot - ground[3] * light[3]);

	return shadowMat;
}
// mouse events are in window coordinates, but your 2D scene is in [0,1]x[0,1],
// so make here the conversion when needed
GsVec2 AppWindow::windowToScene(const GsVec2& v)
{
	// GsVec2 is a lighteweight class suitable to return by value:
	return GsVec2((2.0f*(v.x / float(_w))) - 1.0f,
		1.0f - (2.0f*(v.y / float(_h))));
}

// Called every time there is a window event
void AppWindow::glutKeyboard(unsigned char key, int x, int y)
{
	const float incr = GS_TORAD(2.5f);
	const float incf = 0.05f;
	float spaceyyy=.01;
	switch (key)
	{
	//case ' ': _viewaxis = !_viewaxis; redraw(); break;
	case ' ': cam_view = !cam_view; redraw(); break;
	case 27: exit(1); // Esc was pressed

	/*make sure that you fix all these to work for all the models loaded */


	case 'q': spaceships = spaceships + 0.01; redraw(); break;
	case 'a': spaceships = spaceships - 0.01; redraw(); break;
	case 's': spacexx = spacexx + 0.01; redraw(); break;
	case 'w': spacexx = spacexx - 0.01; redraw(); break;
	case 'e': spaceyy = spaceyy + 0.01; redraw(); break;
	case 'd': spaceyy = spaceyy - 0.01; redraw(); break;
	case 'r':move1(); whole = false; redraw(); break;//_rotxnew += incf;
	case 'f': move2(); whole = true; redraw(); break;
	case 't': move4(); joe = true; redraw(); break;
	case 'g':move5(); joe = false; redraw(); break;
	case 'y': _rotglobalx -= incf; redraw(); break;
	case 'h': _rotglobalx -= incf; redraw(); break;

		
	}

	

}

void AppWindow::glutSpecial(int key, int x, int y)
{
	bool rd = true;
	const float incr = GS_TORAD(2.5f);
	const float incf = 0.05f;
	switch (key)
	{
	case GLUT_KEY_LEFT:      _roty -= incr; break;
	case GLUT_KEY_RIGHT:     _roty += incr; break;
	case GLUT_KEY_UP:        _rotx += incr; break;
	case GLUT_KEY_DOWN:      _rotx -= incr; break;
	case GLUT_KEY_PAGE_UP:   _fovy -= incf; break;
	case GLUT_KEY_PAGE_DOWN: _fovy += incf; break;
	default: return; // return without rendering
	}
	if (rd) redraw(); // ask the window to be rendered when possible
}

void AppWindow::glutMouse(int b, int s, int x, int y)
{
	// The mouse is not used in this example.
	// Recall that a mouse click in the screen corresponds
	// to a whole line traversing the 3D scene.
}

void AppWindow::glutMotion(int x, int y)
{
}

void AppWindow::glutMenu(int m)
{
	std::cout << "Menu Event: " << m << std::endl;
}

void AppWindow::glutReshape(int w, int h)
{
	// Define that OpenGL should use the whole window for rendering
	glViewport(0, 0, w, h);
	_w = w; _h = h;
}

// here we will redraw the scene according to the current state of the application.
void AppWindow::glutDisplay()
{
	// Clear the rendering window
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// Build a cross with some lines (if not built yet):
	if (_axis.changed) // needs update
	{
		_axis.build(1.0f); // axis has radius 1.0
	}

	// Define our scene transformation:
	GsMat rx, ry, stransf, spacex, spacey, spacexxx, spaceyyy, oogabooga,jetx,jety;
	rx.rotx(_rotx);
	ry.roty(_roty);
	spacex.translation(spacexx, spaceyy, spaceships);
	jetx.translation(jx, jy, jz);
	jety.translation(jx, jy, jz);
	stransf = rx*ry;
	spacey.roty(_rotynew);// set the scene transformation matrix
	spacexxx.roty(_rotxnew);
	spaceyyy.roty(_rotglobalx);
	oogabooga.translation(0, -15, 0);
	_light.pos = stransf * GsVec(0, 10, 0);
					 // Define our projection transformation:
					 // (see demo program in gltutors-projection.7z, we are replicating the same behavior here)
	GsMat camview, persp, sproj;
	if (cam_view) {
		GsVec eye(0, 0, 2), center(0, 0, 0), up(0, 1, 0);
		camview.lookat(eye, center, up); // set our 4x4 "camera" matrix
	}
	else {
		//GsVec eye(5, static_cast <int> (spacexx) , static_cast <int> (spaceships)), center(0, static_cast <int> (spacexx), static_cast <int> (spaceships)), up(0, 1, 0);
		//camview.lookat(eye, center, up); // set our 4x4 "camera" matrix

		GsMat cameraTransform, cameraRotation;
		cameraTransform.translation(0, 1, 2);
		cameraRotation.rotx(-3.14159f / 12.0f);
		GsMat viewMatrix = stransf * spacex * cameraTransform * cameraRotation;
		camview = viewMatrix.inverse();
	}
	//shadowMatrix(0,0,0.5,0.5);
	float aspect = (float)_w / (float) _h, znear = 0.1f, zfar = 500.0f;
	persp.perspective(_fovy, aspect, znear, zfar); // set our 4x4 perspective matrix

												   // Our matrices are in "line-major" format, so vertices should be multiplied on the 
												   // right side of a matrix multiplication, therefore in the expression below camview will
												   // affect the vertex before persp, because v' = (persp*camview)*v = (persp)*(camview*v).
	sproj = persp * camview; // set final scene projection

							 //  Note however that when the shader receives a matrix it will store it in column-major 
							 //  format, what will cause our values to be transposed, and we will then have in our 
							 //  shaders vectors on the left side of a multiplication to a matrix.

							 // Draw:
	if (_viewaxis) _axis.draw(stransf, sproj);
	_model.draw(stransf *spacex, sproj, _light);
	_floorball.draw(stransf*spacex*spaceyyy, sproj, _light);
	_ballleft.draw(stransf *jetx*spacexxx, sproj, _light);
	_ballright.draw(stransf *jety*spacex*spacey, sproj, _light);
	_world.draw(stransf, sproj, _light);


	_model.draw(stransf *spacex*shadowMatrix(0.0f, 0.5f, 0.5f), sproj, _light);
	_floorball.draw(stransf*spacex*spaceyyy*shadowMatrix(0.0f, 0.5f, 0.5f), sproj, _light);
	_ballleft.draw(stransf *spacex*spacexxx*shadowMatrix(0.0f, 0.5f, 0.5f), sproj, _light);
	_ballright.draw(stransf *spacex*spacey*shadowMatrix(0.0f, 0.5f, 0.5f), sproj, _light);
	ground.draw(stransf, sproj);

	
	// Swap buffers and draw:
	glFlush();         // flush the pipeline (usually not necessary)
	glutSwapBuffers(); // we were drawing to the back buffer, now bring it to the front
}

 void AppWindow::glutIdle() 
 {
	 static high_resolution_clock::time_point prevtime = high_resolution_clock::now();
	 high_resolution_clock::time_point currentime = high_resolution_clock::now();
	 float framedelta = duration_cast<duration<float>>(currentime - prevtime).count();
	 prevtime = currentime;
	 animutime += framedelta * 6;
	 
	 // when ever animutime goes over 1 it goes to the next frame
	 // fractional component is the distance between the frames
	 if (currentanimu.size() > 0)
	 {
		 if (animutime > 1)
		 {
			 int overage = floor(animutime);
			 for (int i = 0; i < overage; i++) {
				 currentanimu.remove(0);
			 }
			 animutime -= overage;
			 if (currentanimu.size() < 2) {
				 currentanimu.size(0);
				 std::cout << "animation done" << std::endl;
				 return;
			 }
			 std::cout << "new frame" << std::endl;
			 //std::cout << " postion is" << currentanimu[0].x << " " << currentanimu[0].y << " " << currentanimu[0].z << std::endl;
			 //
			 _rotglobalx += 1;
			 _rotynew -= .1;
		 }

		 GsVec a = currentanimu[0];
		 GsVec b = currentanimu[1];
		 GsVec currentpostion = lerp(a, b, animutime);
		 //std::cout << "between " << a.x << " " << a.y << " " << a.z << " and " << b.x << " " << b.y << " " << b.z << std::endl;
		 //std::cout << animutime << " postion is" << currentpostion.x << " " << currentpostion.y << " " << currentpostion.z << std::endl;
		if (whole == false || whole == true)
		{
			 spacexx = currentpostion.x;
			 spaceyy = currentpostion.y;
			 spaceships = currentpostion.z;
			 redraw();
	
		}
 		if (joe == false || joe == true) {
		 jx = currentpostion.x;
		 jy = currentpostion.y;
		 jz = currentpostion.z;
		 redraw();
		}
	 }
	// redraw();
}
void AppWindow::move1() {
	GsArray <GsVec> cntrlpoint;
	cntrlpoint.size(3);

		for (int i = 0; i < cntrlpoint.size(); i++) {
			//figure out contorl points
			cntrlpoint[i] = GsVec((i), ((i - 1)*(i)*(i)) / 2, i);
			std::cout << "herp -> " << cntrlpoint[i].x << " " << cntrlpoint[i].y << " " << cntrlpoint[i].z << std::endl;
		};


		evaluate_bezier(cntrlpoint.size() * 5, currentanimu, cntrlpoint);


		for (int i = 0; i < currentanimu.size(); i++) {
			std::cout << "derp -> " << currentanimu[i].x << " " << currentanimu[i].y << " " << currentanimu[i].z << std::endl;
		}
	
	animutime = 0;
	spaceships = currentanimu[0].z;
	spacexx = currentanimu[0].x;
	spaceyy = currentanimu[0].y;

 }

void AppWindow::move2() {
	
	GsArray <GsVec> cntrlpoint;
	cntrlpoint.size(3);


	for (int i = 0; i < cntrlpoint.size(); i++) {
		//figure out contorl points
		cntrlpoint[i] = GsVec((i - 2), ((i - 2)*(i - 2)*(i - 2)) / 5, 0);
		std::cout << "herp -> " << cntrlpoint[i].x << " " << cntrlpoint[i].y << " " << cntrlpoint[i].z << std::endl;
	};
	evaluate_bezier(cntrlpoint.size() * 5, currentanimu, cntrlpoint);
	
	for (int i = 0; i < currentanimu.size(); i++) {
		std::cout << "derp -> " << currentanimu[i].x << " " << currentanimu[i].y << " " << currentanimu[i].z << std::endl;
	}



	animutime = 0;
	spaceships = currentanimu[0].z;
	spacexx = currentanimu[0].x;
	spaceyy = currentanimu[0].y;

}

void AppWindow::move4() {
	GsArray <GsVec> cntrlpoint;
	cntrlpoint.size(3);


	for (int i = 0; i < cntrlpoint.size(); i++) {
		//figure out contorl points
		cntrlpoint[i] = GsVec((i), ((i - 2)*(i - 2)*(i - 2)) / 5, i-1);
		std::cout << "herp -> " << cntrlpoint[i].x << " " << cntrlpoint[i].y << " " << cntrlpoint[i].z << std::endl;
	};
	evaluate_bezier(cntrlpoint.size() * 5, currentanimu, cntrlpoint);

	for (int i = 0; i < currentanimu.size(); i++) {
		std::cout << "derp -> " << currentanimu[i].x << " " << currentanimu[i].y << " " << currentanimu[i].z << std::endl;
	}



	animutime = 0;
	jx = currentanimu[0].z;
	jy = currentanimu[0].x;
	jz = currentanimu[0].y;

}

void AppWindow::move5() {
	GsArray <GsVec> cntrlpoint;
	cntrlpoint.size(3);


	for (int i = 0; i < cntrlpoint.size(); i++) {
		//figure out contorl points
		cntrlpoint[i] = GsVec((i+1), ((i - 2)*(i - 2)*(i - 2)) / 5, i-1);
		std::cout << "herp -> " << cntrlpoint[i].x << " " << cntrlpoint[i].y << " " << cntrlpoint[i].z << std::endl;
	};
	evaluate_bezier(cntrlpoint.size() * 5, currentanimu, cntrlpoint);

	for (int i = 0; i < currentanimu.size(); i++) {
		std::cout << "derp -> " << currentanimu[i].x << " " << currentanimu[i].y << " " << currentanimu[i].z << std::endl;
	}



	animutime = 0;
	jx = currentanimu[0].z;
	jy = currentanimu[0].x;
	jz = currentanimu[0].y;

}