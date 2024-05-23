#ifndef TRIANGLESURFACE_H
#define TRIANGLESURFACE_H

#include <visualobject.h>
#include <barycentriccalc.h>
#include <PxPhysicsAPI.h>
#include "physicsmanager.h"

using namespace physx;

class TriangleSurface : public VisualObject
{
public:
    TriangleSurface();
    TriangleSurface(std::string filename, GLuint ShaderId, GLuint TextureId, PxPhysics* physics, PxScene* scene, PxCooking* cooking, const char* name);
    ~TriangleSurface() override;
    void readFile(std::string filename);
    void writeFile(std::string filename);
    void origoFixer();
    void construct();
    void triangulate();
    void normalize();
    QVector3D normalize(Vertex::Triangle t);
    Vertex::Triangle findTriangle(float x, float y, float z);
    void shaderToggle(GLuint ShaderId);
    void init(GLint matrixUniform) override;
    void draw() override;
    std::vector<Vertex>& get_vertices() { return mVertices; } // 191120
    std::vector<Vertex::Triangle>& get_triangles(){ return mTriangles; };
    float xmin = 800000.f, xmax = 0, ymin = 131400000.f, ymax = 0, zmin = 300, zmax = 0;
    int width = 0, depth = 0;
    float height = 0;
    float zReturn = 0;
    int Ti = 0;
    bool OOB = true;
    PxRigidStatic *creatStaticPhysics(PxPhysics* physics, PxCooking* cooking, PxScene* scene, const char* name);
    PhysicsComponent* phys;
private:
   PxRigidStatic* mTerrain = nullptr;
   PxShape* mShape;
   std::vector<PxVec3> pxVerticies;
   std::vector<PxU32> pxIndecies;
   PxMaterial* mat;
   const char* name;

};

#endif // TRIANGLESURFACE_H
