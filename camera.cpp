#include "camera.h"

Camera::Camera()
{

}

void Camera::init()
{
    initializeOpenGLFunctions();
    mPMatrix.setToIdentity();
    mVMatrix.setToIdentity();
    mYawMatrix.setToIdentity();
    mPitchMatrix.setToIdentity();
    lookAt();
}

void Camera::perspective(int degrees, double aspect, double nearplane, double farplane)
{
    mPMatrix.perspective(degrees, aspect, nearplane, farplane);
}

void Camera::lookAt()
{
    updateVectors();
    mVMatrix.lookAt(position(), mForward, mUp);
}

void Camera::update(float dt)
{

}

QVector3D Camera::position()
{
    QVector3D pos = {mPosition.column(3).x() ,mPosition.column(3).y(), mPosition.column(3).z()};
    return pos;
}

void Camera::updateVectors()
{
    QVector3D direction{0,0,0};
    direction.setX(cos(qDegreesToRadians(mYaw)));
    direction.setY(sin(qDegreesToRadians(mYaw)));
    direction.normalize();
    mPosition.setColumn(3, {mx, my, mz, w});
    mForward = (position() - target).normalized();
    mRight = QVector3D::normal(wUp, mForward);
    mUp = QVector3D::crossProduct(mForward, mRight);
}

void Camera::translate(float dx, float dy, float dz)
{
    //Change yaw
//    mYawMatrix.rotate(dx*-20, 0,0,-1);//Angle and z-axis rotation from x-axis input
    //Direction calculations, let's the camera move in direction it's facing
//    dx = dy*-(mYawMatrix.column(0).x(), mYawMatrix.column(1).x()); //cos angle - sin angle
//    dy *= (mYawMatrix.column(0).y(), mYawMatrix.column(1).y());   //sin angle + cos angle

    dx=-dx, dy=-dy, dz=-dz;
    mYaw+=dx;
    mx+=dx, my+=dy, mz+=dz;
    updateVectors();
    mVMatrix.translate(dx,dy,dz);
    //lookAt();
}
