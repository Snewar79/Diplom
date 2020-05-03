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
        std::vector<glm::vec3> & out_faces,
        std::vector<std::vector<glm::vec3>>& true_faces
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

        for (int i = 0; i < temp_faces.size(); i++)
        {
            std::cout << "process [" << i << "] face\n";
            std::cout << temp_faces[i][0] << " ";
            std::cout << temp_faces[i][1] << " ";
            std::cout << temp_faces[i][2] << "\n";

            std::vector<glm::vec3> temp;
            temp.push_back(out_vertices[temp_faces[i][0] - 1]);
            temp.push_back(out_vertices[temp_faces[i][1] - 1]);
            temp.push_back(out_vertices[temp_faces[i][2] - 1]);

            true_faces.push_back(temp);
        }

        cetrize(true_faces);

        return true;
    }

    void cetrize(std::vector<std::vector<glm::vec3>>& true_faces)
    {
        double max_x, min_x;
        double max_y, min_y;
        double max_z, min_z;

        max_x = true_faces[0][0][0];
        min_x = true_faces[0][0][0];

        max_y = true_faces[0][0][1];
        min_y = true_faces[0][0][1];

        max_z = true_faces[0][0][2];
        min_z = true_faces[0][0][2];

        for (int i = 0; i < true_faces.size(); i++)
        {
            for (int point = 0; point < 3; point++)
            {
                if (true_faces[i][point][0] > max_x)
                    max_x = true_faces[i][point][0];

                if (true_faces[i][point][0] < min_x)
                    min_x = true_faces[i][point][0];

                if (true_faces[i][point][1] > max_y)
                    max_y = true_faces[i][point][1];

                if (true_faces[i][point][1] < min_y)
                    min_y = true_faces[i][point][1];

                if (true_faces[i][point][2] > max_z)
                    max_z = true_faces[i][point][2];

                if (true_faces[i][point][2] < min_z)
                    min_z = true_faces[i][point][2];


            }
        }

        std::cout << "x == " << max_x - min_x << "\n";
        std::cout << "y == " << max_y - min_y << "\n";
        std::cout << "y == " << max_z - min_z << "\n";

        double x_length = (max_x - min_x) / 2;
        double y_length = (max_y - min_y) / 2;
        double z_length = (max_z - min_z) / 2;

        for (int i = 0; i < true_faces.size(); i++)
        {
            for (int point = 0; point < 3; point++)
            {
                true_faces[i][point][0] = true_faces[i][point][0] - (max_x - x_length);
                true_faces[i][point][1] = true_faces[i][point][1] - (max_y - y_length);
                true_faces[i][point][2] = true_faces[i][point][2] - (max_z - z_length);
            }
        }

        double max = x_length;
        if (max < y_length)
            max = y_length;
        if (max < z_length)
            max = z_length;

        max *= 2;

        std::cout << "max length = " << max << "\n";

        for (int i = 0; i < true_faces.size(); i++)
        {
            for (int point = 0; point < 3; point++)
            {
                true_faces[i][point][0] /= max;
                true_faces[i][point][1] /= max;
                true_faces[i][point][2] /= max;
            }
        }

        //checkAll(true_faces);
    }

    double module_vector(glm::vec3 a)
    {
        return sqrt(a[0] * a[0] + a[1] * a[1] + a[2] * a[2]);
    }

    double trinagle_size(glm::vec3 a, glm::vec3 b, glm::vec3 c)
    {
        double a_side = module_vector(b - a);
        double b_side = module_vector(c - b);
        double c_side = module_vector(a - c);

        //std::cout << a_side << "\n";

        double p = (a_side + c_side + b_side) / 2;

        return sqrt(p * (p - a_side) * (p - b_side) * (p - c_side));
    }


    bool isCross(glm::vec3 a, glm::vec3 b, glm::vec3 c, glm::vec3 x, glm::vec3 r)
    {
        a = a - x;
        b = b - x;
        c = c - x; 
        double p12 = b[0] - a[0];
        double p22 = b[1] - a[1];
        double p32 = b[2] - a[2];

        double p13 = c[0] - a[0];
        double p23 = c[1] - a[1];
        double p33 = c[2] - a[2];

        double ac, bc, cc, dc;
        ac = p22 * p33 - p23 * p32;
        bc = p13 * p32 - p12 * p33;
        cc = p12 * p23 - p13 * p22;
        dc =    a[0] * (p23 * p32 - p22 * p33) +
                a[1] * (p12 * p33 - p13 * p32) +
                a[2] * (p13 * p22 - p12 * p23);

        //std::cout << "dc == " << dc << "\n";

        double determ = (ac * r[0] + bc * r[1] + cc * r[2]);

        if (abs(determ) < 0.000001)
        {
            return false;
        }

        //std::cout << "determ == " << determ << "\n";

        double t = - dc / (ac * r[0] + bc * r[1] + cc * r[2]);

        //std::cout << "t  == " << t << "\n";

        if (t < 0)
            return false;

        glm::vec3 point = r;

        point *= t;

        point = point + x;
        a += x;
        b += x;
        c += x;

        //std::cout << "cross_point = " << point[0] << " " << point[1] << " " << point[2] << "\n";

        double abc = trinagle_size(a, b, c);
        double abd = trinagle_size(a, b, point);
        double bcd = trinagle_size(point, b, c);
        double acd = trinagle_size(a, point, c);

        //std::cout << " s1 = " << abc << " s2 = " << abd + bcd + acd << "\n";

        if (abs(abc - (abd + bcd + acd)) < 0.0001){
            //std:: cout << "return true\n";
            return true;
        }

        return false;
    }

    bool check_point(glm::vec3 point, std::vector<std::vector<glm::vec3>> true_faces){

        int inside = 0;
        int outside = 0;

        glm::vec3 r;
        for (double x = -0.5; x < 0.5; x += 0.2){
            for (double y = -0.5; y < 0.5; y += 0.2){
                for (double z = -0.5; z < 0.5; z += 0.2){
                    r = glm::vec3({x, y, z});
                        int cross_count = 0;
                        for (int i = 0; i < true_faces.size(); i++){
                            if (isCross(true_faces[i][0], true_faces[i][1], true_faces[i][2], point, r))
                            {
                                cross_count += 1;
                            }
                        }

                        if (cross_count % 2 == 1)
                        {
                            inside += 1;
                        }
                        else
                        {
                            outside += 1;
                        }

                }
            }
        }

        //std::cout << "inside = " << inside << " outside = " << outside << "\n";

        if ( inside > outside)
        {
            return true;
        }

        return false;
    }

    std::vector<std::vector<std::vector<bool>>> segmentation(std::vector<std::vector<glm::vec3>>& true_faces, int side){

        std::vector<std::vector<std::vector<bool>>> result = std::vector<std::vector<std::vector<bool>>>(side, std::vector<std::vector<bool>>(side, std::vector<bool>(side, false)));
        std::cout << "segmentation is start\n";
        for (int x = 0; x < side; x++)
        for (int y = 0; y < side; y++)
        for (int z = 0; z < side; z++){
            double px = -0.5 + (double)(x) / side;
            double py = -0.5 + (double)(y) / side;
            double pz = -0.5 + (double)(z) / side;
            glm::vec3 point = {px, py, pz};
            if (check_point(point, true_faces))
            {
                //std::cout << "add\n";
                result[x][y][z] = true;
            }
                
        }
        std::cout << "segmentation is end\n";

        return result;
    }

};