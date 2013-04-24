#ifndef SPHERETREE_H
#define SPHERETREE_H

#include <Config.h>
#include <Math2D.h>
#include <AbstractSphere.h>
#include <SphereSurface.h>

namespace Sphere {	

	//OCT-TREE
	class SphereTree{		
	public:

		////////////////////////////
		struct SphereNode{

			AbstractSphere   sphere;
			int subRingsTop,subRingsDown;
			int subSectionsLeft,subSectionsRight;

			SphereSurface surfaces[8];
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
					   );
			/**
			* destructor
			*/
			~SphereNode();
			/**
			* release cpu memory
			*/
			void clearCPUMemorySurfaces(){
				for(auto& surfaces:surfaces)
					surfaces.clearCPUMemory();			
			}
			/**
			* draw sphere sub parts 
			*/
			void draw();
			/**
			* draw sphere sub parts boxs 
			*/
			void drawBoxs();
			/*
			* add a sub node
			*/
			SphereNode& addNode(int i,int factor=2);
			SphereNode& getNode(int i);
		};
		////////////////////////////

		////////////////////////////
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
		
	private:

		SphereNode root;
	};


};

#endif