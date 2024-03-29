#include <stdafx.h>
#include <MainInstance.h>
///////////////////////
using namespace Sphere;

MainInstance::MainInstance(const String& app,
						   unsigned int width,
						   unsigned int height,
						   unsigned int bites,
						   unsigned int freamPerSecond,
						   bool fullscreen){
Application::instance()
			 ->getScreen() 
			 ->createWindow(app.c_str(),width,height,bites,freamPerSecond,fullscreen);
}