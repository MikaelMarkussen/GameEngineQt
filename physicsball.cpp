#include "physicsball.h"

PhysicsBall::PhysicsBall(PxVec3 velocity, PxSphereGeometry radius, QVector3D position, GLuint ShaderId, GLuint TextureId)
{
    mShaderId = ShaderId;
    mTextureId = TextureId;
    mVelocityPx = velocity;
    mGeomentry = radius;
    mMatrix.setColumn(3,position.toVector4D());

}

PxRigidBody *PhysicsBall::createDynamic()
{
    if(mPose.isValid())
    {
        mMat = mPhys->getPhysics()->createMaterial(0.5f,0.5f,0.5f);
        dynamic = PxCreateDynamic(*mPhys->getPhysics(),mPose,mGeomentry,*mMat,10.f);
        if(mVelocityPx != PxVec3(0,0,0)){
            dynamic->setAngularVelocity(mVelocityPx);
        }
        return dynamic;
    }else
    {
        return nullptr;
    }
}

void PhysicsBall::move()
{
    PxMat44 temp = dynamic->getGlobalPose();

    PxVec3 t = temp.getPosition();

    mMatrix.setColumn(3,QVector4D(t.x,t.y,t.z,1));



}

void PhysicsBall::creatMesh()
{

}
