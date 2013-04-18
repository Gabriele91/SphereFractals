#include <stdafx.h>
#include <Application.h>
#include <MainInstance.h>
#include <Camera.h>
#include <Math2D.h>

using namespace Sphere;

class SolidSphere
{
protected:
    std::vector<GLfloat> vertices;
    std::vector<GLfloat> normals;
    std::vector<GLfloat> texcoords;
    std::vector<GLushort> indices;

public:

    SolidSphere(float radius, unsigned int rings, unsigned int sectors)
    {
        float const R = 1./(float)(rings-1);
        float const S = 1./(float)(sectors-1);
        int r, s;

        vertices.resize(rings * sectors * 3);
        normals.resize(rings * sectors * 3);
        texcoords.resize(rings * sectors * 2);
        std::vector<GLfloat>::iterator v = vertices.begin();
        std::vector<GLfloat>::iterator n = normals.begin();
        std::vector<GLfloat>::iterator t = texcoords.begin();
        for(r = 0; r < rings; r++) for(s = 0; s < sectors; s++) {
                float const y = sin( -  Math::PI/2. + Math::PI * r * R );
                float const x = cos(  2*Math::PI * s * S) * sin( Math::PI * r * R );
                float const z = sin(  2*Math::PI * s * S) * sin( Math::PI * r * R );

                *t++ = s*S;
                *t++ = r*R;

                *v++ = x * radius;
                *v++ = y * radius;
                *v++ = z * radius;

                *n++ = x;
                *n++ = y;
                *n++ = z;
        }

        indices.resize(rings * sectors * 4);
        std::vector<GLushort>::iterator i = indices.begin();
        for(r = 0; r < rings-1; r++) for(s = 0; s < sectors-1; s++) {
                *i++ = r * sectors + s;
                *i++ = r * sectors + (s+1);
                *i++ = (r+1) * sectors + (s+1);
                *i++ = (r+1) * sectors + s;
        }
    }
    void draw(bool lines=false){		
		//
		if(lines) 
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		//
        glVertexPointer(3, GL_FLOAT, 0, &vertices[0]);
        glNormalPointer(GL_FLOAT, 0, &normals[0]);
        glTexCoordPointer(2, GL_FLOAT, 0, &texcoords[0]);
        glDrawElements(GL_QUADS, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
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

	SolidSphere sphere;
	Camera camera;
	
	SphereInstance()
		:MainInstance("Sphere",1280,720)
		,sphere(1, 12, 24){}


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
		glCullFace( GL_BACK ); 
		//set projection matrix
		projectionAngle=45.0f;
		setProjection(projectionAngle,0.1f,1000.0f);
		//reset model view matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
		//enable state
        glEnableClientState(GL_VERTEX_ARRAY);
        glEnableClientState(GL_NORMAL_ARRAY);
        glEnableClientState(GL_TEXTURE_COORD_ARRAY);
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
		sphere.draw();
	}
	void end(){
	}
	/* OOP-STYLE EVENTS */
	virtual void onKeyPress(Key::Keyboard key){}
	virtual void onKeyRelease(Key::Keyboard key) {}
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
						    norm.x*Math::PI,
							0);
		quadY.setFromEulero(0,
						    0,
						    -norm.y*Math::PI);
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