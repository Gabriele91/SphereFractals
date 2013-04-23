#include <stdafx.h>
#include <TheSphere.h>
///////////////////////////
using namespace Sphere;
///////////////////////////
void Ray::draw(float leng){	
	//VAO DRAW
	Vec3 vertices[2]={point,dir*leng};
	Vec3 color[2]={Vec3(0.0,1.0,1.0),Vec3(0.0,1.0,1.0)};
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); 
	glVertexPointer(3,  GL_FLOAT,sizeof(Vec3),&vertices[0]);			
	glColorPointer (3,  GL_FLOAT,sizeof(Vec3),&color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	//END DRAW
}
///////////////////////////
void Segment::draw(){
	//VAO DRAW
	Vec3 vertices[2]={t[0],t[1]};
	Vec3 color[2]={Vec3(0.0,0.0,0.0),Vec3(1.0,0.0,0.0)};
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); 
	glVertexPointer(3,  GL_FLOAT,sizeof(Vec3),&vertices[0]);			
	glColorPointer (3,  GL_FLOAT,sizeof(Vec3),&color[0]);
	glDrawArrays(GL_LINES, 0, 2);
	//END DRAW	
}
///////////////////////////
bool SphereSruface::Box::collision(const Box& box) const{
	return
	fabs( box.point.y - point.y ) <= (box.extents.y + extents.y) &&
	fabs( box.point.x - point.x ) <= (box.extents.x + extents.x) &&
	fabs( box.point.z - point.z ) <= (box.extents.z + extents.z);
} 
void SphereSruface::Box::draw() const{			
	std::vector<GLVertex> vertices;
	Vec3 color(1.0,0.0,0.0);
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//squere 1
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//line1
	vertices.push_back(GLVertex(Vec3(point.x+extents.x, point.y+extents.y, point.z-extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y-extents.y, point.z-extents.z),color));
	//line2
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y-extents.y, point.z-extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y-extents.y, point.z-extents.z),color));
	//line3
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y-extents.y, point.z-extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y+extents.y, point.z-extents.z),color));
	//line4			
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y+extents.y, point.z-extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y+extents.y, point.z-extents.z),color));
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//squere 2
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//line1
	vertices.push_back(GLVertex(Vec3(point.x+extents.x, point.y+extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y-extents.y, point.z+extents.z),color));
	//line2
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y-extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y-extents.y, point.z+extents.z),color));
	//line3
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y-extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y+extents.y, point.z+extents.z),color));
	//line4			
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y+extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y+extents.y, point.z+extents.z),color));
	/////////////////////////////////////////////////////////////////////////////////////////////////			
	//connect squere			
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//line 1
	vertices.push_back(GLVertex(Vec3(point.x+extents.x, point.y+extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x+extents.x, point.y+extents.y, point.z-extents.z),color));
	//line 2			
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y-extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x+extents.x,point.y-extents.y, point.z-extents.z),color));
	//line 3			
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y-extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y-extents.y, point.z-extents.z),color));
	//line 4			
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y+extents.y, point.z+extents.z),color));
	vertices.push_back(GLVertex(Vec3(point.x-extents.x,point.y+extents.y, point.z-extents.z),color));
	/////////////////////////////////////////////////////////////////////////////////////////////////
	//draw
	glBindBuffer( GL_ARRAY_BUFFER, 0 );
	glBindBuffer( GL_ELEMENT_ARRAY_BUFFER, 0 ); 
	glVertexPointer(3,  GL_FLOAT,sizeof(GLVertex),&vertices[0]);			
	glColorPointer (3,  GL_FLOAT,sizeof(GLVertex),((char*)&vertices[0])+sizeof(Vec3));
	glDrawArrays(GL_LINES, 0, vertices.size());
}
///////////////////////////
void SphereSruface::buildSruface(
				  TheSphere* argSphere,
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
	//
	Vec3 color(Math::random(),0,0);
	for(int r = sRings; r<eRings; ++r){
		for(int s= sSections; s<eSections; ++s){
			/* calc vertex */
			GLVertex glvertex;
			spherePoint(glvertex.vertex,r,s);
			glvertex.color=color;
			vertices.push_back(glvertex);
		}
	}
	//calc box
	//center
	for(auto& vertice:vertices)
		box.point+=vertice.vertex;	
	box.point/=vertices.size();
	//extents	
	box.extents=-Vec3::MAX;
	for(auto& vertice:vertices)
		box.extents=Math::max(box.extents,(vertice.vertex-box.point).getAbs());
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
void SphereSruface::spherePoint(Vec3& point,int ring, int section){

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
void SphereSruface::draw(bool vao){
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
//////////////////////////////////////////////////
//////////////////////////////////////////////////
//////////////////////////////////////////////////
#define DSPHERENODE SphereTree::SphereNode
DSPHERENODE::SphereNode(
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
			    middle=subRingsTop+(subRingsDown-subRingsTop)/2.0f,
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

DSPHERENODE::~SphereNode(){
			for(auto node:nodes)
				if(node)
					delete node; 
		}

void DSPHERENODE::draw(){
			//draw face
			for(auto& surface:surfaces) 
				surface.draw();
		}

void DSPHERENODE::drawBoxs(){
	for(auto& surface:surfaces) 
		surface.getBox().draw();
}

void DSPHERENODE::addNode(int i,int factor){			
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

DSPHERENODE& DSPHERENODE::getNode(int i){
			return *nodes[i];
		}
//////////////////////////////////////////////////