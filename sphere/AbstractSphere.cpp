#include <stdafx.h>
#include <AbstractSphere.h>
#include <Rays.h>
///////////////////////
using namespace Sphere;
///////////////////////
bool AbstractSphere::rayCast(const Ray& r,Segment& sg) const{
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
