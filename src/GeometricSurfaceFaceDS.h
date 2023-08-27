///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	GeometricSurfaceFaceDS.h
//	------------------------
//	
//	Base code for geometric assignments.
//
//	This is the minimalistic Face-based D/S for storing
//	surfaces, to be used as the basis for fuller versions
//	
//	It will include object load / save code & render code
//	
///////////////////////////////////////////////////
#ifndef _GEOMETRIC_SURFACE_FACE_DS_H
#define _GEOMETRIC_SURFACE_FACE_DS_H

#include <vector>
#include <map>
#include <utility>
#include <set>

#include <QString>
#include <QFile>
#include <QTextStream>
#include <QFileInfo>
#include <QDebug>

#include "Cartesian3.h"

const QString stu_name = QStringLiteral("ZHOU FANXING");
const unsigned long stu_number = 201598922;
const QString prefix_info = "# University of Leeds 2022-2023\n"
                      "# COMP 5812M Assignment 1\n"
                      "# " + stu_name + "\n"
                      "# " + QString::number(stu_number) + "\n"
                      "# \n";
const QString sharp_sign = QStringLiteral("#");
const QString object_name = QStringLiteral("Object Name");
const QString n_vertices = QStringLiteral("Vertices");
const QString n_faces = QStringLiteral("Faces");
const QString vertex_sign = QStringLiteral("Vertex");
const QString face_sign = QStringLiteral("Face");

class GeometricSurfaceFaceDS
	{ // class GeometricSurfaceFaceDS
    public:
	// vectors to store vertex and triangle information - relying on POD rule
    std::vector<Cartesian3> vertices; // each vertex is a vec3

    struct face{ int f0,f1,f2;};
    // std::vector<Cartesian3> vertices_soup; // write back in for soup method

//public objects
public:
    QString objectName;
    // triangle soup objects
    long nTriangles = 0;
    long nVertices = 0;
    long nFaces = 0;

    // f2f objects
    std::vector<Cartesian3> vertices_f2f; // each vertex is a vec3
    std::vector<face> faces_f2f; // faces should equals to nTriangles

    // directed edges objects
    std::vector<std::pair<int,int>> edges;
    std::vector<int> first_dir_edge; // each vertex is a vec3
    std::vector<int> other_half; // faces should equals to nTriangles
    std::vector<int> edge_dect;

    bool is_mainfold = false;

    int n_genus=0;

    int t = 0;


    //    QFile file;
	// bounding sphere size
	float boundingSphereSize;

	// midpoint of object
	Cartesian3 midPoint;

	// constructor will initialise to safe values
	GeometricSurfaceFaceDS();
	
	// read routine returns true on success, failure otherwise
	bool ReadFileTriangleSoup(char *fileName);

    // 2-mainfold with boundary detecting
    void manifold_test();
    // if an object is 2-mainfold, do this!
    void compute_genus();
    void get_face_and_vertex(int f0, int f1, int f2);
    // face 2 face index program logic
    void f2f_index();
    // face index 2 directed edges program logic
    void fi2_directed_edge();
    // write f2f index .face file to system
    void write_f2f_file(QString fileName);
    // read f2f index .face file from system
    void read_f2f_file(QString fileName);
    // write directed/ half edge .diredge file to system
    void write_diredge_file(QString fileName);


    // routine to render
    void Render();

}; // class GeometricSurfaceFaceDS


#endif
