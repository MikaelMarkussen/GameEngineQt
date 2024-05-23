#ifndef CAMERA_H
#define CAMERA_H

#include <QOpenGLFunctions_4_1_Core>
#include <QMatrix4x4>
#include <QVector3D>
#include <QQuaternion>

//http://www.opengl-tutorial.org/intermediate-tutorials/tutorial-17-quaternions/#reading-quaternions

class Camera : public QOpenGLFunctions_4_1_Core
{
public:
    Camera();
    ~Camera(){};
    void init();
    void perspective(int degrees, double aspect, double nearplane, double farplane);
    void frustum();
    void lookAt();
    void update(float dt);
    void translate(float dx, float dy, float dz);

    QQuaternion rotateVec(QVector3D start, QVector3D end);
    QQuaternion rotateTo(QQuaternion q1, QQuaternion q2, float maxAngle);
    QMatrix4x4 mVMatrix{};
    QMatrix4x4 mPMatrix{};
    QVector3D position();
    QVector3D getView();

    QVector3D rotationAxis;
    QQuaternion quat;


private:
    float mx = 0, my = -2, mz = 5, w = 1;
    QMatrix4x4 mPosition;

    QVector3D wForward {0.f, -1.f, 0.f};
    QVector3D wRight{1.f, 0.f, 0.f};
    QVector3D wUp{0.f, 0.f, 1.f};
    QVector3D target {0,0,0};
    QVector3D mForward {0,0,0};
    QVector3D mRight {0,0,0};
    QVector3D mUp {0,0,0};
    void updateVectors();

    float rx=0, ry=0, rz=-90, angle=0;
    QVector3D eulerAngles = {90, 45, 0};

    float mPitch{0.f};
    float mYaw{-90.f};

    QMatrix4x4 mYawMatrix;
    QMatrix4x4 mPitchMatrix;
    float mSpeed{0.f};
};

#endif // CAMERA_H
