#include <stdafx.h>
#include <SphereSurface.h>
///////////////////////
using namespace Sphere;
///////////////////////
///////////////////////////
bool SphereSurface::Box::collision(const Box& box) const{
	return
	fabs( box.point.y - point.y ) <= (box.extents.y + extents.y) &&
	fabs( box.point.x - point.x ) <= (box.extents.x + extents.x) &&
	fabs( box.point.z - point.z ) <= (box.extents.z + extents.z);
} 
bool SphereSurface::Box::collision(const Vec3& point) const{
	return point.x<=(this->point.x+this->extents.x) &&
           point.x>=(this->point.x-this->extents.x) &&
           point.y<=(this->point.y+this->extents.y) &&
           point.y>=(this->point.y-this->extents.y) &&
           point.z<=(this->point.z+this->extents.z) &&
           point.z>=(this->point.z-this->extents.z) ;
} 
void SphereSurface::Box::draw() const{			
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
void SphereSurface::buildSurface(
				  AbstractSphere* argSphere,
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
	//range
	int rings=eRings-sRings;
	int sectors=eSections-sSections;
	/////////////////////////////////////////////////////////
	//set size vertex
	vertices.resize(rings*sectors);
	int idv=0;
	Vec3 color(Math::random(),0,0);
	//set vertex
	for(int r = sRings; r<eRings; ++r){
		for(int s= sSections; s<eSections; ++s){
			/* calc vertex */
			spherePoint(vertices[idv].vertex,r,s);
			vertices[idv].color=color;
			++idv;
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
void SphereSurface::spherePoint(Vec3& point,int ring, int section){

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
void SphereSurface::draw(bool vao){
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