#include "objectmesh.h"
#include "sstream"

ObjectMesh::ObjectMesh(std::string fileName,GLuint ShaderId, GLuint TextureId)
{
    mShaderId=ShaderId;
    mTextureId=TextureId;
    readFile(fileName);


    mMatrix.setToIdentity();

}

ObjectMesh::~ObjectMesh()
{

}



void ObjectMesh::init(GLint matrixUniform)
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
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE,  sizeof(Vertex),  reinterpret_cast<const void*>(3 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(1);

        // 3nd attribute buffer : S T
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE,  sizeof( Vertex ),  (GLvoid*)(6 * sizeof(GLfloat)) );
        glEnableVertexAttribArray(2);

        glGenBuffers(1, &mIBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(GLuint), mIndices.data(), GL_STATIC_DRAW);

        glBindVertexArray(0);
}

void ObjectMesh::readFile(std::string fileName)
{
    std::ifstream fileIn;
    fileIn.open(fileName, std::ifstream::in);
    if(!fileIn)
        qDebug() << "Could Not open file for reading: "<<QString::fromStdString(fileName);
    std::string oneLine;
    std::string oneWord;
    std::vector<QVector3D> tempVertices;
    std::vector<QVector3D> tempNormals;
    std::vector<QVector2D> tempUVs;

    unsigned int temp_index = 0;
    while(std::getline(fileIn,oneLine))
    {
        std::stringstream sStream;
        sStream << oneLine;
        oneWord ="";
        sStream >>oneWord;
        if(oneWord == "#")
        {
            continue;
        }
        if (oneWord == "")
        {
            continue;
        }
        if(oneWord == "v")
        {
            QVector3D tempVertex;
            sStream >> oneWord;
            tempVertex.setX(std::stof(oneWord));
            sStream >>oneWord;
            tempVertex.setY(std::stof(oneWord));
            sStream >>oneWord;
            tempVertex.setZ(std::stof(oneWord));
            tempVertices.push_back(tempVertex);

            continue;
        }
        if(oneWord == "vt")
        {
            QVector2D tempUV;
            sStream >> oneWord;
            tempUV.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempUV.setY(std::stof(oneWord));
            tempUVs.push_back(tempUV);

            continue;
        }
        if(oneWord == "vn")
        {
            QVector3D tempNormal;
            sStream >> oneWord;
            tempNormal.setX(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setY(std::stof(oneWord));
            sStream >> oneWord;
            tempNormal.setZ(std::stof(oneWord));
            tempNormals.push_back(tempNormal);

            continue;
        }
        if(oneWord == "f")
        {
            int index, normal, uv;
            for(int i = 0; i<3; i++)
            {
                sStream >> oneWord;
                std::stringstream tempWord(oneWord);
                std::string segment;
                std::vector<std::string> segmentArray;
                while(std::getline(tempWord, segment, '/'))
                    segmentArray.push_back(segment);
                index =std::stoi(segmentArray[0]);
                if(segmentArray[1] !="")
                    uv = std::stoi(segmentArray[1]);
                else
                    uv=0;
                normal = std::stoi(segmentArray[2]);
                index--;
                uv--;
                normal--;
                if(uv>-1)
                {
                    Vertex tempVertex(tempVertices[index],tempNormals[normal],tempUVs[uv]);
                    mVertices.push_back(tempVertex);
                }
                else
                {
                    Vertex tempVertex(tempVertices[index],tempNormals[normal],QVector2D{0,0});
                    mVertices.push_back(tempVertex);
                }
                mIndices.push_back(temp_index++);
            }
            continue;
        }

    }
    fileIn.close();
}

void ObjectMesh::draw()
{
    glBindVertexArray( mVAO );
    glUniformMatrix4fv( mMatrixUniform, 1, GL_FALSE, mMatrix.constData());
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, nullptr);
}
