#include <stdafx.h>
#include <Application.h>
#include <MainInstance.h>
#include <Camera.h>
#include <Math2D.h>

using namespace Sphere;

class TheSphere {

	//sphere radius
	float radius;
	//division sphere
	int rings;
	int sections;
	//value for normalize a section and a ring point
	float nrmRings;
	float nrmSections;

public:
	
	TheSphere(float radius,
			   int rings,
			   int sections)
			:radius(radius)
			,rings(rings)
			,sections(sections)
			,nrmRings(1./(float)(rings-1))
			,nrmSections(1./(float)(sections-1))
	{}

	DFORCEINLINE float getSection(int section){
		return section*nrmSections;
	}	
	DFORCEINLINE float getRing(int ring){
		return ring*nrmRings;
	}
	DFORCEINLINE int getSections(){
		return sections;
	}	
	DFORCEINLINE int getRings(){
		return rings;
	}
	DFORCEINLINE int getRadius(){
		return radius;
	}

};

class SphereSruface{
	
protected:
	
	struct GLVertex{
		Vec3 vertex;
		Vec3 color;
	};

	std::vector<GLVertex> vertices;
    std::vector<GLushort> indices;
	TheSphere *ptrSphere;

public:

	SphereSruface(TheSphere* argSphere,
				  //start and end rings
				  int sRings,
				  int eRings,
				  //start and end sections
				  int sSection,
				  int eSection)
				  :ptrSphere(argSphere)
	{

		for(int r = sRings; r<eRings; ++r){
			for(int s= sSection; s<eSection; ++s){
				GLVertex glvertex;
				spherePoint(glvertex.vertex,r,s);
				glvertex.color.x=ptrSphere->getRing(r);
				glvertex.color.y=ptrSphere->getSection(s);
				glvertex.color.x=ptrSphere->getRing(eRings)*ptrSphere->getSection(eSection);
				vertices.push_back(glvertex);
			}
		}
		//range
		int rings=eRings-sRings;
		int sectors=eSection-sSection;
		/*		
        *i++ = r * sectors + s;
        *i++ = r * sectors + (s+1);
        *i++ = (r+1) * sectors + (s+1);
        *i++ = (r+1) * sectors + s;
		*/
		/*      1   2
				*---*--------
				|   |
		        *---*--------
				4   3
		*/
		indices.resize(rings * sectors * 6);
        auto i = indices.begin();
        for(int r = 0; r < rings-1; r++) {
			for(int s = 0; s < sectors-1; s++) {
				//1 2 3
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
				//1 3 4
                *i++ = r * sectors + s;
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
			}
		}
	}
	
	void spherePoint(Vec3& point,int ring, int section){

		float const y = sin( -  Math::PI/2. + Math::PI * ptrSphere->getRing(ring));

        float const x = cos(  2*Math::PI * ptrSphere->getSection(section)) *
						sin( Math::PI * ptrSphere->getRing(ring) );

        float const z = sin(  2*Math::PI * ptrSphere->getSection(section)) * 
			            sin( Math::PI * ptrSphere->getRing(ring) );

		point.x=x*ptrSphere->getRadius();
		point.y=y*ptrSphere->getRadius();
		point.z=z*ptrSphere->getRadius();
	}

	void draw(bool lines=false){
		//
		if(lines)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//
        glVertexPointer(3,  GL_FLOAT,sizeof(GLVertex),&vertices[0]);
		glColorPointer (3,  GL_FLOAT,sizeof(GLVertex),((char*)&vertices[0])+sizeof(Vec3));
		glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
        glPopMatrix();
    }

};

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
	}

public:

	TheSphere sphere;
	SphereSruface 
		sphereSurface1,
		sphereSurface2,
		sphereSurface3,
		sphereSurface4,
		sphereSurface5,
		sphereSurface6,
		sphereSurface7,
		sphereSurface8;
	Camera camera;

	SphereInstance()
		:MainInstance("Sphere",1280,800,32,60,false)
		,sphere(1,320,320)

		//up
		,sphereSurface1(&sphere,0,160,0,80)
		,sphereSurface2(&sphere,0,160,80,160)
		,sphereSurface3(&sphere,0,160,160,240)
		,sphereSurface4(&sphere,0,160,240,320)
		//down
		,sphereSurface5(&sphere,160,320,0,80)
		,sphereSurface6(&sphere,160,320,80,160)
		,sphereSurface7(&sphere,160,320,160,240)
		,sphereSurface8(&sphere,160,320,240,320)	
	{}


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
		glMatrixMode(GL_MODELVIEW);
		glLoadMatrixf(camera.getGlobalMatrix());
		//draw
		sphereSurface1.draw(false);
		sphereSurface2.draw(false);
		sphereSurface3.draw(false);
		sphereSurface4.draw(false);
		sphereSurface5.draw(false);
		sphereSurface6.draw(false);
		sphereSurface7.draw(false);
		sphereSurface8.draw(false);
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
		//
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
		//
		Quaternion quadX,quadY;
		quadX.setFromEulero(0,
						    norm.x*Math::PI*2,
							0);
		quadY.setFromEulero(0,
						    0,
						    -norm.y*Math::PI*2);
		//
		camera.setRotation(quadX.mul(quadY));
	}
	virtual void onMousePress(Vec2 mousePosition, Key::Mouse button){}
	virtual void onMouseRelease(Vec2 mousePosition, Key::Mouse button){}
	virtual void onMouseScroll(short scrollDelta){
		projectionAngle*=(scrollDelta>0 ? 2.0f : 0.5f );
		setProjection(projectionAngle,0.1f,1000.0f);
	}
};

int main(){
	Application::create();
	Application::instance()->exec(new SphereInstance());
	delete Application::instance()->getMainInstance();
	return 0;
}
