#pragma once

#include <iostream>
#include <bullet/btBulletDynamicsCommon.h>
#include <GL/glew.h>
#include <GL/freeglut.h> 
#include "getBMP.h"
#include "world.h"
#include "body.h"
#include "abstractGlBody.h"
#include <deque>

class AstroidGlBody : public AbstractGlBody {
    public:
		AstroidGlBody(float); 
		void setColor(float, float, float, float);
		void setTexture(int);
        void draw();
        void drawTail(int, float);
		void draw(btVector3);
    private:
		float radius;
		float rgba[4];
		float textureId;
		bool isTextureBound = false;
        std::deque<btVector3> prevPositions;
        int drawCount = 0;
};
