#pragma once
#include <glm/glm.hpp>
#include <fstream>
#include <vector>
//g++ main.cpp -o test -lglut -lGLU -lGL

class model_parser{

public:
    model_parser(){}

    bool loadOBJ(
        const char * path,
        std::vector<glm::vec3> & out_vertices,
        std::vector<glm::vec3> & out_faces
    ){

        std::vector< unsigned int > vertexIndices, uvIndices, normalIndices;
        std::vector< glm::vec3 > temp_vertices;
        std::vector< glm::vec3 > temp_faces;

        FILE * file = fopen(path, "r");
        if( file == NULL ){
            printf("Impossible to open the file !\n");
            return false;
        }

        while( true ){
            char lineHeader[128];

            int res = fscanf(file, "%s", lineHeader);
            if (res == EOF)
                break;

            if ( strcmp( lineHeader, "v" ) == 0 ){
                glm::vec3 vertex;
                fscanf(file, "%f %f %f\n", &vertex.x, &vertex.y, &vertex.z );
                temp_vertices.push_back(vertex);
            }
            else if ( strcmp( lineHeader, "f" ) == 0 ){
                glm::vec3 vertex;
                unsigned int vertexIndex[3], uvIndex[3], normalIndex[3];
                int matches = fscanf(file, "%d/%d/%d %d/%d/%d %d/%d/%d\n", &vertexIndex[0], &uvIndex[0], &normalIndex[0], &vertexIndex[1], &uvIndex[1], &normalIndex[1], &vertexIndex[2], &uvIndex[2], &normalIndex[2] );
                if (matches != 9){
                    printf("File can't be read by our simple parser : ( Try exporting with other options\n");
                    return false;
                }

                vertex.x = vertexIndex[0];
                vertex.y = vertexIndex[1];
                vertex.z = vertexIndex[2];
                temp_faces.push_back(vertex);
            }
            
        }
        
        out_vertices = temp_vertices;
        out_faces = temp_faces;

        return true;
    }

};