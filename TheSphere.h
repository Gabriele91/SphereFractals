#ifndef THESHEPRE_H
#define THESHPERE_H

#include <Config.h>
#include <Math2D.h>

namespace Sphere{

struct Ray{
	
	Vec3 point;
	Vec3 dir;

	Ray(const Vec3& point,const Vec3& dir)
		:point(point),dir(dir.getNormalize()){
	}

	void draw(float leng=10000);
};

struct Segment{

	Vec3 t[2];	
	void draw();

};

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
	
	TheSphere( float radius=0.0f,
			   int rings=2,
			   int sections=2)
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

	bool rayCast(const Ray& r,Segment& sg){
		/*
		http://www.siggraph.org/education/materials/HyperGraph/raytrace/rtinter1.htm
		http://www.gamedev.net/topic/476158-ray-sphere-intersection/
		*/
		//calc B					 
		// We solve this second-degree equation in t:
	    // distance(p+t*v,center)==radius
	    // If we define w = p-center
	    // we can write that as
	    // <w+t*v,w+t*v> == radius*radius
	    // <w,w> + 2.0f*t*<w,v> + t*t*<v,v> - radius*radius == 0
	    // <v,v>*t*t + 2.0f*<w,v>*t + <w,w>-radius*radius == 0
	    // A*t*t + B*t*t + C*t*t == 0
		Vector3D w = r.point;
		float A = r.dir.dot(r.dir);
		float B = 2*w.dot(r.dir);
		float C = w.dot(w) - radius*radius;
		//calc d
		float D = B*B-4.0f*A*C;
		//points
		sg.t[0]=r.point+r.dir*(-B - sqrt(D))/(2.0f*A);
		sg.t[1]=r.point+r.dir*(-B + sqrt(D))/(2.0f*A);
		//
		return D >=0;
	}

};

class SphereSruface{
	
protected:
	
	struct GLVertex{
		Vec3 vertex;
		Vec3 color;
		GLVertex(){}
		GLVertex(const Vec3& vertex,const Vec3& color)
			    :vertex(vertex),color(color){}
		GLVertex(float values[6])
				:vertex(values[0],values[1],values[2])
				,color(values[3],values[4],values[5]){}
		GLVertex(float values[3],const Vec3& color)
				:vertex(values[0],values[1],values[2])
				,color(color){}
	};

	std::vector<GLVertex> vertices;
    std::vector<GLushort> indices;
	TheSphere *ptrSphere;

	//start and end rings
	int sRings;
	int eRings;
	//start and end sections
	int sSections;
	int eSections;
	//buffer in GPU
	uint vertexBuffer;
	uint indexBuffer;
	
	//culling values	
	struct Box {
		Vec3 point;
		Vec3 extents;
		bool collision(const Box& box) const; 
		void draw() const;
	};
	Box box;

public:
	
	const Box& getBox(){
		return box;
	}

	DFORCEINLINE int startRings(){ return sRings; }
	DFORCEINLINE int endRings(){ return eRings; }
	DFORCEINLINE int startSections(){ return sSections; }
	DFORCEINLINE int endSections(){ return eSections; }


	SphereSruface():ptrSphere(NULL),vertexBuffer(0),indexBuffer(0){}
	SphereSruface(TheSphere* argSphere,
				  //start and end rings
				  int sRings,
				  int eRings,
				  //start and end sections
				  int sSections,
				  int eSections)
				  :ptrSphere(argSphere),vertexBuffer(0),indexBuffer(0){
		buildSruface(argSphere,sRings,eRings,sSections,eSections);
	}

	~SphereSruface(){
		//////////////////////////////////////////////
		//delete gpu buffers
		if( vertexBuffer )
			glDeleteBuffers(1, &vertexBuffer );
		if( indexBuffer )
			glDeleteBuffers(1, &indexBuffer );
		//////////////////////////////////////////////
	}
	/**
	* build a sphere sub part
	*/
	void buildSruface(TheSphere* argSphere,
				  //start and end rings
				  int sRings,
				  int eRings,
				  //start and end sections
				  int sSections,
				  int eSections);
	/**
	* get sphere point
	*/
	void spherePoint(Vec3& point,int ring, int section);
	/**
	* draw sphere surface
	*/
	void draw(bool vao=false);
	/**
	* release cpu memory
	*/
	void clearCPUMemory(){
		//force clear
		vertices.clear();
		vertices.resize(0);
		vertices.shrink_to_fit();
		//force clear
		indices.clear();
		indices.resize(0);
		indices.shrink_to_fit();
	}

};

//OCT-TREE
class SphereTree{



	struct SphereNode{

		TheSphere   sphere;
		int subRingsTop,subRingsDown;
		int subSectionsLeft,subSectionsRight;

		SphereSruface surfaces[8];
		SphereNode* nodes[8];
		/**
		* costructor
		*/
		SphereNode(
				   //sphere (origin)
				   float radius,int rings,int sections,
				   //sphere sub part
				   int subRingsTop,int subRingsDown,
				   int subSectionsLeft,int subSectionsRight
				   );
		/**
		* destructor
		*/
		~SphereNode();
		/**
		* release cpu memory
		*/
		void clearCPUMemorySurfaces(){
			for(auto& surfaces:surfaces)
				surfaces.clearCPUMemory();			
		}
		/**
		* draw sphere sub parts 
		*/
		void draw();
		/**
		* draw sphere sub parts boxs 
		*/
		void drawBoxs();
		/*
		* add a sub node
		*/
		void addNode(int i,int factor=2);
		SphereNode& getNode(int i);
	};
	
	SphereNode root;

public:

	SphereTree(//sphere (origin)
			   float radius,int rings,int sections)
			   :root(//sphere (origin)
					 radius,rings,sections,
					 //sphere sub part
					 0,rings,
					 0,sections)
	{}

	SphereNode& getRoot(){
		return root;
	}

	void draw(){
		root.draw();
	}

};

};

#endif