#include "body.h"
#include "abstractGlBody.h"
#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>

Body :: Body(AbstractGlBody *glBody, btVector3 *position, btCollisionShape* collisionShape, btScalar mass=0, bool isStatic=false) {
    this->glBody = glBody;
    this->position = *position;
    btDefaultMotionState* motionState 
        = new btDefaultMotionState(
                btTransform(
                    btQuaternion(0, 0, 0, 1),
                    this->position
                    )
                );
    btVector3 intertia(0, 0, 0);
    if( !isStatic ) {
        collisionShape->calculateLocalInertia(mass, intertia);
    } else {
        mass = 0;
    }
    btRigidBody::btRigidBodyConstructionInfo rigidBodyCI(mass, motionState, collisionShape, intertia);
    this->rigidBody = new btRigidBody(rigidBodyCI);
}

Body :: ~Body() {
	std::cout << "body destoyed" << std::endl;
}

void Body :: update() {
    // Get transformation as opengl compatible matrix.
    btTransform transform;
    this->rigidBody->getMotionState()->getWorldTransform(transform);
    transform.getOpenGLMatrix(this->rotationMatrix);

    this->glBody->setPosition(this->rigidBody->getWorldTransform().getOrigin());
    this->setPosition(this->rigidBody->getWorldTransform().getOrigin());
}

void Body :: draw() {
    glPushMatrix();
        glMultMatrixf((GLfloat*)this->rotationMatrix); // Apply the transformation.
        this->glBody->draw();
    glPopMatrix();
}
btRigidBody* Body :: getRigidBody() {
    return this->rigidBody;
}

void Body :: setPosition(btVector3 position) {
    this->position.setX( position.getX() );
    this->position.setY( position.getY() );
    this->position.setZ( position.getZ() );

    this->glBody->setPosition(this->position);
}
void Body :: setPosition(float x, float y, float z) {
    this->position.setX( x );
    this->position.setY( y );
    this->position.setZ( z );
    this->glBody->setPosition(this->position);
}