#pragma once
#include <bullet/btBulletDynamicsCommon.h>
#include "abstractGlBody.h"
#include <GL/glew.h>
#include <GL/freeglut.h> 

class Body {
// This class wraps our opengl body and btRigidBody.
	public:
		Body(AbstractGlBody *glBody, btVector3 *position, btCollisionShape *collisionShape, btScalar mass, bool isStatic);
		~Body();
        void update();  // update position of Bullet object as well as opengl Object.
        void draw();    // draw the opengl object.
		void setPosition(float, float, float); // set the position of opengl object and Bullet object
		void setPosition(btVector3 position); // set the position of opengl object and Bullet object
        btVector3 const getPosition() const;
        void setRotation(float, float, float, float);
        btRigidBody* getRigidBody();
	private:
        btRigidBody *rigidBody;
        AbstractGlBody *glBody;
        btVector3 position;
        
        btScalar rotationMatrix[16];
};
