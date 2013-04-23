#ifndef THESHEPRE_H
#define THESHPERE_H

#include <Config.h>
#include <Math2D.h>

namespace Sphere{

struct Ray{
	
	Vec3 point;
	Vec3 dir;

	Ray(const Vec3& point=Vec3::ZERO,const Vec3& dir=Vec3::ZERO)
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

	bool rayCast(const Ray& r,Segment& sg) const{
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
		float sqrtD=sqrt(D);
		//points
		sg.t[0]=r.point+r.dir*(-B - sqrtD)/(2.0f*A);
		sg.t[1]=r.point+r.dir*(-B + sqrtD)/(2.0f*A);
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

template<static const int NX,
		 static const int NY>
class RayGrid{

public:

	enum CountRays
	{
	  nx=NX,
	  ny=NY,
	  count=NX*NY,
	};

private:
	

	Ray rays[NX*NY];
	Mat4 inverse;	

	void getFromProjection(float angle,float n,float f,float& outX,float& autY){
		autY = n * tanf(angle/2 * Math::PIOVER180);             
		outX = autY * ((float)(Application::instance()->getScreen()->getWidth())/
							  (Application::instance()->getScreen()->getHeight()));
	}

public:

	RayGrid(){};
	
	DFORCEINLINE void calcRayCam(float angle,float n,float f,const Mat4& mat){
		//
		inverse=mat.getInverse();
		//projection factor
		Vec3 factor;
		getFromProjection(angle,n,f,factor.x,factor.y);
		//create a grid		
		static const float invX=1.0/NX;
		static const float invY=1.0/NY;
		static const float invX2=2.0/NX;
		static const float invY2=2.0/NY;
		Vec2 step,stepDir;
		float diff=1.0/n;
		//rays
		for(int y=0;y<ny;++y)
		for(int x=0;x<nx;++x){
			//calc step
			step.x=(invX2*x+invX-1)*factor.x;
			step.y=(invX2*y+invY-1)*factor.y;
			stepDir.x=(invX2*x+invX-1)*diff*factor.x;
			stepDir.y=(invX2*y+invY-1)*diff*factor.y;
			Vec3 pos(inverse.mul(Vec4( step,      0.0,1.0)).xyz());
			Vec3 dir(inverse.mul(Vec4( stepDir,  -1.0,0.0)).xyz());
			rays[y*nx+x]=Ray(pos,dir);
		}
	}
	DFORCEINLINE void calcRay(float angle,float n,float f,const Mat4& mat){
		//
		inverse=mat.getInverse();
		//projection factor
		Vec3 factor;
		getFromProjection(angle,n,f,factor.x,factor.y);
		//create a grid		
		static const float invX=1.0/NX;
		static const float invY=1.0/NY;
		static const float invX2=2.0/NX;
		static const float invY2=2.0/NY;
		Vec2 step;
		//rays
		for(int y=0;y<ny;++y)
		for(int x=0;x<nx;++x){
			//calc step
			step.x=(invX2*x+invX-1)*factor.x;
			step.y=(invX2*y+invY-1)*factor.y;
			//
			rays[y*nx+x]=Ray(inverse.mul(Vec4( step,  0.0,1.0)).xyz(),
			                 inverse.mul(Vec4( step, -1.0,0.0)).xyz());
		}
	}

	void getNearPoints(const TheSphere& sphere,std::vector<Vec3>& points) const{		
		points.clear();
		Segment sTmp;
		for(auto& ray:rays)
			if(sphere.rayCast(ray,sTmp)) 
				points.push_back(sTmp.t[0]);		
	}

	void draw(){
		for(auto& ray:rays) ray.draw();		
	}
	void drawCast(const TheSphere& sphere){		
		Segment sTmp;
		for(auto& ray:rays)
			if(sphere.rayCast(ray,sTmp)) 
				sTmp.draw();
	}
};


};

#endif