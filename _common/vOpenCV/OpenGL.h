#ifndef _GL_HELPER_
#define _GL_HELPER_

#include <gl/GL.h>
#include <opencv2/core/opengl_interop.hpp>
#include <string>

using std::string;

#pragma comment(lib,"opengl32.lib")

#ifdef _DEBUG
#pragma comment(lib,"opencv_core232d.lib")
#pragma comment(lib,"opencv_highgui232d.lib")
#else
#pragma comment(lib,"opencv_core232.lib")
#pragma comment(lib,"opencv_highgui232.lib")
#endif

namespace cv
{
class I3DRenderer
{
public:
	I3DRenderer(const string& title, int w=800,int h = 600):_window_name(title)
	{ 
		namedWindow(title, WINDOW_OPENGL);
		resizeWindow(title, w, h);
		setOpenGlDrawCallback(title, I3DRenderer::static_draw_cb, this);
		setMouseCallback(title, I3DRenderer::static_mouse_cb, this);

		_near = 1.0f;
		_far = 1000.0f;
		_fov = 45.0f;
	}

	virtual ~I3DRenderer()
	{
		destroyWindow(_window_name);
	}
// 	enum
// 	{
// 		EVENT_MOUSEMOVE      =0,
// 		EVENT_LBUTTONDOWN    =1,
// 		EVENT_RBUTTONDOWN    =2,
// 		EVENT_MBUTTONDOWN    =3,
// 		EVENT_LBUTTONUP      =4,
// 		EVENT_RBUTTONUP      =5,
// 		EVENT_MBUTTONUP      =6,
// 		EVENT_LBUTTONDBLCLK  =7,
// 		EVENT_RBUTTONDBLCLK  =8,
// 		EVENT_MBUTTONDBLCLK  =9
// 	};
	virtual void onMouseEvent(int event, int x, int y, int flags){}
	virtual void draw() = 0;

	void update()
	{
		updateWindow(_window_name);
	}

	static void static_mouse_cb(int event, int x, int y, int flags, void* userdata)
	{
		I3DRenderer* renderer = static_cast<I3DRenderer*>(userdata);
		renderer->_calculate_mouse_dxdy(event, x, y, flags);
		renderer->onMouseEvent(event, x, y, flags);
	}

	static void static_draw_cb(void* userdata)
	{
		I3DRenderer* renderer = static_cast<I3DRenderer*>(userdata);
		renderer->_update_camera_projection();
		renderer->draw();
	}
private:

	void _update_camera_projection()
	{
		double aspect = getWindowProperty(_window_name, WND_PROP_ASPECT_RATIO);
		_camera.setPerspectiveProjection(_fov, aspect, _near, _far);
	}

	void _calculate_mouse_dxdy(int event, int x, int y, int flags)
	{
		static int oldx = x;
		static int oldy = y;
		static bool moving = false;

		if (event == EVENT_LBUTTONDOWN)
		{
			oldx = x;
			oldy = y;
			moving = true;
		}
		else if (event == EVENT_LBUTTONUP)
		{
			moving = false;
		}

		if (moving)
		{
			_mouse_dx = oldx - x;
			_mouse_dy = oldy - y;
		}
		else
		{
			_mouse_dx = 0;
			_mouse_dy = 0;
		}
	}
protected:
	float _near;
	float _far;
	float _fov;
	GlCamera _camera;
	string _window_name;
	int _mouse_dx;
	int _mouse_dy;
};
}
#endif