#include <LinuxScreen.h>
///////////////////////
using namespace Sphere;
///////////////////////

#define WINDOW_ATTRIBUTE ExposureMask | KeyPressMask | KeyReleaseMask | ButtonPressMask | ButtonReleaseMask | PointerMotionMask | StructureNotifyMask
LinuxScreen::LinuxScreen()
	:nativeWidth(0)
	,nativeHeight(0)
	,screenWidth(0)
	,screenHeight(0)
	,freamPerSecond(0)
	,fullscreen(false)
	,showmouse(true){
	//connetect to display
    display =XOpenDisplay(NULL);
    DEBUG_ASSERT(display);
    //get size screen
	nativeWidth = XWidthOfScreen(XDefaultScreenOfDisplay(display));
	nativeHeight = XHeightOfScreen(XDefaultScreenOfDisplay(display));
}
///////////////////////////////////////////////////////////
void LinuxScreen::__createGLXContext(uint bites){
	///////////////////////////////////////////////////////////
    //SETUP openGL
    bitesOpenGL=bites<24?bites:24;
	///////////////////////////////////////////////////////////
    //SET BUFFERS
    int bufferOpenGL[]={  GLX_RGBA,
                          GLX_DEPTH_SIZE, bitesOpenGL,
                          GLX_DOUBLEBUFFER,
                          None };
    //setup color map
    visual  = glXChooseVisual(display, screen,  bufferOpenGL );
    if (visual  == NULL){
        bufferOpenGL[3]=None;//diable GLX_DOUBLEBUFFER
        visual  = glXChooseVisual(display, screen,  bufferOpenGL );
        doubleBuffered = false;
        DEBUG_MESSAGE("singlebuffered rendering will be used, no doublebuffering available");
    }
    else{
        doubleBuffered = true;
        DEBUG_MESSAGE("doublebuffered rendering available");
    }
    DEBUG_ASSERT(visual);
	///////////////////////////////////////////////////////////
	//get openGL version
	int glxMajor, glxMinor;
    glXQueryVersion(display, &glxMajor, &glxMinor);
	DEBUG_MESSAGE("openGL rendering :"<<glxMajor<<"."<<glxMinor);
    // create a GLX context
    context = glXCreateContext(display, visual , 0, GL_TRUE);
    DEBUG_ASSERT(context);
	///////////////////////////////////////////////////////////
    //COLOR MAP WINDOW
    Colormap cmap;
    // create a color map
    cmap = XCreateColormap(display,
                           RootWindow(display, visual->screen),
                           visual ->visual,
                           AllocNone);
    //set att....
    winAttr.colormap = cmap;
    winAttr.border_pixel = 0;
	///////////////////////////////////////////////////////////
}
void LinuxScreen::__deleteGLXContext(){

    DEBUG_ASSERT(context);

	if( !glXMakeCurrent(display, None, NULL)){
		DEBUG_MESSAGE("Could not release drawing context.\n");
	}
	// destroy the context
	glXDestroyContext(display, context);
	context = NULL;

}
void LinuxScreen::__createFullScreenWindow(){
        //set fullscreen=true
        fullscreen=true;
        //set fullscreen
        XF86VidModeModeInfo **modes;
        int modeNum, bestMode=-1;
        //get info
        XF86VidModeGetAllModeLines(display,screen, &modeNum, &modes);
        // save desktop-resolution before switching modes
        desktopMode = *modes[0];
        //look for mode with requested resolution
        for (int i = 0; i < modeNum; i++)
            if ((modes[i]->hdisplay == screenWidth) && (modes[i]->vdisplay == screenHeight))
                bestMode = i;
        //////////////////////////////////////////////////////////////////////////////
        DEBUG_ASSERT_MSG(bestMode!=-1,"error : full-screen resolution not supported");
        //////////////////////////////////////////////////////////////////////////////
        //witch to fullscreen
        XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
        XF86VidModeSetViewPort(display, screen, 0, 0);
        nativeWidth = modes[bestMode]->hdisplay;
        nativeHeight = modes[bestMode]->vdisplay;
        DEBUG_MESSAGE("resolution : "<<nativeWidth<<"x"<<nativeHeight);
        XFree(modes);
        //set window attributes
        winAttr.override_redirect = True;
        winAttr.event_mask = WINDOW_ATTRIBUTE;
        window = XCreateWindow(display,
                               RootWindow(display, visual ->screen),
                               0, 0,
                               nativeWidth,
                               nativeHeight, 0,
                               bitesOpenGL,
                               InputOutput,
                               visual->visual,
                               CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                               &(winAttr));
        XWarpPointer(display, None, window, 0, 0, 0, 0, 0, 0);
        XMapRaised(display, window);
        XGrabKeyboard(display, window, 1, GrabModeAsync,GrabModeAsync, CurrentTime);
        XGrabPointer(display, window, 1, ButtonPressMask,GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
}
void LinuxScreen::__createWindow(){
        //set fullscreen=false
        fullscreen=false;
        //create a window in window mode
        winAttr.event_mask = WINDOW_ATTRIBUTE;
        window =
        XCreateWindow(display, RootWindow(display, visual ->screen),
                        0, 0,
                        screenWidth,
                        screenHeight,
                        0,
                        visual ->depth,
                        InputOutput,
                        visual ->visual,
                        CWBorderPixel | CWColormap | CWEventMask,
                        &(winAttr));
        //only set window title and handle wm_delete_events if in windowed mode
        Atom wmDelete = XInternAtom(display, "WM_DELETE_WINDOW", 1);
        XSetWMProtocols(display, window, &wmDelete, 1);
        XSetStandardProperties(display,
                               window,
                               appname,
                               appname,
                               None,
                               NULL,
                               0,
                               NULL);
        XMapRaised(display, window);
        //disable resize
        XSizeHints size_hints;
        size_hints.flags = PSize | PMinSize | PMaxSize;
        size_hints.min_height = size_hints.max_height = screenHeight;
        size_hints.min_width  = size_hints.max_width  = screenWidth;
        XSetWMNormalHints(display,window,&size_hints);
}
void LinuxScreen::__deleteWindow(){
    // switch back to original desktop resolution if we were in fullscreen
    if( fullscreen ){
        XF86VidModeSwitchToMode(display, screen, &(desktopMode));
        XF86VidModeSetViewPort(display, screen, 0, 0);
    }
    XCloseDisplay(display);
}
///////////////////////////////////////////////////////////
/**
* open a window
*/
void LinuxScreen::createWindow(const char* argappname,
                                  uint width,
                                  uint height,
                                  uint bites,
                                  uint freamPerSecond,
                                  bool fullscreen){

    appname=argappname;
    screenWidth= width;
    screenHeight= height;
    this->freamPerSecond=freamPerSecond;
    //x11 values
    //get screen
    screen = DefaultScreen(display);
	//create openGL context
	__createGLXContext(bites);
    //set fullscreen
    if(fullscreen)
		__createFullScreenWindow();
	else
		__createWindow();
    // connect the glx-context to the window
    glXMakeCurrent(display, window, context);
    //init openGL2
    initOpenGL2();
}
/**
* close window
*/
void LinuxScreen::closeWindow(){
	__deleteGLXContext();
	__deleteWindow();
}
/**
* LinuxScreen destructor
*/
LinuxScreen::~LinuxScreen(){
	closeWindow();
}
/**
* set fullscreen mode
*/
void LinuxScreen::setFullscreen(bool argfullscreen){
	if(fullscreen!=argfullscreen){
		__deleteWindow();
		if(fullscreen)
			__createFullScreenWindow();
		else
			__createWindow();
	}
}
/**
* return screen width
*/
uint LinuxScreen::getWidth(){
	return screenWidth;
}
/**
* return screen height
*/
uint LinuxScreen::getHeight(){
	return screenHeight;
}
/**
* return screen native width
*/
uint LinuxScreen::getNativeWidth(){
	return nativeWidth;
}
/**
* return screen  native height
*/
uint LinuxScreen::getNativeHeight(){
	return nativeHeight;
}
/**
* return screen orientation
*/
LinuxScreen::Orientation LinuxScreen::getOrientation(){
	return Screen::LANDSCAPE_RIGHT;
}
/**
* show or hide mouse cursor
* @param show, set true if you want show cursor otherwise false
*/
void LinuxScreen::setCursor(bool show){
	showmouse=show;
	if(!showmouse){
        Cursor invisibleCursor;
        Pixmap bitmapNoData;
        XColor black;
        static char noData[] = { 0,0,0,0,0,0,0,0 };
        black.red = black.green = black.blue = 0;

        bitmapNoData = XCreateBitmapFromData(display, window, noData, 8, 8);
        invisibleCursor = XCreatePixmapCursor(display, bitmapNoData, bitmapNoData,
                                             &black, &black, 0, 0);
        XDefineCursor(display,window, invisibleCursor);
        XFreeCursor(display, invisibleCursor);
	}
	else{
	    /*
	    Cursor cursor;
        cursor=XCreateFontCursor(display,XC_left_ptr);
        XDefineCursor(display, window, cursor);
        XFreeCursor(display, cursor);
	    */
        XUndefineCursor(display, window);
	}
}
/**
* return if cursor is shown or hidden
*/
bool LinuxScreen::getCursor(){
	return showmouse;
}
/**
* return if fullscreen is enable return true
*/
bool LinuxScreen::isFullscreen(){
	return fullscreen;
}
/**
* return frame rate
*/
uint LinuxScreen::getFrameRate(){
	return freamPerSecond;
}
/**
* set the specified thread's current rendering context
*/
void LinuxScreen::acquireContext(){
    // connect the glx-context to the window
    glXMakeCurrent(display, window, context);
}
/**
* swap the buffers
*/
void LinuxScreen::swap(){
	glXSwapBuffers( display, window );
}
