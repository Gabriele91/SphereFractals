#include <stdafx.h>
#include <Application.h>
#include <ETime.h>
#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )
#elif defined( PLATFORM_WINDOW )
#include <WindowsApp.h>
#elif defined( PLATFORM_LINUX )
#include <LinuxApp.h>
#elif defined( PLATFORM_ANDROID )
#endif
///////////////////////
using namespace Sphere;
///////////////////////
Application *Application::appSingleton=NULL;
///////////////////////
Application::Application()
	:mainInstance(NULL)
	,screen(NULL)
	,input(NULL)
	,audio(NULL){

}
Application::~Application(){
	appSingleton=NULL;	
}

Application *Application::create(){

	DEBUG_ASSERT(!appSingleton);
	
	Math::seedRandom(GetTime());

#if defined( PLATFORM_IOS )
#elif defined( PLATFORM_OSX )
#elif defined( PLATFORM_WINDOW )
	appSingleton=new WindowsApp();
#elif defined( PLATFORM_LINUX )
	appSingleton=new LinuxApp();
#elif defined( PLATFORM_ANDROID )
#endif
	//registration delete at exit
	atexit([](){ 
		delete Application::instance(); 
	});
	//
	return appSingleton;
}

Application *Application::instance(){
	return appSingleton;
}
