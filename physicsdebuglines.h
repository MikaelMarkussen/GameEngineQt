#ifndef PHYSICSDEBUGLINES_H
#define PHYSICSDEBUGLINES_H
#include "physicsmanager.h"
#include "visualobject.h"


class PhysicsDebugLines : public VisualObject
{
public:
    PhysicsDebugLines();
    ~PhysicsDebugLines();
    PhysicsComponent mPhys;
    void setDebugLines(const PxRenderBuffer& rd);
    void init(GLint matrixUniform) override;
    void draw() override;
};

#endif // PHYSICSDEBUGLINES_H
