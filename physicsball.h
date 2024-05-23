#ifndef PHYSICSBALL_H
#define PHYSICSBALL_H
#include "visualobject.h"
#include "physicsmanager.h"


using namespace physx;

class PhysicsBall : public VisualObject
{
public:
    PhysicsBall(PxVec3 velocity, PxSphereGeometry radius, QVector3D position, GLuint ShaderId, GLuint TextureId);
    PxRigidBody *createDynamic();
    PhysicsComponent* mPhys;
    void move();
private:

    const PxTransform mPose = PxTransform(PxVec3(mMatrix.column(3).x(),mMatrix.column(3).y(),mMatrix.column(3).z()));
    PxSphereGeometry mGeomentry = PxSphereGeometry(1);
    PxVec3 mVelocityPx  = PxVec3(0,0,0);
    PxMaterial* mMat;
    PxRigidDynamic* dynamic;

    void creatMesh();

};

#endif // PHYSICSBALL_H
