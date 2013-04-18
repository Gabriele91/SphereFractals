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
/**
* set fullscreen mode
*/
void LinuxScreen::setFullscreen(bool fullscreen){
    //save
    this->fullscreen=fullscreen;
    if(fullscreen){
        //values
        XF86VidModeModeInfo **modes;
        int modeNum, bestMode;
        //get info
        XF86VidModeGetAllModeLines(display,screen, &modeNum, &modes);
        // save desktop-resolution before switching modes
        desktopMode = *modes[0];
        //look for mode with requested resolution
        for (int i = 0; i < modeNum; i++)
            if ((modes[i]->hdisplay == screenWidth) && (modes[i]->vdisplay == screenHeight))
                bestMode = i;
        //witch to fullscreen
        XF86VidModeSwitchToMode(display, screen, modes[bestMode]);
        XF86VidModeSetViewPort(display, screen, 0, 0);
        nativeWidth = modes[bestMode]->hdisplay;
        nativeHeight = modes[bestMode]->vdisplay;
        DEBUG_MESSAGE("resolution : "<<nativeWidth<<"x"<<nativeHeight);
        XFree(modes);

    }
    else{
        XF86VidModeSwitchToMode(display, screen, &desktopMode);
        XF86VidModeSetViewPort(display, screen, 0, 0);
    }

}
/**
* open a window
*/
void LinuxScreen::createWindow(const char* appname,
                                  uint width,
                                  uint height,
                                  uint bites,
                                  uint freamPerSecond,
                                  bool fullscreen){
    screenWidth= width;
    screenHeight= height;
    this->freamPerSecond=freamPerSecond;
    //x11 values
    XVisualInfo *visual ;
    Colormap cmap;
    int i, dpyWidth, dpyHeight;
    int glxMajor, glxMinor, vmMajor, vmMinor;
    //get screen
    screen = DefaultScreen(display);
    DEBUG_ASSERT(screen);
    //query
    XF86VidModeQueryVersion(display, &vmMajor, &vmMinor);
    //setup openGL
    //SET BUFFERS
    int bufferOpenGL[]={  GLX_RGBA,
                          GLX_DEPTH_SIZE, bites<24?bites:24,
                          GLX_DOUBLEBUFFER,
                          None };
    //setup color map
    visual  = glXChooseVisual(display, screen,  bufferOpenGL );
    if (visual  == NULL){
        bufferOpenGL[3]=None;//diable GLX_DOUBLEBUFFER
        visual  = glXChooseVisual(display, screen,  bufferOpenGL );
        doubleBuffered = false;
        DEBUG_MESSAGE("singlebuffered rendering will be used, no doublebuffering available\n");
    }
    else{
        doubleBuffered = true;
        DEBUG_MESSAGE("doublebuffered rendering available\n");
    }
    glXQueryVersion(display, &glxMajor, &glxMinor);
    // create a GLX context
    context = glXCreateContext(display, visual , 0, GL_TRUE);
    // create a color map
    cmap = XCreateColormap(display,
                           RootWindow(display, visual->screen),
                           visual ->visual,
                           AllocNone);

    DEBUG_ASSERT(context);
    //set att....
    winAttr.colormap = cmap;
    winAttr.border_pixel = 0;
    //set fullscreen
    if (fullscreen){
        //set fullscreen
        setFullscreen(true);
        //set window attributes
        winAttr.override_redirect = True;
        winAttr.event_mask = WINDOW_ATTRIBUTE;
        window = XCreateWindow(display,
                               RootWindow(display, visual ->screen),
                               0, 0,
                               nativeWidth,
                               nativeHeight, 0,
                               bites,
                               InputOutput,
                               visual->visual,
                               CWBorderPixel | CWColormap | CWEventMask | CWOverrideRedirect,
                               &(winAttr));
        XWarpPointer(display, None, window, 0, 0, 0, 0, 0, 0);
        XMapRaised(display, window);
        XGrabKeyboard(display, window, 1, GrabModeAsync,GrabModeAsync, CurrentTime);
        XGrabPointer(display, window, 1, ButtonPressMask,GrabModeAsync, GrabModeAsync, window, None, CurrentTime);
    }
    else{
        //create a window in window mode
        winAttr.event_mask = WINDOW_ATTRIBUTE;
        window =
        XCreateWindow(display, RootWindow(display, visual ->screen),
                        0, 0,
                        width,
                        height,
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
        size_hints.min_height = size_hints.max_height = height;
        size_hints.min_width  = size_hints.max_width  = width;
        XSetWMNormalHints(display,window,&size_hints);
        //
    }
    // connect the glx-context to the window
    glXMakeCurrent(display, window, context);
}
/**
* close window
*/
void LinuxScreen::closeWindow(){

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
