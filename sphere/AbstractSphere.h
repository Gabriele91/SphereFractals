#ifndef ABSTRACTSPHERE_H
#define ABSTRACTSPHERE_H

#include <Config.h>
#include <Math2D.h>

namespace Sphere {
	/////////////////////////
	struct Ray;
	struct Segment;
	/////////////////////////
	class AbstractSphere {

		//sphere radius
		float radius;
		//division sphere
		int rings;
		int sections;
		//value for normalize a section and a ring point
		float nrmRings;
		float nrmSections;

	public:
	
		AbstractSphere( float radius=0.0f,
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

		bool rayCast(const Ray& r,Segment& sg) const;
	};

};

#endif