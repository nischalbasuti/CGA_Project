#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include "world.h"
#include "body.h"
#include "abstractGlBody.h"

World :: World(float gravity) {
    // std::cout << std::endl  << "....construct world...." << std::endl <<std::endl;

    // Initializing the world.////////////////////////////////////////////// 

    // Build the broadphase (an algorithm for collision detection).
    btBroadphaseInterface* broadphase = new btDbvtBroadphase();

    // Build collision config. and dispatcher.
    btDefaultCollisionConfiguration* collisionConfiguration = new btDefaultCollisionConfiguration();
    btCollisionDispatcher* dispatcher = new btCollisionDispatcher(collisionConfiguration);

    // Build the actual physics solver.
    btSequentialImpulseConstraintSolver* solver = new btSequentialImpulseConstraintSolver;

    // Build the world. All the stuff above are passed to create the world.
    this->dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, broadphase, solver, collisionConfiguration);
    // Set Gravity (Y-axis is "up" here)
    dynamicsWorld->setGravity(btVector3(0, -gravity, 0));
    ////////////////////////////////////////////////////////////////////////
}

World :: ~ World(){
	std::cout << std::endl << "....destroy world...." << std::endl;
}

void World :: drawBodies() {
	for(auto b : this->bodies) {
		b->update();
		b->draw();
	}
}

void World :: update() {
	for(auto b : this->bodies) {
		b->update();
	}
}

void World :: addBody(Body &body) {
	bodies.push_back(&body);
    this->dynamicsWorld->addRigidBody(body.getRigidBody());
}

void World :: removeBody(Body &body) {
	int removeIndex = -1;
	int indexCount = 0;
	//find the index of the body, if it exists.
	for(auto b : bodies){
		if(b == &body){
			removeIndex = indexCount;
			break;
		}
		indexCount++;
	}
	//return of the body does not exist.
	if(removeIndex == -1){
		return;
	}
	//remove the body if it exists.
	bodies.erase(bodies.begin() + indexCount);

    //remove from Bullet world.
    this->dynamicsWorld->removeRigidBody(body.getRigidBody());
}

btDiscreteDynamicsWorld* World :: getDynamicsWorld() const {
	return this->dynamicsWorld;
}
