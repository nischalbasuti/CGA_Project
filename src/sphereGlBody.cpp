#include "sphereGlBody.h"

SphereGlBody :: SphereGlBody(float radius) {
    this->radius = radius;
    this->rgba[0] = 0;
    this->rgba[1] = 1;
    this->rgba[2] = 2;
    this->rgba[3] = 3;
}
void SphereGlBody :: setColor(float r, float g, float b, float a) {
    this->rgba[0] = r;
    this->rgba[1] = g;
    this->rgba[2] = b;
    this->rgba[3] = a;
}
void SphereGlBody :: draw() {
    auto quad = gluNewQuadric();
    glPushMatrix();
    glColor4fv(this->rgba);
    // gluSphere(quad, radius, 40, 40);
    // gluSphere(quad, radius, 4, 4);
    gluDeleteQuadric(quad);
    // glutWireSphere(radius,3,3);
    glutWireSphere(radius,30,30);
    glPopMatrix();
}
