#include <stdafx.h>
#include <SphereManager.h>

///////////////////////
using namespace Sphere;
///////////////////////
DFORCEINLINE int calcIterationSubSegments(int vals,int livel){
	for(int i=0;i<livel;++i) vals/=2 ;
	return vals;
}
SphereManager::SphereManager(float maxAngle,
							 int maxLivels,
							 int maxRings,
							 int maxSegments)
	:maxAngle(maxAngle)
	,diffAngle(maxAngle/(float)maxLivels)
	,curLivel(0)
	,maxLivels(maxLivels)
	,sphere(1,calcIterationSubSegments(maxRings,maxLivels)
			 ,calcIterationSubSegments(maxSegments,maxLivels)){
	

}
DFORCEINLINE void SphereManager::__addNode(SphereTree::SphereNode& node,int livel){
	if(livel<=0) return;
	for(int i=0;i<8;++i)
		__addNode(node.addNode(i),livel-1);
}

void SphereManager::update(float angle,float n,float f,const Mat4& mat){
	//intersecation point
	points.clear();
	gridview.calcRayCamInverse(angle,n,f,mat.getInverse());
	gridview.getNearPoints(sphere.getRoot().sphere,points);
	//get livel
	if(angle>maxAngle){ curLivel=0; return; }
	float livAngle=maxAngle;
	int liv=0;
	for(liv=0;angle<livAngle || livAngle<0;++liv,livAngle-=diffAngle);
	curLivel=Math::min(liv,maxLivels);
	//add livels
	//__addNode(sphere.getRoot(),curLivel);
}

DFORCEINLINE bool SphereManager::__surfaceCollision(SphereSurface& surface){
	for(auto& point:points)
		if(surface.getBox().collision(point))
		return true;		
	return false;
}
DFORCEINLINE void SphereManager::__drawSurfaceInPoints(SphereSurface& surface){
	for(auto& point:points){
		if(surface.getBox().collision(point)){
			surface.draw();
			return;
		}
	}
}
DFORCEINLINE void SphereManager::__drawNode(SphereTree::SphereNode& node,
											int selfLivel,
											int drawLivel){
	if(selfLivel<0) return;
	if(selfLivel<drawLivel){
		for(int i=0;i<8;++i){
			if(__surfaceCollision(node.surfaces[i])){
				__drawNode(node.addNode(i),
						   selfLivel+1,
						   drawLivel);
			}
		}
	}
	else if(selfLivel==drawLivel){
		node.draw();
	}
}

void SphereManager::draw(){
	if(curLivel)
		__drawNode(sphere.getRoot(),1,curLivel);
	else 
		sphere.getRoot().draw();
}