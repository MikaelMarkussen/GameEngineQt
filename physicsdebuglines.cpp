#include "physicsdebuglines.h"

PhysicsDebugLines::PhysicsDebugLines()
{

}

PhysicsDebugLines::~PhysicsDebugLines()
{

}

void PhysicsDebugLines::setDebugLines(const PxRenderBuffer &rd)
{   //This line crashes the app
    //mPhys.mScene->setVisualizationParameter(PxVisualizationParameter::eACTOR_AXES, 1.f);
    for(PxU32 i = 0; i < rd.getNbLines(); i++){
           const PxDebugLine line = rd.getLines()[i];
           PxVec3 pos1 = line.pos0;
           PxVec3 pos2 = line.pos1;

           PxU32 rgb1 = line.color0;
           PxU32 rgb2 = line.color1;

           mVertices.push_back(Vertex(pos1.x,pos1.y,pos1.z,1,0,0));
           mVertices.push_back(Vertex(pos2.x,pos2.y,pos2.z,1,0,0));
    }
}

void PhysicsDebugLines::init(GLint matrixUniform) {
    mMatrixUniform = matrixUniform;

    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof( Vertex ), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    //enable the matrixUniform
    // mMatrixUniform = glGetUniformLocation( matrixUniform, "matrix" );

    glBindVertexArray(0);
}

void PhysicsDebugLines::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawArrays(GL_LINES, 0, mVertices.size());
}
