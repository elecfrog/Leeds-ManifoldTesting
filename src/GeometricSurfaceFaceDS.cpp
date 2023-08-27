///////////////////////////////////////////////////
//
//	Hamish Carr
//	January, 2018
//
//	------------------------
//	GeometricSurfaceFaceDS.cpp
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
#include "GeometricSurfaceFaceDS.h"
#include <iostream>
#include <fstream>
#include <math.h>

// constructor will initialise to safe values
GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()
	{ // GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()
	// force the size to nil (should not be necessary, but . . .)
	vertices.resize(0);

	// set this to something reasonable
	boundingSphereSize = 1.0;
	
	// set the midpoint to the origin
	midPoint = Cartesian3(0.0, 0.0, 0.0);
} // GeometricSurfaceFaceDS::GeometricSurfaceFaceDS()

// read routine returns true on success, failure otherwise
bool GeometricSurfaceFaceDS::ReadFileTriangleSoup(char *fileName)
	{ // GeometricSurfaceFaceDS::ReadFileTriangleSoup()
	// these are for accumulating a bounding box for the object
	Cartesian3 minCoords(1000000.0, 1000000.0, 1000000.0);
	Cartesian3 maxCoords(-1000000.0, -1000000.0, -1000000.0);

    std::ifstream inFile(fileName);
    if (inFile.bad())
		return false;
	
	// set the midpoint to the origin
	midPoint = Cartesian3(0.0, 0.0, 0.0);

	// read in the number of vertices
    inFile >> nTriangles; // read the first line: the number of triangles
    nVertices = nTriangles * 3; // compute the number of vertices
    vertices.resize(nVertices); // allocate a meomery for vertices
	
	// now loop to read the vertices in, and hope nothing goes wrong
    for (int vertex = 0; vertex < nVertices; vertex++)
    { // for each vertex
        inFile >> vertices[vertex].x >> vertices[vertex].y >> vertices[vertex].z;
		// keep running track of midpoint, &c.
        midPoint = midPoint + vertices[vertex]; // add each position of vertex to the midpoint
        if (vertices[vertex].x < minCoords.x) minCoords.x = vertices[vertex].x;
        if (vertices[vertex].y < minCoords.y) minCoords.y = vertices[vertex].y;
        if (vertices[vertex].z < minCoords.z) minCoords.z = vertices[vertex].z;

        if (vertices[vertex].x > maxCoords.x) maxCoords.x = vertices[vertex].x;
        if (vertices[vertex].y > maxCoords.y) maxCoords.y = vertices[vertex].y;
        if (vertices[vertex].z > maxCoords.z) maxCoords.z = vertices[vertex].z;
    } // for each vertex

    vertices_f2f = vertices; // copy from vertices_soup to f2f dealings

	// now sort out the size of a bounding sphere for viewing
	// and also set the midpoint's location
    midPoint = midPoint / vertices.size();  // set the midpoint as the average point

	// now go back through the vertices, subtracting the mid point
	for (int vertex = 0; vertex < nVertices; vertex++)
    { // per vertex
        vertices[vertex] = vertices[vertex] - midPoint;
    } // per vertex

	// the bounding sphere radius is just half the distance between these
	boundingSphereSize = sqrt((maxCoords - minCoords).length()) * 1.0;
    return true;
}

void GeometricSurfaceFaceDS::manifold_test()
{// 2- mainfold (with boundary )detecting
    printf("2-MANIFOLD TEST\n");
    printf("START EDGE TEST\n");
    bool edge_valid = true;
    for(int edge_index =0; edge_index < edges.size(); ++edge_index)
    {
        int n_egde_appers=0;
        for(int face_index =0; face_index < faces_f2f.size(); ++face_index)
        {
            auto e0 = std::make_pair(faces_f2f[face_index].f0,faces_f2f[face_index].f1);
            auto e1 = std::make_pair(faces_f2f[face_index].f1,faces_f2f[face_index].f2);
            auto e2 = std::make_pair(faces_f2f[face_index].f2,faces_f2f[face_index].f0);
            auto e0_twin = std::make_pair(faces_f2f[face_index].f1,faces_f2f[face_index].f0);
            auto e1_twin = std::make_pair(faces_f2f[face_index].f2,faces_f2f[face_index].f1);
            auto e2_twin = std::make_pair(faces_f2f[face_index].f0,faces_f2f[face_index].f2);

            if(edges[edge_index] == e0 ||edges[edge_index] == e1 ||edges[edge_index] == e2
                ||edges[edge_index] == e0_twin ||edges[edge_index] == e1_twin ||edges[edge_index] == e2_twin)
                { n_egde_appers++; }
            if(n_egde_appers>2){break;}
        }
        if(n_egde_appers!=2)
        {
            edge_valid = false;
            printf("EDGE TEST: FAILD -> edge %i is not valid!\n",edge_index);
        }
    }
    if(edge_valid){printf("EDGE TEST: PASS\n");}

    printf("START VERT TEST\n");
    bool vert_valid = true;
    for(int vertex =0; vertex<vertices_f2f.size();vertex++)
    {
        std::vector<int> test_vert_set;
        for(int e_index=0;e_index<edges.size();e_index++)
        {// get each edge vertex orients the distenation
            if(edges[e_index].first == vertex) test_vert_set.push_back(edges[e_index].second);
        }
        bool local_valid = false;
        int n_hole = 0;
        for(int vertex_index=0;vertex_index<test_vert_set.size();vertex_index++)
        {// traverse each destianted vertex

            int current_vertex = vertex_index;
            int next_vertex = vertex_index+1;
            if(next_vertex == test_vert_set.size()) {next_vertex=0;}

            for(int edge_index=0;edge_index<edges.size();edge_index++)
            {
                if((test_vert_set[current_vertex]== edges[edge_index].first && test_vert_set[next_vertex]== edges[edge_index].second)
                    ||(test_vert_set[current_vertex]== edges[edge_index].second && test_vert_set[next_vertex]== edges[edge_index].first))
                {// loop situation
                    local_valid = true;
                    break;
                }
            }
            if(!local_valid){n_hole++;}
            if(n_hole>1){vert_valid = false; printf("VERT TEST: FAILD -> vertex %i is not valid!\n",vertex); break;}
        }
    }
    if(vert_valid){printf("VERT TEST: PASS\n");}
    if(edge_valid&&vert_valid){this->is_mainfold = true;printf("MAINFOLD TEST: SCUESS -> This is a 2-mainfold\n");}
    else{printf("MAINFOLD TEST: FALID -> This is not a 2-mainfold\n");}
}//
void GeometricSurfaceFaceDS::compute_genus()
{
    if(this->is_mainfold)
    {
        printf("Compute Genus\n");
        std::vector<std::vector<face>> faces_list;
        std::vector<std::vector<int>>  vertices_list;
        std::vector<face> tmp_faces;
        std::vector<int> tmp_vertices;
        std::vector<int> tmp;
        for(int f=0;f<faces_f2f.size();++f)
        {
            if(f==0)
            {
                tmp_faces.push_back({faces_f2f[0].f0,faces_f2f[0].f1,faces_f2f[0].f2});
                tmp_vertices.push_back(faces_f2f[0].f0);
                tmp_vertices.push_back(faces_f2f[0].f1);
                tmp_vertices.push_back(faces_f2f[0].f2);
//                tmp = tmp_vertices;
            }
            else if(f!=0)
            {   bool face_add = false;
                for(int i=0;i<tmp_vertices.size();++i)
                {
                    if(faces_f2f[f].f0 == tmp_vertices[i] || faces_f2f[f].f1 == tmp_vertices[i]||faces_f2f[f].f2 == tmp_vertices[i])
                    {
                        face_add = true;

                        if(!std::count(tmp_vertices.begin(),tmp_vertices.end(),faces_f2f[f].f0)){tmp_vertices.push_back(faces_f2f[f].f0);}
                        if(!std::count(tmp_vertices.begin(),tmp_vertices.end(),faces_f2f[f].f1)){tmp_vertices.push_back(faces_f2f[f].f1);}
                        if(!std::count(tmp_vertices.begin(),tmp_vertices.end(),faces_f2f[f].f2)){tmp_vertices.push_back(faces_f2f[f].f2);}
                        break;
                    }
                }
//                tmp = tmp_vertices;
                if(face_add){tmp_faces.push_back({faces_f2f[f].f0,faces_f2f[f].f1,faces_f2f[f].f2});}
                else if(!face_add)
                {
                    faces_list.push_back(tmp_faces);
                    vertices_list.push_back(tmp_vertices);
                    tmp_faces.resize(0);
                    tmp_faces.push_back({faces_f2f[f].f0,faces_f2f[f].f1,faces_f2f[f].f2});
                    tmp_vertices.resize(0);
                    tmp_vertices.push_back(faces_f2f[f].f0);
                    tmp_vertices.push_back(faces_f2f[f].f1);
                    tmp_vertices.push_back(faces_f2f[f].f2);
                }
                if(f==faces_f2f.size()-1)
                {
                    faces_list.push_back(tmp_faces);
                    vertices_list.push_back(tmp_vertices);
                }
            }
        }

        std::vector<std::vector<int>>  new_vert_list;
        new_vert_list.resize(vertices_list.size());
        for(int i =0; i<faces_list.size();++i)
        {
            for(int j =0; j<faces_list[i].size();++j)
            {
                new_vert_list[i].push_back(faces_list[i][j].f0);
                new_vert_list[i].push_back(faces_list[i][j].f1);
                new_vert_list[i].push_back(faces_list[i][j].f2);
            }
        }

        for(int i =0; i<new_vert_list.size();++i)
        {
            std::set<int>s(new_vert_list[i].begin(), new_vert_list[i].end());
            new_vert_list[i].assign(s.begin(), s.end());
        }

        for(int i =0; i<new_vert_list.size();++i)
        {
            auto vert_size = new_vert_list[i].size();
            auto face_size = faces_list[i].size();
            auto edge_size = face_size*3/2;
            this->n_genus += int((-vert_size + edge_size - face_size +2 )/2);
            printf("Piecewise Object %i Genus is %i\n",i,this->n_genus);
        }
        printf("Total Genus is %i\n",this->n_genus);
    }
}
void GeometricSurfaceFaceDS::f2f_index()
{
    if(this->nTriangles!=0 && (this->nVertices%3==0))
    {
        std::vector<Cartesian3> tmp; // tmp is a new list to push in f2f vertices info
        this->nFaces = this->nTriangles;
        for(int index =0; index<this->vertices_f2f.size();index++)
        {
            bool new_vertex = false;
            auto current_v = this->vertices_f2f[index];
            // load three vertices at a time.
            auto v0 = this->vertices_f2f[0];
            if(index==0){ tmp.push_back(v0); }

            for(int index =0;index<tmp.size();++index)
            {
                new_vertex = true;
                if(current_v==tmp[index]){ new_vertex = false; break;}
            }
            if(new_vertex){tmp.push_back(current_v);}
        }

        for(int i = 0; i<this->nFaces; i++)
        {
            auto v0 = vertices_f2f[i*3];
            auto v1 = vertices_f2f[i*3+1];
            auto v2 = vertices_f2f[i*3+2];
            int i0 = 0, i1 = 0, i2 = 0;

            for(int tmp_index = 0; tmp_index<tmp.size();++tmp_index)
            {
                if(v0 == tmp[tmp_index])
                {
                   i0 = tmp_index;
                }
            }
            for(int tmp_index = 0; tmp_index<tmp.size();++tmp_index)
            {

                if(v1 == tmp[tmp_index])
                {
                   i1 = tmp_index;
                }
            }
            for(int tmp_index = 0; tmp_index<tmp.size();++tmp_index)
            {
                if(v2 == tmp[tmp_index])
                {
                   i2 = tmp_index;
                }
            }
            faces_f2f.push_back({i0,i1,i2});
        }

        vertices_f2f.resize(tmp.size());
        vertices_f2f = tmp;
//        for(int i=0;i<vertices_f2f.size();i++)
//        {
//            qDebug()<<"vertices_f2f"<<vertices_f2f[i].x<<vertices_f2f[i].y<<vertices_f2f[i].z;
//        }
        std::vector<Cartesian3>().swap(tmp); //delete tmp safely
    }else{return;}
}// f2f index logic
void GeometricSurfaceFaceDS::fi2_directed_edge()
{
    if(faces_f2f.size()!=0)
    {// if faces exits dealing with the first direction edges(tasks)
        for(int index=0;index<faces_f2f.size();++index)
        {
            //get face indices from the face list
            auto i0 = faces_f2f[index].f0;
            auto i1 = faces_f2f[index].f1;
            auto i2 = faces_f2f[index].f2;

            // combine them to edge tuples
            edges.push_back(std::make_pair(i0,i1));
            edges.push_back(std::make_pair(i1,i2));
            edges.push_back(std::make_pair(i2,i0));
        }

        // init the size of first_dir_edge;
        first_dir_edge.resize(vertices_f2f.size());

        for(int index=0;index<edges.size();++index)
        {   // first directed edges
            // edges[index].first is the edgeTo vertex index
            // actually I get the last time edgeTo variable
            first_dir_edge[edges[index].first] = index;

            for(int j=0;j<edges.size();++j)
            {// do swap and make pairs
                auto tmp = std::make_pair(edges[j].first,edges[j].second);
                std::swap(tmp.first,tmp.second);
                if(edges[index] == tmp)
                {
                    other_half.push_back(j);
                }
            }
        }
    }else{return;}

}//face index 2 directed edges logic
void GeometricSurfaceFaceDS::write_f2f_file(QString fileName)
{// write f2f index to .face file
    f2f_index();
    QFileInfo info(fileName);
    QString obj_name = info.baseName();
    QString obj_sfx = info.suffix();
    if(obj_sfx!="tri")
    {
        qDebug()<<"ERROR: Write File Error";
        return;
    }
    QString out_file = "../models/"+ obj_name +".face";

    QString prefix_obj="# Object Name: " + obj_name + "\n"
                        +"# Vertices = " + QString::number(this->vertices_f2f.size())
                        + " Faces = " + QString::number(this->faces_f2f.size()) +"\n"
                        +"# \n";
    QFile file(out_file);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << prefix_info;
        stream << prefix_obj;
        for (int vertex = 0; vertex < vertices_f2f.size(); vertex++){ // per vertex output
            stream<<"Vertex"<<" "<<vertex
                  <<" "<<this->vertices_f2f[vertex].x
                  <<" "<<this->vertices_f2f[vertex].y
                  <<" "<<this->vertices_f2f[vertex].z
                  <<"\n";
        } // per vertex output
        for (int face = 0; face < faces_f2f.size(); face++){ // per face output
            stream<<"Face"
                  <<" "<<QString::number(face)
                  <<" "<<this->faces_f2f[face].f0
                  <<" "<<this->faces_f2f[face].f1
                  <<" "<<this->faces_f2f[face].f2
                  <<"\n";
         } // per face output
    }
}// write f2f index .face file to system

void GeometricSurfaceFaceDS::read_f2f_file(QString fileName)
{
    QFileInfo info(fileName);
    QString obj_name = info.baseName();
//    QString in_file = "../models/"+ fileName;
//    qDebug()<<in_file ;
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly)||info.suffix() != "face")
    {
         qWarning()<<"ERROR: Read File Error!\nUsage: _out_direction _file_name.face";
         return;
    }
    int vertex_count = 0;
    int face_count = 0;
    while (!file.atEnd())
    {
        QByteArray line = file.readLine().data();
        QString str(line);


        if(str.contains(sharp_sign)&&(!str.contains(object_name))&&(!str.contains(n_vertices)))
        {
            continue;
        }
        else if(str.contains(object_name))
        {
            str.chop(str.size() - str.lastIndexOf("\n"));
            QStringList str_list = str.split(" ");
            this->objectName = str_list[3];
            qDebug()<<"READ F2F: object name is :"<<this->objectName;
        }
        else if(str.contains(n_vertices)&&str.contains(n_faces))
        {

            QStringList str_list = str.split(" ");
            this->nVertices = str_list[3].toInt();
            this->nFaces = str_list[6].toInt();
            qDebug()<<"READ F2F: numbers of vertices is :"<<this->nVertices;
            qDebug()<<"READ F2F: numbers of faces is :"<<this->nFaces;
        }
        else if(str.startsWith(vertex_sign)&&vertex_count<this->nVertices)
        {
            str.chop(str.size() - str.lastIndexOf("\n"));
            QStringList str_list = str.split(" ");
            if(vertex_count == str_list[1].toInt())
            {
                float x = str_list[2].toFloat();
                float y = str_list[3].toFloat();
                float z = str_list[4].toFloat();
                vertices_f2f.push_back({x,y,z});
//                qDebug()<<"READ F2F: Vertex"<<vertex_count<<x<<y<<z;
                vertex_count++;
            }
        }
        else if(str.startsWith(face_sign)&&face_count<this->nFaces)
        {
            str.chop(str.size() - str.lastIndexOf("\n"));
            QStringList str_list = str.split(" ");
            if(face_count == str_list[1].toInt())
            {
                int x = str_list[2].toInt();
                int y = str_list[3].toInt();
                int z = str_list[4].toInt();
                faces_f2f.push_back({x,y,z});
//                qDebug()<<"READ F2F: Face"<<face_count<<x<<y<<z;
                face_count++;
            }
        }
    }

    fi2_directed_edge();
}// read f2f index .face file from system

void GeometricSurfaceFaceDS::write_diredge_file(QString fileName)
{// write directed/ half edge .diredge file to system
    f2f_index();
    fi2_directed_edge();
    QFileInfo info(fileName);
    QString obj_name = info.baseName();
    QString obj_sfx = info.suffix();
    if(obj_sfx!="tri")
    {
        qDebug()<<"ERROR: Write File Error";
        return;
    }
    QString out_file = "../models/"+ obj_name +".diredge";

    QString prefix_obj="# Object Name: " + obj_name + "\n"
                        +"# Vertices = " + QString::number(this->vertices_f2f.size())
                        + " Faces = " + QString::number(this->faces_f2f.size()) +"\n"
                        +"# \n";
    QFile file(out_file);
    if (file.open(QIODevice::ReadWrite)) {
        QTextStream stream(&file);
        stream << prefix_info;
        stream << prefix_obj;
        for (int vertex = 0; vertex < vertices_f2f.size(); vertex++){ // per vertex output
            stream<<"Vertex"<<" "<<vertex
                  <<" "<<this->vertices_f2f[vertex].x
                  <<" "<<this->vertices_f2f[vertex].y
                  <<" "<<this->vertices_f2f[vertex].z
                  <<"\n";
        } // per vertex output
        for (int fd_e = 0; fd_e < first_dir_edge.size(); fd_e++){ // per vertex output
            stream<<"FirstDirectedEdge"<<" "<<fd_e
                  <<" "<<this->first_dir_edge[fd_e]
                  <<"\n";
        } // per first directed edge
        for (int face = 0; face < faces_f2f.size(); face++){ // per face output
            stream<<"Face"
                  <<" "<<QString::number(face)
                  <<" "<<this->faces_f2f[face].f0
                  <<" "<<this->faces_f2f[face].f1
                  <<" "<<this->faces_f2f[face].f2
                  <<"\n";
         } // per face output
        for (int oh_e = 0; oh_e < other_half.size(); oh_e++){ // per vertex output
            stream<<"OtherHalf"<<" "<<oh_e
                  <<" "<<this->other_half[oh_e]
                  <<"\n";
        } // per first directed edge
    }
}// write directed/ half edge .diredge file to system
