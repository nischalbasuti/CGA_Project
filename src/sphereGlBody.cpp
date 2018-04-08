#include "sphereGlBody.h"

SphereGlBody :: SphereGlBody(float radius) {
    this->radius = radius;
    this->rgba[0] = 0;
    this->rgba[1] = 1;
    this->rgba[2] = 2;
    this->rgba[3] = 3;
    this->isTextureBound = false;
}
void SphereGlBody :: setColor(float r, float g, float b, float a) {
    this->rgba[0] = r;
    this->rgba[1] = g;
    this->rgba[2] = b;
    this->rgba[3] = a;
}
void SphereGlBody :: setTexture(int textureId){
    this->textureId = textureId;
    this->isTextureBound = true;
}
void SphereGlBody :: draw() {
    glPushMatrix();
    glRotatef(90,0,0,1);
    auto quad = gluNewQuadric();
    if(this->isTextureBound) {
        glEnable(GL_COLOR_MATERIAL);
            glColor4f(1,1,1,this->rgba[3]);
            // glColor4f(1,1,1,1);
        glDisable(GL_COLOR_MATERIAL);
        gluQuadricTexture(quad, GL_TRUE);
        glBindTexture(GL_TEXTURE_2D, this->textureId);
    } else {
        glEnable(GL_COLOR_MATERIAL);
            glColor4fv(this->rgba);
        glDisable(GL_COLOR_MATERIAL);
    }
    gluSphere(quad, radius, 40, 40);
    gluDeleteQuadric(quad);
    glPopMatrix();
}
void SphereGlBody :: draw(btVector3 position) {
    glPushMatrix();
        glTranslatef(
            position.getX(),
            position.getY(),
            position.getZ());
        this->draw();
    glPopMatrix();
}
