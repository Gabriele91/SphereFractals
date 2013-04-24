#ifndef SPHEREMANAGER_H
#define SPHEREMANAGER_H

#include <Config.h>
#include <Math2D.h>
#include <SphereTree.h>
#include <Rays.h>

namespace Sphere {

	class SphereManager {
		
		float maxAngle
			 ,diffAngle;
		int maxLivels
		   ,curLivel;
		std::vector<Vec3> points;
		RayGrid<50,50> gridview;
		SphereTree sphere;

		DFORCEINLINE void __addNode(SphereTree::SphereNode& node,int livel);
		DFORCEINLINE void __drawNode(SphereTree::SphereNode& node,int selfLivel,int drawLivel);
		DFORCEINLINE bool __surfaceCollision(SphereSurface& surface);
		DFORCEINLINE void __drawSurfaceInPoints(SphereSurface& surface);
	public:

		SphereManager(float maxAngle,
					  int maxLivels,
					  int maxRings,
					  int maxSegments);
		void update(float angle,float n,float f,const Mat4& mat);
		void draw();
	};

};

#endif