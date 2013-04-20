#include <stdafx.h>
#include <Application.h>
#include <MainInstance.h>
#include <Camera.h>
#include <Math2D.h>
#include <TheSphere.h>

using namespace Sphere;


class SphereInstance : public MainInstance,
							  Input::KeyboardHandler,
							  Input::MouseHandler{


	float projectionAngle;
	Matrix4x4 projection;
	void setProjection(float angle,float n,float f){
		projection.setPerspective
			(angle,
			(float)
			(Application::instance()->getScreen()->getWidth())/
			(Application::instance()->getScreen()->getHeight()),n,f);
		//update projection
		glMatrixMode(GL_PROJECTION);
		glLoadMatrixf(projection);
		//now change only MODELVIEW
		glMatrixMode(GL_MODELVIEW);
	}

public:

	SphereTree sphereTree;
	Camera camera;
	Vec2 rotation;


	SphereInstance()
		:MainInstance("Sphere",1280,800,32,60,false)
		,sphereTree(1,160,160)	
	{
		sphereTree.getRoot().addNode(0);
		sphereTree.getRoot().addNode(1);
		sphereTree.getRoot().addNode(2);
		sphereTree.getRoot().addNode(3);
		sphereTree.getRoot().addNode(5);
		sphereTree.getRoot().getNode(0).addNode(0);
	}


	void start(){
		//input
		Application::instance()->getInput()->addHandler((Input::KeyboardHandler*)this);
		Application::instance()->getInput()->addHandler((Input::MouseHandler*)this);
		/////////////////////////////////////////////
		//OPENGL
		//view port
		glViewport(0, 0, Application::instance()->getScreen()->getWidth(),
						 Application::instance()->getScreen()->getHeight());
		//enable culling
		glEnable( GL_CULL_FACE );
		glCullFace( GL_FRONT );
		//set projection matrix
		projectionAngle=45.0f;
		setProjection(projectionAngle,0.1f,1000.0f);
		//reset model view matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//enable state
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_COLOR_ARRAY);
		/////////////////////////////////////////////
		//camera position
		camera.setPosition(Vec3(0,0,-20));
	}


	void run(float dt){
		//clear
		glClearColor(0.25f, 0.5f, 1.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		//reset model view matrix
		glLoadMatrixf(camera.getGlobalMatrix());
		//draw
		//glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
		sphereTree.draw();
		sphereTree.getRoot().getNode(0).draw();
		sphereTree.getRoot().getNode(5).draw();
		sphereTree.getRoot().getNode(0).getNode(0).draw();
	}
	void end(){
	}
	/* OOP-STYLE EVENTS */
	virtual void onKeyPress(Key::Keyboard key){}
	virtual void onKeyRelease(Key::Keyboard key) {
		//exit event
		if(key==Key::ESCAPE) 
			Application::instance()->exit();
	}

	virtual void onMouseMove(Vec2 mousePosition) {
		if(Application::instance()->getInput()->getMouseDown(Key::BUTTON_LEFT)){
			//center of screen
			mousePosition.x-=Application::instance()->getScreen()->getWidth();
			mousePosition.y-=Application::instance()->getScreen()->getHeight();
			//normalize pos mouse
			Vec2 norm( mousePosition.x/Application::instance()->getScreen()->getWidth(),
					   mousePosition.y/Application::instance()->getScreen()->getHeight() );
			//
			//fake center
			norm.y+=0.5;
			norm.x+=0.5;
			//rotation vel
			float vel=2.;//projectionAngle*0.1;
			rotation.x=norm.x*Math::PI*vel;
			rotation.y=-norm.y*Math::PI*vel;
			//
			Quaternion quadX,quadY;
			quadX.setFromEulero(0,
								rotation.x,
								0);
			quadY.setFromEulero(0,
								0,
								rotation.y);
			//
			camera.setRotation(quadX.mul(quadY));
		}
	}
	virtual void onMousePress(Vec2 mousePosition, Key::Mouse button){
			
	}
	virtual void onMouseRelease(Vec2 mousePosition, Key::Mouse button){}
	virtual void onMouseScroll(short scrollDelta){
		projectionAngle*=(scrollDelta>0 ? 1.25f : 0.75f );
		setProjection(projectionAngle,0.1f,1000.0f);
	}
};

int main(){
	Application::create();
	Application::instance()->exec(new SphereInstance());
	delete Application::instance()->getMainInstance();
	return 0;
}
