#ifndef VERTEX_H
#define VERTEX_H

#include <iostream>
#include <fstream>
#include <QVector3D>

class Vertex {
    //! Overloaded ostream operator which writes all vertex data on an open textfile stream
    friend std::ostream& operator<< (std::ostream&, const Vertex&);
    //! Overloaded ostream operator which reads all vertex data from an open textfile stream
    friend std::istream& operator>> (std::istream&, Vertex&);

private:
   float m_xyz[3];
   float m_rgb[3];
   float m_uv[2];

public:
   Vertex();
   Vertex(float x, float y, float z, float r, float g, float b);
   Vertex(float x, float y, float z, float r, float g, float b, float u, float v);
   Vertex(QVector3D a, QVector3D b, QVector2D c);
   struct Triangle
   {
       Triangle(int i0, int i1, int i2, int n0, int n1, int n2);
       int indexes[3];
       int neighbours[3];
   };
   float getX();
   float getY();
   float getZ();
   void setXYZ(float x, float y, float z);
   void setNormal(QVector3D normal);
};

#endif // VERTEX_H
