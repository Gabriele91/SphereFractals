#ifndef SPHERESURFACE_H
#define SPHERESURFACE_H

#include <Config.h>
#include <Math2D.h>
#include <AbstractSphere.h>

namespace Sphere {

	class SphereSurface{
	
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
		AbstractSphere *ptrSphere;

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
			bool collision(const Vec3& point) const; 
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


		SphereSurface():ptrSphere(NULL),vertexBuffer(0),indexBuffer(0){}
		SphereSurface(AbstractSphere* argSphere,
					  //start and end rings
					  int sRings,
					  int eRings,
					  //start and end sections
					  int sSections,
					  int eSections)
					  :ptrSphere(argSphere),vertexBuffer(0),indexBuffer(0){
			buildSurface(argSphere,sRings,eRings,sSections,eSections);
		}

		~SphereSurface(){
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
		void buildSurface(AbstractSphere* argSphere,
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

};

#endif