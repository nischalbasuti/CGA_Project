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
		float radius;
		float t = 0;
		float rotationY = 0;
};