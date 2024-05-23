#ifndef OBJECTMESH_H
#define OBJECTMESH_H

#include "visualobject.h"

class ObjectMesh : public VisualObject
{
public:
    ObjectMesh(std::string fileName, GLuint ShaderId, GLuint TextureId);
    ~ObjectMesh()override;
    virtual void draw() override;
    virtual void init(GLint matrixUniform) override;
private:
    void readFile(std::string fileName);
};

#endif // OBJECTMESH_H
