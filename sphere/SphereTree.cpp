#include <stdafx.h>
#include <SphereTree.h>

///////////////////////
using namespace Sphere;
///////////////////////
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
			surfaces[0].buildSurface(&sphere,up,middle,part1,part2);
			surfaces[1].buildSurface(&sphere,up,middle,part2,part3);
			surfaces[2].buildSurface(&sphere,up,middle,part3,part4);
			surfaces[3].buildSurface(&sphere,up,middle,part4,part5);
			//down
			surfaces[4].buildSurface(&sphere,middle,down,part1,part2);
			surfaces[5].buildSurface(&sphere,middle,down,part2,part3);
			surfaces[6].buildSurface(&sphere,middle,down,part3,part4);
			surfaces[7].buildSurface(&sphere,middle,down,part4,part5);
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


DSPHERENODE& DSPHERENODE::addNode(int i,int factor){			
			if(!nodes[i])
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
			return *nodes[i];
}

DSPHERENODE& DSPHERENODE::getNode(int i){
			return *nodes[i];
		}
//////////////////////////////////////////////////