#include <iostream>
#include <cmath>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "getBMP.h"
#include "world.h"
#include "body.h"
#include "abstractGlBody.h"
#include "sphereGlBody.h"

#define PI 3.14

class PlanetBody : public Body {
	public:
		PlanetBody(float radius, AbstractGlBody *glBody, btVector3 *position,
		 btCollisionShape* collisionShape,
		 btScalar mass=0, bool isStatic=false)
		: Body(glBody, position, collisionShape,mass,isStatic){
			this->radius = radius;		
		}
		void revolve(float revolutionSlices,
		 float radius = -1, float X = 0, float Y = 0, float Z = 0) { //TODO: remove radius, should be class memeber.

            this->revolutionSlices = revolutionSlices;
			float R = this->radius;
			if(radius != -1) {
				R = radius;
			}
			auto v = btVector3(0, 0, 0);
			v.setX(X + R * cos(this->t));
			v.setY(0.0);
			v.setZ(Z + R * sin(this->t));
			this->t -= 2 * PI / revolutionSlices;
			this->setPosition(v);
		}
        btVector3 getFuturePosition(float delta_t) const{
            float X=0, Y=0 , Z=0; //TODO: maybe parameterize.
            float tempT = this->t - (delta_t * 2 * PI / this->revolutionSlices);
			float R = this->radius;
			// if(radius != -1) { //TODO
			// 	R = radius;
			// }
			auto v = btVector3(0, 0, 0);
			v.setX(X + R * cos(tempT));
			v.setY(0.0);
			v.setZ(Z + R * sin(tempT));
            // std::cout << v.getX() << " ";
            // std::cout << v.getY() << " ";
            // std::cout << v.getZ() << std::endl;
            // std::cout << this->getPosition().getX() << " ";
            // std::cout << this->getPosition().getY() << " ";
            // std::cout << this->getPosition().getZ() << std::endl;
            return v;
        }
		float radius;
		float t = 0;
		float rotationY = 0;
        float revolutionSlices = 10;
};
