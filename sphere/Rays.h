#ifndef RAYS_H
#define RAYS_H

#include <Config.h>
#include <Math2D.h>
#include <AbstractSphere.h>

namespace Sphere {

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

		void getNearPoints(const AbstractSphere& sphere,std::vector<Vec3>& points) const{		
			points.clear();
			Segment sTmp;
			for(auto& ray:rays)
				if(sphere.rayCast(ray,sTmp)) 
					points.push_back(sTmp.t[0]);		
		}

		void draw(){
			for(auto& ray:rays) ray.draw();		
		}
		void drawCast(const AbstractSphere& sphere){		
			Segment sTmp;
			for(auto& ray:rays)
				if(sphere.rayCast(ray,sTmp)) 
					sTmp.draw();
		}
	};

};

#endif