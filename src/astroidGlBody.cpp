#include "astroidGlBody.h"

AstroidGlBody :: AstroidGlBody(float radius) {
    this->radius = radius;
    this->rgba[0] = 0;
    this->rgba[1] = 1;
    this->rgba[2] = 2;
    this->rgba[3] = 3;
    this->isTextureBound = false;
    // for (int i = 0; i < 10; i++) {
    //     this->prevPositions.push_back(this->getPosition());
    // }
}
void AstroidGlBody :: setColor(float r, float g, float b, float a) {
    this->rgba[0] = r;
    this->rgba[1] = g;
    this->rgba[2] = b;
    this->rgba[3] = a;
}
void AstroidGlBody :: setTexture(int textureId){
    this->textureId = textureId;
    this->isTextureBound = true;
}
void AstroidGlBody :: draw() {
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
    if(this->drawCount >= 20) {
        this->prevPositions.pop_front();
        this->drawCount = 19;
    } else {
        this->prevPositions.push_back(this->getPosition());
        this->drawCount += 1;
    }
}
void AstroidGlBody :: drawTail(int textureId, float alpha=1) {
    float radius = 0.05;
    auto quad = gluNewQuadric();
    gluQuadricTexture(quad, GL_TRUE);
    glBindTexture(GL_TEXTURE_2D, textureId);
    for(auto position : this->prevPositions) {
        // auto position = this->prevPositions[i];
        glEnable(GL_COLOR_MATERIAL);
                glColor4f(1, 1, 1, alpha);
        glDisable(GL_COLOR_MATERIAL);
        glPushMatrix();
            glTranslatef(position.getX(), position.getY(), position.getZ());
            gluSphere(quad, radius, 20, 20); 
        glPopMatrix();
        radius = radius + 0.05;
    }
    gluDeleteQuadric(quad);
}
void AstroidGlBody :: draw(btVector3 position) {
    glPushMatrix();
        glTranslatef(
            position.getX(),
            position.getY(),
            position.getZ());
        this->draw();
    glPopMatrix();
}
