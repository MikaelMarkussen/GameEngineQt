#include <QDebug>
#include <iostream>
#include "trianglesurface.h"
#include "vertex.h"

TriangleSurface::TriangleSurface() : VisualObject()
{

}

TriangleSurface::TriangleSurface(std::string filename, GLuint ShaderId, GLuint TextureId, PxPhysics *physics, PxScene *scene, PxCooking* cooking, const char *name) : VisualObject()
{
    mShaderId = ShaderId;
    mTextureId = TextureId;
    readFile(filename);

    origoFixer(); //moves point data to origo
    construct(); //takes point data and fills our grid with it to construct the proper surface
    triangulate(); //sets up triangles with indexes and neighbours
    normalize();

    creatStaticPhysics(physics,cooking,scene,name);

    mMatrix.setToIdentity();
}

TriangleSurface::~TriangleSurface()
{

}

void TriangleSurface::readFile(std::string filename)
{
    std::ifstream inn;
    inn.open(filename.c_str());

    if (inn.is_open())
    {
        int n;
        Vertex vertex;
        inn >> n;
        mVertices.reserve(n);
        for (int i=0; i<n; i++)
        {
             inn >> vertex;
             mVertices.push_back(vertex);
             if(vertex.getX() < xmin)
                 xmin = vertex.getX();
             if(vertex.getY() < ymin)
                 ymin = vertex.getY();
             if(vertex.getZ() < zmin)
                 zmin = vertex.getZ();
             if(vertex.getX() > xmax)
                 xmax = vertex.getX();
             if(vertex.getY() > ymax)
                 ymax = vertex.getY();
             if(vertex.getZ() > zmax)
                 zmax = vertex.getZ();
        }
        inn.close();
    }
}

void TriangleSurface::writeFile(std::string filename)
{
    std::ofstream ut;
    ut.open(filename.c_str());

    if (ut.is_open())
    {

        auto n = mVertices.size();
        Vertex vertex;
        ut << n << std::endl;
        for (auto it=mVertices.begin(); it != mVertices.end(); it++)
        {
            //vertex = *it;
            ut << *it << std::endl;
        }
        ut.close();
    }
}

void TriangleSurface::origoFixer()
{
    width = xmax - xmin, depth = ymax - ymin, height = zmax - zmin;
    for (unsigned int i = 0; i < mVertices.size(); i++)
    {
        mVertices.at(i).setXYZ(mVertices.at(i).getX()-xmin,
                               mVertices.at(i).getY()-ymin,
                               mVertices.at(i).getZ()-zmin);
    }
}

void TriangleSurface::construct()
{
    QVector3D grid[12][17];
    for (unsigned int i = 0; i < mVertices.size(); i++)
    {
        int x = mVertices[i].getX()/10;
        int y = mVertices[i].getY()/10;
        float z = mVertices[i].getZ()/10;
        if(grid[x][y].z())
        {
            z = (z + mVertices[i].getZ())/2.f;
        }
        else
            z = mVertices[i].getZ();
        //qDebug() << x << y << z;
        grid[x][y].setX(x),grid[x][y].setY(y), grid[x][y].setZ(z);
    }

    std::vector<QVector3D> normal;
    for (unsigned int i = 0; i < mVertices.size(); i++)
    {
        float x = mVertices[i].getX()/10;
        float y = mVertices[i].getY()/10;
        float z = mVertices[i].getZ()/10;

        long double ls = pow(x,2) + pow(y,2) + pow(z,2);
        double l = sqrt(ls);
        x /= l, y/= l, z/= l;
        normal.push_back(QVector3D(x,y,z));
    }

    mVertices.clear();
    depth /= 10, width /= 10;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < depth; j++)
        {
            float x = (grid[i][j].x() -width/2)*2+1;
            float y = (grid[i][j].y() -depth/2)*2+1;
            float z = (grid[i][j].z()/2 -15);
            float u = ((x + width)/(width)/2),
                    v = ((y + depth)/(depth)/2);
            mVertices.push_back({x,y,z, 0,0,0, u,v});
        }
    }

    for(int i = 0; i < width - 1; i++)       // for each row a.k.a. each strip
    {
        for(int j = 0; j < depth - 1; j++)      // for each column
        {
            int Vi = (i * depth) + j;
            mIndices.push_back(Vi);         //Lower left
            mIndices.push_back(Vi + depth); //Lower right
            mIndices.push_back(Vi + 1);     //Upper left

            mIndices.push_back(Vi + 1);
            mIndices.push_back(Vi + depth);
            mIndices.push_back(Vi + depth + 1);  //Upper right
        }
    }
}

void TriangleSurface::triangulate()
{
    int t = 0;
    int size = (width-1)*(depth-1)*2;
    int yloop = 32; //used to check if we've looped the grid on the y axis
    int n0, n1, n2; //neighbours
    for(unsigned int i = 0; i < mIndices.size(); i+=6) // +1 per triangle, 2 triangles at a time
    {
        //Bottom left triangle, even numbered triangles
        n0 = t + 1;     //the neighbour above
        n1 = t - 31;    //the neighbour to the left
        if(n1 < 0)      //check to see if n1 is outside the grid
            n1 = -1;

        if(yloop == 32) //using yloop to see if we're at the bottom of the grid
        {
            n2 = -1;
            yloop = 0;  //reset yloop
            //qDebug() << "Y loop reset";
        }
        else            //the neighbour below if we're not at the bottom of the grid
            n2 = t - 1;
        Vertex::Triangle tEven(mIndices.at(i), mIndices.at(i + 1), mIndices.at(i + 2), n0, n1, n2);
        mTriangles.push_back(tEven);
        //qDebug() << "t" << t << ": " << mIndices[i] << mIndices[i + 1] << mIndices[i + 2] << n0 << n1 << n2;
        t++;

        //Top right triangle, odd numbered triangles
        n0 = t + 31;    //the neighbour to the right
        if(n0 >= size)
            n0 = -1;

        if(yloop == 30) //using yloop to see if we're at the top of the grid
            n1 = -1;
        else
            n1 = t + 1;     //the neighbour above

        n2 = t - 1;     //the neighbour below
        Vertex::Triangle tOdd(mIndices.at(i + 3), mIndices.at(i + 4), mIndices.at(i + 5), n0, n1, n2);
        mTriangles.push_back(tOdd);
        //qDebug() << "t" << t << ": "<< mIndices[i+3] << mIndices[i + 4] << mIndices[i + 5] << n0 << n1 << n2;
        yloop += 2;     //moving two times up the grid
        t++;
//        qDebug() << tEven.indexes[0] << tEven.indexes[1] << tEven.indexes[2]
//                << tEven.neighbours[0] << tEven.neighbours[1] << tEven.neighbours[2]
//                << "\n" << tOdd.indexes[0] << tOdd.indexes[1] << tOdd.indexes[2]
//                   << tOdd.neighbours[0] << tOdd.neighbours[1] << tOdd.neighbours[2];
    }
}

void TriangleSurface::normalize()
{
    for(int i = 0; i < width; i++)
    {
        for(int j = 0; j < depth-1; j++)
        {
            QVector3D n = {0,0,0};
            QVector3D V0, V1, V2, V3, V4, V5, V6 = {0,0,0};
            float Vi = i * depth + j;
            //Current vertex
            V0 = {Vi, Vi, mVertices[Vi].getZ()};
            if(j != 0)                  //Lower vertex
                V1 = {Vi, Vi - 1, mVertices[Vi - 1].getZ()};
            if(i + 1 < width && j != 0) //Lower right vertex
                V2 = {Vi + depth, Vi - 1, mVertices[Vi + depth - 1].getZ()};
            if(i + 1 < width)           //Right vertex
                V3 = {Vi + depth, Vi, mVertices[Vi + depth].getZ()};
            if(j < depth)               //Upper vertex
                V4 = {Vi, Vi + 1, mVertices[Vi + 1].getZ()};
            if(j < depth-1 && i > 0)    //Upper left vertex
                V5 = {Vi - depth, Vi + 1, mVertices[Vi - depth + 1].getZ()};
            if(i > 0)                   //Left vertex
                V6 = {Vi - depth, Vi, mVertices[Vi - depth].getZ()};

            n += QVector3D::normal(V0, V1, V2); //T0
            n += QVector3D::normal(V0, V2, V3); //T1
            n += QVector3D::normal(V0, V3, V4); //T2
            n += QVector3D::normal(V0, V4, V5); //T3
            n += QVector3D::normal(V0, V5, V6); //T4
            n += QVector3D::normal(V0, V6, V1); //T5
            n.normalize();
            mVertices[Vi].setNormal(n);
        }
    }
}

QVector3D TriangleSurface::normalize(Vertex::Triangle t)
{
    QVector3D n0 = {mVertices[t.indexes[0]].getX(),
                    mVertices[t.indexes[0]].getY(),
                    mVertices[t.indexes[0]].getZ()};
    QVector3D n1 = {mVertices[t.indexes[1]].getX(),
                    mVertices[t.indexes[1]].getY(),
                    mVertices[t.indexes[1]].getZ()};
    QVector3D n2 = {mVertices[t.indexes[2]].getX(),
                    mVertices[t.indexes[2]].getY(),
                    mVertices[t.indexes[2]].getZ()};
return QVector3D::normal(n0,n1,n2);
}

Vertex::Triangle TriangleSurface::findTriangle(float x, float y, float z)
{
    bool found = false;
    Ti = 0;
    float u = 0, v = 0, w = 0;
    if(Ti>=0 && Ti < mTriangles.size())
    {
        do {
            //qDebug() << "Current triangle: " << Ti;
            QVector2D P = {mVertices[mTriangles[Ti].indexes[0]].getX(), mVertices[mTriangles[Ti].indexes[0]].getY()},
                    Q = {mVertices[mTriangles[Ti].indexes[1]].getX(), mVertices[mTriangles[Ti].indexes[1]].getY()},
                    R = {mVertices[mTriangles[Ti].indexes[2]].getX(), mVertices[mTriangles[Ti].indexes[2]].getY()};
            //qDebug() << P << Q << R;
            //Beregn barysentriske koordinater for trekant Ti
            BarycentricCalc BC(QVector2D{x,y});
            QVector3D result = BC.calculate(P, Q, R);
            u = result.x(), v = result.y(), w = result.z();
            if (u >= 0 && v >= 0 && w >= 0
                    && u <= 1 && v <= 1 && w <= 1)
            {
                found = true;
            }
            //else Ti = nabo som svarer til minste barysentriske koordinaten
            else
            {
                //qDebug() << "Vectors:" << result;
                if(u<=v && u<=w && mTriangles[Ti].neighbours[0] != -1)
                {
                    Ti = mTriangles[Ti].neighbours[0];
                }
                else if(v<=u && v<=w && mTriangles[Ti].neighbours[1] != -1)
                {
                    Ti = mTriangles[Ti].neighbours[1];
                }
                else if(w<=u && w<=v && mTriangles[Ti].neighbours[2] != -1)
                {
                    Ti = mTriangles[Ti].neighbours[2];
                }
                else
                {
                    //qDebug() << "Out of bounds";
                    //zReturn = -5;
                    OOB = true;
                    found = true;
                }
            }
        } while (!found);
        float mP = u * mVertices[mTriangles[Ti].indexes[0]].getZ();
        float mQ = v * mVertices[mTriangles[Ti].indexes[1]].getZ();
        float mR = w * mVertices[mTriangles[Ti].indexes[2]].getZ();
        zReturn = 0.1 + (mP + mQ + mR);
        if(z > zReturn)
            OOB = true;
        else
            OOB = false;
        //qDebug() << zReturn;
        if(OOB)
        {
            //qDebug() << OOB << "Out of bounds";
            return Vertex::Triangle(-1,-1,-1,-1,-1,-1);
        }
        else
        {
//            qDebug() << "Triangle found!" << mTriangles[Ti].indexes[0]
//                     << mTriangles[Ti].indexes[1] << mTriangles[Ti].indexes[2];
            return mTriangles[Ti];
        }
    }
    else
    {
        //qDebug() << "Out of bounds";
        //zReturn = 0;
        return Vertex::Triangle(-1,-1,-1,-1,-1,-1);
    }
}

void TriangleSurface::shaderToggle(GLuint ShaderId)
{
    mShaderId = ShaderId;
}

void TriangleSurface::init(GLint matrixUniform)
{
    mMatrixUniform = matrixUniform;
    initializeOpenGLFunctions();

    //Vertex Array Object - VAO
    glGenVertexArrays( 1, &mVAO );
    glBindVertexArray( mVAO );

    //Vertex Buffer Object to hold vertices - VBO
    glGenBuffers( 1, &mVBO );
    glBindBuffer( GL_ARRAY_BUFFER, mVBO );
    glBufferData( GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), mVertices.data(), GL_STATIC_DRAW );

    // 1rst attribute buffer : vertices
    glBindBuffer(GL_ARRAY_BUFFER, mVBO);
    glVertexAttribPointer(0, 3, GL_FLOAT,GL_FALSE,sizeof(Vertex), reinterpret_cast<const void*>(0));
    glEnableVertexAttribArray(0);

    // 2nd attribute buffer : colors
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(3 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(1);

    // 3rd attribute buffer : uvs
    glVertexAttribPointer(2, 2,  GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)( 6 * sizeof(GLfloat)) );
    glEnableVertexAttribArray(2);

    glGenBuffers(1, &mIBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void TriangleSurface::draw()
{
    initializeOpenGLFunctions();
    glBindVertexArray( mVAO );
    // GL_FALSE for QMatrix4x4
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, GLsizei(mIndices.size()), GL_UNSIGNED_INT, reinterpret_cast<const void*>(0));

//    //GLDrawArrays for testing purposes
//    glBindVertexArray(mVAO);
//    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
//    glPointSize(10.0f);
    //    glDrawArrays(GL_POINTS, 0, mVertices.size());
}

PxRigidStatic *TriangleSurface::creatStaticPhysics(PxPhysics* physics, PxCooking* cooking, PxScene *scene, const char* name)
{
        PxTriangleMesh* triogem;
        PxTriangleMeshDesc triangleDescription;

        for(auto i = 0; i < mVertices.size(); i++)
        {
             PxReal px = mVertices[i].getX();
             PxReal py = mVertices[i].getY();
             PxReal pz = mVertices[i].getZ();

             pxVerticies.push_back(PxVec3(px,py,pz));

        }
        for(auto i = 0; i< mIndices.size(); i++)
        {
            pxIndecies.push_back(static_cast<PxU32>(mIndices[i]));
        }
       triangleDescription.points.count = (PxU32)pxVerticies.size();
       triangleDescription.points.stride = sizeof(PxVec3);
       triangleDescription.points.data = pxVerticies.data();

       triangleDescription.triangles.count = (PxU32)pxIndecies.size()/3;
       triangleDescription.triangles.stride = 3*sizeof(PxU32);
       triangleDescription.triangles.data = pxIndecies.data();


       PxDefaultMemoryOutputStream buffer;

       PxTriangleMeshCookingResult::Enum result;
       bool test=triangleDescription.isValid();
       if(test){
        cooking->validateTriangleMesh(triangleDescription);
        bool stat = cooking->cookTriangleMesh(triangleDescription, buffer, &result);
         if(!stat){
              std::cout << "cooking failed";
         }

        PxDefaultMemoryInputData bufferInput(buffer.getData(),buffer.getSize());

         triogem = physics->createTriangleMesh(bufferInput);
         mat = physics->createMaterial(0.5,0.5,0.5);

         PxVec3 position(mMatrix.column(3).x(),mMatrix.column(3).y(),mMatrix.column(3).z());

         PxTransform transform(position);
         PxTriangleMeshGeometry trigeo(triogem);
         mTerrain = PxCreateStatic(*physics,transform,trigeo,*mat);
         mTerrain->setName(name);
         scene->addActor(*mTerrain);
           return mTerrain;
        }else{
           std::cout << "ERROR: TriangleMeshDesc not valid";
           return nullptr;
       }
}

