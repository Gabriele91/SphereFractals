#ifndef THESHEPRE_H
#define THESHPERE_H

#include <Config.h>
#include <Math2D.h>

namespace Sphere{

	
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

	//start and end rings
	int sRings;
	int eRings;
	//start and end sections
	int sSections;
	int eSections;
	//buffer in GPU
	uint vertexBuffer;
	uint indexBuffer;


public:
	
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
				  int eSections){
		//save values	  
		this->ptrSphere=argSphere;
		this->sRings=sRings;
		this->eRings=eRings;
		this->sSections=sSections;
		this->eSections=eSections;
		/////////////////////////////////////////////////////////
		//r <= eRings and s <= eSection
		++eSections;
		++eRings;
		//max is argSphere->getRings() and argSphere->getSections() 
		eSections=Math::min(eSections,argSphere->getSections());
		eRings=Math::min(eRings,argSphere->getRings());
		/////////////////////////////////////////////////////////
		//
		Vec3 color(Math::random(),Math::random(),Math::random());
		//
		for(int r = sRings; r<eRings; ++r){
			for(int s= sSections; s<eSections; ++s){
				GLVertex glvertex;
				spherePoint(glvertex.vertex,r,s);
				glvertex.color=color;
				vertices.push_back(glvertex);
			}
		}
		//range
		int rings=eRings-sRings;
		int sectors=eSections-sSections;
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
		/////////////////////////////////////////////////////////////////////////////////////////////////
		//send to GPU 			
		if( !vertexBuffer )
			glGenBuffers(1, &vertexBuffer );
		glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLVertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
		if( !indexBuffer )
		   glGenBuffers(1, &indexBuffer );		
		glBindBuffer(GL_ARRAY_BUFFER, indexBuffer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(ushort) * indices.size(), &indices[0], GL_STATIC_DRAW);
		/////////////////////////////////////////////////////////////////////////////////////////////////
	}
	/**
	* get sphere point
	*/
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
	/**
	* draw sphere surface
	*/
	void draw(bool vao=false){
		//
		//
		if(vao){
			glBindBuffer( GL_ARRAY_BUFFER, 0 );
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); 
			glVertexPointer(3,  GL_FLOAT,sizeof(GLVertex),&vertices[0]);
			glColorPointer (3,  GL_FLOAT,sizeof(GLVertex),((char*)&vertices[0])+sizeof(Vec3));
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, &indices[0]);
		}
		//vbo
		else if(vertexBuffer && indexBuffer){
			//bind VBO
			glBindBuffer( GL_ARRAY_BUFFER, vertexBuffer );
			//set vertex
			glVertexPointer(3, GL_FLOAT, sizeof(GLVertex), 0 );
			glColorPointer(3, GL_FLOAT, sizeof(GLVertex), (void*)sizeof(Vec3) );
			//bind IBO
			glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, indexBuffer ); 
			//draw
			glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_SHORT, 0);
		}
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
				   )
				   :sphere(radius,rings,sections)
				   ,subRingsTop(subRingsTop)
				   ,subRingsDown(subRingsDown)
				   ,subSectionsLeft(subSectionsLeft)
				   ,subSectionsRight(subSectionsRight)
		{
			//set to 0 references
			memset(nodes,0,sizeof(SphereNode*)*8);
			//calc division rigns
			int up=subRingsTop,
			    middle=ceil((float)subRingsDown/2.0f),
			    down=subRingsDown;
			//calc division sections
			int hlpart=(subSectionsRight-subSectionsLeft)/4.0f;
			int part1=subSectionsLeft,
			    part2=subSectionsLeft+hlpart,
			    part3=subSectionsLeft+hlpart*2,
			    part4=subSectionsLeft+hlpart*3,
			    part5=subSectionsRight;
			//build parts
			//up
			surfaces[0].buildSruface(&sphere,up,middle,part1,part2);
			surfaces[1].buildSruface(&sphere,up,middle,part2,part3);
			surfaces[2].buildSruface(&sphere,up,middle,part3,part4);
			surfaces[3].buildSruface(&sphere,up,middle,part4,part5);
			//down
			surfaces[4].buildSruface(&sphere,middle,down,part1,part2);
			surfaces[5].buildSruface(&sphere,middle,down,part2,part3);
			surfaces[6].buildSruface(&sphere,middle,down,part3,part4);
			surfaces[7].buildSruface(&sphere,middle,down,part4,part5);
		}
		/**
		* destructor
		*/
		~SphereNode(){
			for(auto node:nodes)
				if(node)
					delete node; 
		}
		/**
		* draw sphere sub parts 
		*/
		void draw(){
			//draw face
			for(auto& surface:surfaces) 
				surface.draw();
		}
		/*
		* add a sub node
		*/
		void addNode(int i,int factor=2){
			
			if(!nodes[i]){
				nodes[i]=
					new SphereNode(
						//shere
						sphere.getRadius(),
						sphere.getRings()*factor,
						sphere.getSections()*factor,
						//sub part
						surfaces[i].startRings()*factor,
						surfaces[i].endRings()*factor,
						surfaces[i].startSections()*factor,
						surfaces[i].endSections()*factor
					);
			}

		}
		SphereNode& getNode(int i){
			return *nodes[i];
		}
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