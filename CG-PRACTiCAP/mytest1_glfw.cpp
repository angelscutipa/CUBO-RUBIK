#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <vector>
#include <cstdlib>
#include <unistd.h>
#include <stdio.h>      /* printf, scanf, puts, NULL */
#include <stdlib.h>     /* srand, rand */
#include <time.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "matriz.cpp"

#include <iostream>
#include <fstream>


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;


using namespace std;

matriz trans;
glm::mat4 view = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first


float alpha = 0;

float eyex = 1;
float eyey = 1;
float eyez = 1;

float v_eye[3];


float up[3];

float vc[3];
int dim=1;

vector<string> nombres;
vector<string> n_color;
vector<string> l_giros_alg;
string Cgiros;

std::vector<std::string> split(const std::string& s, char delimiter)
{
   std::vector<std::string> tokens;
   std::string token;
   std::istringstream tokenStream(s);
   while (std::getline(tokenStream, token, delimiter))
   {
      tokens.push_back(token);
   }
   return tokens;
}

string leer_archivo(){
    std::ifstream infile("SomeFileC.txt");
    std::string line;
    string ret;
    while (std::getline(infile, line))
    {
        ret=line;
    }
    return ret;
}
unsigned int texture1s[7], texture2s[7];

void controls(GLFWwindow* window, int key, int scancode, int action, int mods);

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    if(yoffset>0) {
        if (v_eye[0]!=0)
        {
          if (v_eye[0]>0)
          {
            v_eye[0]=v_eye[0]-0.05;
          }
          else if (v_eye[0]<0)
          {
            v_eye[0]=v_eye[0]+0.05;
          }
        }
        if (v_eye[1]!=0)
        {
          if (v_eye[1]>0)
          {
            v_eye[1]=v_eye[1]-0.05;
          }
          else if (v_eye[1]<0)
          {
            v_eye[1]=v_eye[1]+0.05;
          }
        }
        if (v_eye[2]!=0)
        {
          if (v_eye[2]>0)
          {
            v_eye[2]=v_eye[2]-0.05;
          }
          else if (v_eye[2]<0)
          {
            v_eye[2]=v_eye[2]+0.05;
          }
        }
    }

    if(yoffset<0) {
        if (v_eye[0]!=0)
        {
          if (v_eye[0]>0)
          {
            v_eye[0]=v_eye[0]+0.05;
          }
          else if (v_eye[0]<0)
          {
            v_eye[0]=v_eye[0]-0.05;
          }
        }
        if (v_eye[1]!=0)
        {
          if (v_eye[1]>0)
          {
            v_eye[1]=v_eye[1]+0.05;
          }
          else if (v_eye[1]<0)
          {
            v_eye[1]=v_eye[1]-0.05;
          }
        }
        if (v_eye[2]!=0)
        {
          if (v_eye[2]>0)
          {
            v_eye[2]=v_eye[2]+0.05;
          }
          else if (v_eye[2]<0)
          {
            v_eye[2]=v_eye[2]-0.05;
          }
        }
    }

}

void Adar_textura(){
    // texture 1
    for (int i = 0; i < 7; ++i)
    {
        glGenTextures(1, &texture1s[i]);
        glBindTexture(GL_TEXTURE_2D, texture1s[i]);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
        unsigned char *data = stbi_load(nombres[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);
        // texture 2
        // ---------
        
        glGenTextures(1, &texture2s[i]);
        glBindTexture(GL_TEXTURE_2D, texture2s[i]);
        // set the texture wrapping parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        // set texture filtering parameters
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        // load image, create texture and generate mipmaps
        data = stbi_load(n_color[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {   
            // note that the awesomeface.png has transparency and thus an alpha channel, so make sure to tell OpenGL the data type is of GL_RGBA
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
            
        }
        else
        {
            std::cout << "Failed to load texture" << std::endl;
        }
        stbi_image_free(data);

        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        // -------------------------------------------------------------------------------------------
    } 
}

class cubo
{
public:
    unsigned int VBO[6], VAO[6], EBO[6];
    float l=0.5;
    float xi, yi, zi;
    Shader *ourShader;
    unsigned int texture1[6], texture2[6];
    GLfloat vertices[72];
    GLfloat colors[72];
    float vertCompletos[6][30];
    unsigned char *data;
    cubo(float _xi, float _yi, float _zi, float _ini){
        ourShader=new Shader("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/camera.vs", "/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/camera.fs");

        // set up vertex data (and buffer(s)) and configure vertex attributes
        // ------------------------------------------------------------------
        xi=_xi;     yi=_yi;     zi=_zi;
        for (int i = 0; i < 72; ++i)
        {
            vertices[i]=0.0;
            colors[i]=0.0;
        }

        vertices[0]=xi;     vertices[1]=yi;     vertices[2]=zi;     vertices[3]=xi-l;       vertices[4]=yi;     vertices[5]=zi;     vertices[6]=xi-l;   vertices[7]=yi-l;       vertices[8]=zi;     vertices[9]=xi;     vertices[10]=yi-l;      vertices[11]=zi;
        vertices[12]=xi;    vertices[13]=yi;    vertices[14]=zi+l;  vertices[15]=xi-l;      vertices[16]=yi;    vertices[17]=zi+l;  vertices[18]=xi-l;  vertices[19]=yi-l;      vertices[20]=zi+l;  vertices[21]=xi;    vertices[22]=yi-l;      vertices[23]=zi+l;
        vertices[24]=xi;     vertices[25]=yi;     vertices[26]=zi;  vertices[27]=xi-l;      vertices[28]=yi;    vertices[29]=zi;    vertices[30]=xi-l;  vertices[31]=yi;        vertices[32]=zi+l;  vertices[33]=xi;    vertices[34]=yi;        vertices[35]=zi+l; 
        vertices[36]=xi-l;   vertices[37]=yi-l;   vertices[38]=zi;  vertices[39]=xi;        vertices[40]=yi-l;  vertices[41]=zi;    vertices[42]=xi;    vertices[43]=yi-l;      vertices[44]=zi+l;  vertices[45]=xi-l;  vertices[46]=yi-l;      vertices[47]=zi+l;
        vertices[48]=xi;     vertices[49]=yi;     vertices[50]=zi;  vertices[51]=xi;        vertices[52]=yi;    vertices[53]=zi+l;  vertices[54]=xi;    vertices[55]=yi-l;      vertices[56]=zi+l;  vertices[57]=xi;    vertices[58]=yi-l;      vertices[59]=zi;
        vertices[60]=xi-l;   vertices[61]=yi;     vertices[62]=zi;  vertices[63]=xi-l;      vertices[64]=yi;    vertices[65]=zi+l;  vertices[66]=xi-l;  vertices[67]=yi-l;      vertices[68]=zi+l;  vertices[69]=xi-l;  vertices[70]=yi-l;      vertices[71]=zi;
        
        dar_textura();
        combinar_ver_tc();
        dibujar_col_tex();
        dar_color(_ini);
    }
    void combinar_ver_tc(){
        for (int i = 0; i < 6; ++i)
        {
            float vtem[30];
            float verPart[12];
            float colorPart[12];
            int ipos=0;
            for (int j = i*12; j < (i*12)+12; ++j)
            {
                verPart[ipos]=vertices[j];
                colorPart[ipos]=colors[j];
                ipos++;    
            }
            vertCompletos[i][0]=verPart[0];         vertCompletos[i][1]=verPart[1];        vertCompletos[i][2]=verPart[2];     vertCompletos[i][3]=1.0;     vertCompletos[i][4]=1.0;
            vertCompletos[i][5]=verPart[3];         vertCompletos[i][6]=verPart[4];        vertCompletos[i][7]=verPart[5];     vertCompletos[i][8]=0.0;     vertCompletos[i][9]=1.0;
            vertCompletos[i][10]=verPart[9];        vertCompletos[i][11]=verPart[10];      vertCompletos[i][12]=verPart[11];   vertCompletos[i][13]=1.0;    vertCompletos[i][14]=0.0;
            vertCompletos[i][15]=verPart[9];        vertCompletos[i][16]=verPart[10];      vertCompletos[i][17]=verPart[11];   vertCompletos[i][18]=1.0;    vertCompletos[i][19]=0.0;
            vertCompletos[i][20]=verPart[6];        vertCompletos[i][21]=verPart[7];       vertCompletos[i][22]=verPart[8];    vertCompletos[i][23]=0.0;    vertCompletos[i][24]=0.0;
            vertCompletos[i][25]=verPart[3];        vertCompletos[i][26]=verPart[4];       vertCompletos[i][27]=verPart[5];    vertCompletos[i][28]=0.0;    vertCompletos[i][29]=1.0;       
        }
    }
    void cambiar_giro(){
        for (int i = 0; i < 6; ++i)
        {
            float vtem[30];
            float verPart[12];
            float colorPart[12];
            int ipos=0;
            for (int j = i*12; j < (i*12)+12; ++j)
            {
                verPart[ipos]=vertices[j];
                colorPart[ipos]=colors[j];
                ipos++;    
            }
            vertCompletos[i][0]=verPart[0];         vertCompletos[i][1]=verPart[1];        vertCompletos[i][2]=verPart[2];     
            vertCompletos[i][5]=verPart[3];         vertCompletos[i][6]=verPart[4];        vertCompletos[i][7]=verPart[5];     
            vertCompletos[i][10]=verPart[9];        vertCompletos[i][11]=verPart[10];      vertCompletos[i][12]=verPart[11];   
            vertCompletos[i][15]=verPart[9];        vertCompletos[i][16]=verPart[10];      vertCompletos[i][17]=verPart[11];   
            vertCompletos[i][20]=verPart[6];        vertCompletos[i][21]=verPart[7];       vertCompletos[i][22]=verPart[8];    
            vertCompletos[i][25]=verPart[3];        vertCompletos[i][26]=verPart[4];       vertCompletos[i][27]=verPart[5];           
        }
    }
    void dar_textura(){
        // texture 1
        for (int i = 0; i < 6; ++i)
        {
            texture1[i]=texture1s[i];
            texture2[i]=texture2s[i];
            ourShader->use();
            ourShader->setInt("texture1", 0);
            ourShader->setInt("texture2", 1);

            // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
            // -----------------------------------------------------------------------------------------------------------
            glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
            ourShader->setMat4("projection", projection);
        } 
    }
    void dar_texturaOsc(int cara){
        // texture 1
        texture1[cara]=texture1s[6];
        texture2[cara]=texture2s[6];

        // tell opengl for each sampler to which texture unit it belongs to (only has to be done once)
        // -------------------------------------------------------------------------------------------
        ourShader->use();
        ourShader->setInt("texture1", 0);
        ourShader->setInt("texture2", 1);

        // pass projection matrix to shader (as projection matrix rarely changes there's no need to do this per frame)
        // -----------------------------------------------------------------------------------------------------------
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        ourShader->setMat4("projection", projection);
    }
    void dar_color(float xini){
        GLfloat colores[]={
            1, 0, 0,      1, 0, 0,      1, 0, 0,      0, 0, 0,
            0, 1, 0,      0, 1, 0,      0, 1, 0,      0, 0, 0,
            0, 0, 1,      0, 0, 1,      0, 0, 1,      0, 0, 0,
            1, 1, 1,      1, 1, 1,      1, 1, 1,      0, 0, 0,
            1, 0.5, 0,    1, 0.5, 0,    1, 0.5, 0,    0, 0, 0,
            1, 1, 0,      1, 1, 0,      1, 1, 0,      0, 0, 0
        };

        int ind=0;

        for (int i = 0; i < 72; i=i+12)
        {
            ind=i/12;
            float inv[12];
            for (int j = 0; j < 12; ++j)
            {
                inv[j]=0;
            }
            int pos=0;
            for (int j = i; j < i+12; ++j)
            {
                inv[pos]=vertices[j];
                pos++;
            }
            if (comprobar(inv, xini)==7)
            {
                dar_texturaOsc(ind);
            }

        }
        
    }
    int comprobar(float vec[12], float xini){
        if ((vec[0]==xini) && (vec[3]==xini) && (vec[6]==xini) && (vec[9]==xini))       return 0;

        else if (vec[0]==-xini && vec[3]==-xini && vec[6]==-xini && vec[9]==-xini)      return 1;

        else if ((vec[1]==xini) && (vec[4]==xini) && (vec[7]==xini) && (vec[10]==xini)) return 2;

        else if (vec[1]==-xini && vec[4]==-xini && vec[7]==-xini && vec[10]==-xini)     return 3;

        else if ((vec[2]==xini) && (vec[5]==xini) && (vec[8]==xini) && (vec[11]==xini)) return 4;

        else if (vec[2]==-xini && vec[5]==-xini && vec[8]==-xini && vec[11]==-xini)     return 5;

        else return 7;
    }

    void cambiar_coord(int cara, float inix, float iniy){
        float div=0.33333333;
        float vtem[30];
        float verPart[12];
        float colorPart[12];
        int ipos=0;
        for (int j = cara*12; j < (cara*12)+12; ++j)
        {
            verPart[ipos]=vertices[j];
            colorPart[ipos]=colors[j];
            ipos++;    
        }

        vertCompletos[cara][0]=verPart[0];         vertCompletos[cara][1]=verPart[1];        vertCompletos[cara][2]=verPart[2];     vertCompletos[cara][3]=inix+div;     vertCompletos[cara][4]=iniy+div;
        vertCompletos[cara][5]=verPart[3];         vertCompletos[cara][6]=verPart[4];        vertCompletos[cara][7]=verPart[5];     vertCompletos[cara][8]=inix;         vertCompletos[cara][9]=iniy+div;
        vertCompletos[cara][10]=verPart[9];        vertCompletos[cara][11]=verPart[10];      vertCompletos[cara][12]=verPart[11];   vertCompletos[cara][13]=inix+div;    vertCompletos[cara][14]=iniy;
        vertCompletos[cara][15]=verPart[9];        vertCompletos[cara][16]=verPart[10];      vertCompletos[cara][17]=verPart[11];   vertCompletos[cara][18]=inix+div;    vertCompletos[cara][19]=iniy;
        vertCompletos[cara][20]=verPart[6];        vertCompletos[cara][21]=verPart[7];       vertCompletos[cara][22]=verPart[8];    vertCompletos[cara][23]=inix;        vertCompletos[cara][24]=iniy;
        vertCompletos[cara][25]=verPart[3];        vertCompletos[cara][26]=verPart[4];       vertCompletos[cara][27]=verPart[5];    vertCompletos[cara][28]=inix;        vertCompletos[cara][29]=iniy+div;

        glGenVertexArrays(1, &VAO[cara]);
        glGenBuffers(1, &VBO[cara]);

        glBindVertexArray(VAO[cara]);

        glBindBuffer(GL_ARRAY_BUFFER, VBO[cara]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertCompletos[cara]), vertCompletos[cara], GL_STATIC_DRAW);

        // position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        // texture coord attribute
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
        // load and create a texture 
        // -------------------------

    }
    void dibujar_col_tex(){
        for (int i = 0; i < 6; ++i)
        {
            /*
            vtem[0]=verPart[0];         vtem[1]=verPart[1];        vtem[2]=verPart[2];     vtem[3]=colorPart[0];        vtem[4]=colorPart[1];         vtem[5]=colorPart[2];     vtem[6]=1.0;     vtem[7]=1.0;
            vtem[8]=verPart[3];         vtem[9]=verPart[4];        vtem[10]=verPart[5];     vtem[11]=colorPart[3];        vtem[12]=colorPart[4];         vtem[13]=colorPart[5];    vtem[14]=0.0;     vtem[15]=1.0;
            vtem[16]=verPart[9];        vtem[17]=verPart[10];      vtem[18]=verPart[11];   vtem[19]=colorPart[9];       vtem[20]=colorPart[10];       vtem[21]=colorPart[11];   vtem[22]=1.0;    vtem[23]=0.0;
            vtem[24]=verPart[9];        vtem[25]=verPart[10];      vtem[26]=verPart[11];   vtem[27]=colorPart[9];        vtem[28]=colorPart[10];       vtem[29]=colorPart[11];   vtem[30]=1.0;    vtem[31]=0.0;
            vtem[32]=verPart[6];        vtem[33]=verPart[7];       vtem[34]=verPart[8];    vtem[35]=colorPart[6];       vtem[36]=colorPart[7];        vtem[37]=colorPart[8];    vtem[38]=0.0;    vtem[39]=0.0;
            vtem[40]=verPart[3];        vtem[41]=verPart[4];       vtem[42]=verPart[5];    vtem[43]=colorPart[3];        vtem[44]=colorPart[4];         vtem[45]=colorPart[5];    vtem[46]=0.0;    vtem[47]=1.0;       
            

            vtem[0]=verPart[0];         vtem[1]=verPart[1];        vtem[2]=verPart[2];     vtem[3]=colorPart[0];        vtem[4]=colorPart[1];         vtem[5]=colorPart[2];     vtem[6]=1.0;     vtem[7]=1.0;
            vtem[8]=verPart[3];         vtem[9]=verPart[4];        vtem[10]=verPart[5];     vtem[11]=colorPart[3];        vtem[12]=colorPart[4];         vtem[13]=colorPart[5];    vtem[14]=0.0;     vtem[15]=1.0;
            vtem[16]=verPart[9];        vtem[17]=verPart[10];      vtem[18]=verPart[11];   vtem[19]=colorPart[9];       vtem[20]=colorPart[10];       vtem[21]=colorPart[11];   vtem[22]=1.0;    vtem[23]=0.0;
            vtem[24]=verPart[6];        vtem[25]=verPart[7];       vtem[26]=verPart[8];    vtem[27]=colorPart[6];       vtem[28]=colorPart[7];        vtem[29]=colorPart[8];    vtem[30]=0.0;    vtem[31]=0.0;
            */
            
            glGenVertexArrays(1, &VAO[i]);
            glGenBuffers(1, &VBO[i]);

            glBindVertexArray(VAO[i]);

            glBindBuffer(GL_ARRAY_BUFFER, VBO[i]);
            glBufferData(GL_ARRAY_BUFFER, sizeof(vertCompletos[i]), vertCompletos[i], GL_STATIC_DRAW);

            // position attribute
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(0);
            // texture coord attribute
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
            glEnableVertexAttribArray(1);
            // load and create a texture 
            // -------------------------
          
        }
    }

    void dibujar(){
        // bind Texture
        for (int i = 0; i < 6; ++i)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture1[i]);
            glActiveTexture(GL_TEXTURE1);
            glBindTexture(GL_TEXTURE_2D, texture2[i]);


            ourShader->use();
            ourShader->setMat4("view", view);


            glBindVertexArray(VAO[i]);
            glm::mat4 model = glm::mat4(1.0f);

            ourShader->setMat4("model", model);

            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

    }
    void rotarZ(float angulo){
        float vec[4];
        for (int i = 0; i < 24; ++i)
        {
            vec[0]=vertices[(i*3)+0];
            vec[1]=vertices[(i*3)+1];
            vec[2]=vertices[(i*3)+2];
            vec[4]=0;

            trans.mul_rotZ(vec, angulo);

            vertices[(i*3)+0]=vec[0];
            vertices[(i*3)+1]=vec[1];
            vertices[(i*3)+2]=vec[2];
        }
        cambiar_giro();
        dibujar_col_tex();
    }
    void rotarY(float angulo){
        float vec[4];
        for (int i = 0; i < 24; ++i)
        {
            vec[0]=vertices[(i*3)+0];
            vec[1]=vertices[(i*3)+1];
            vec[2]=vertices[(i*3)+2];
            vec[4]=0;

            trans.mul_rotY(vec, angulo);

            vertices[(i*3)+0]=vec[0];
            vertices[(i*3)+1]=vec[1];
            vertices[(i*3)+2]=vec[2];
        }
        cambiar_giro();
        dibujar_col_tex();
    }

    void rotarX(float angulo){
        float vec[4];
        for (int i = 0; i < 24; ++i)
        {
            vec[0]=vertices[(i*3)+0];
            vec[1]=vertices[(i*3)+1];
            vec[2]=vertices[(i*3)+2];
            vec[4]=0;

            trans.mul_rotX(vec, angulo);

            vertices[(i*3)+0]=vec[0];
            vertices[(i*3)+1]=vec[1];
            vertices[(i*3)+2]=vec[2];
        }
        cambiar_giro();
        dibujar_col_tex();
    }

    ~cubo(){
        for (int i = 0; i < 6; ++i)
        {
            glDeleteVertexArrays(1, &VAO[i]);
            glDeleteBuffers(1, &VBO[i]);
        }

    }
    
};

vector<vector<vector<cubo*>>> rubik;
float inicio=0.75;
float angle=0;
int rot=0;

void dar_tex_cara(){
    int x=0, y=0;
    for (int i = 2; i >= 0; --i)
    {
        for (int j = 2; j >= 0; --j)
        {
            rubik[0][i][j]->cambiar_coord(0, 0.33333*x, 0.33333*y);
            x++;
        }
        y++;
    }

    x=0, y=0;
    for (int i = 2; i >= 0; --i)
    {
        for (int j = 2; j >= 0; --j)
        {
            rubik[2][i][j]->cambiar_coord(1, 0.33333*x, 0.33333*y);
            x++;
        }
        y++;
    }

    x=0, y=0;
    for (int i = 2; i >= 0; --i)
    {
        for (int j = 2; j >= 0; --j)
        {
            rubik[j][i][0]->cambiar_coord(4, 0.33333*x, 0.33333*y);
            x++;
        }
        y++;
    }

    x=0, y=0;
    for (int i = 2; i >= 0; --i)
    {
        for (int j = 2; j >= 0; --j)
        {
            rubik[j][i][2]->cambiar_coord(5, 0.33333*x, 0.33333*y);
            x++;
        }
        y++;
    }

    x=0, y=0;
    for (int i = 2; i >= 0; --i)
    {
        for (int j = 2; j >= 0; --j)
        {
            rubik[i][0][j]->cambiar_coord(2, 0.33333*x, 0.33333*y);
            x++;
        }
        y++;
    }

    x=0, y=0;
    for (int i = 2; i >= 0; --i)
    {
        for (int j = 0; j < 3; ++j)
        {
            rubik[i][2][j]->cambiar_coord(3, 0.33333*x, 0.33333*y);
            x++;
        }
        y++;
    }

}

void crear_cubo(){

    vector<pair<float,float>> vecxy;
    vector<pair<float,float>> vecxy2;

    vecxy.push_back(pair<float,float>(0.75,0.75));
    vecxy.push_back(pair<float,float>(0.25,0.75));
    vecxy.push_back(pair<float,float>(-0.25,0.75));
    vecxy.push_back(pair<float,float>(0.75,0.25));
    vecxy.push_back(pair<float,float>(0.25,0.25));
    vecxy.push_back(pair<float,float>(-0.25,0.25));
    vecxy.push_back(pair<float,float>(0.75,-0.25));
    vecxy.push_back(pair<float,float>(0.25,-0.25));
    vecxy.push_back(pair<float,float>(-0.25,-0.25));

    
    float z=-0.75;

    vector<vector<cubo*>> plano1;
    for (int i = 0; i < 3; ++i)
    {
        vector<cubo*> r;
        cubo *a=new cubo(vecxy[(i*3)+0].first, vecxy[(i*3)+0].second, z, 0.75);
        r.push_back(a);
        cubo *b=new cubo(vecxy[(i*3)+1].first, vecxy[(i*3)+1].second, z, 0.75);
        r.push_back(b);
        cubo *c=new cubo(vecxy[(i*3)+2].first, vecxy[(i*3)+2].second, z, 0.75);
        r.push_back(c);

        plano1.push_back(r);
    }

    rubik.push_back(plano1);


    z=-0.25;

    vector<vector<cubo*>> plano2;
    for (int i = 0; i < 3; ++i)
    {
        vector<cubo*> r;
        cubo *a=new cubo(vecxy[(i*3)+0].first, vecxy[(i*3)+0].second, z, 0.75);
        r.push_back(a);
        cubo *b=new cubo(vecxy[(i*3)+1].first, vecxy[(i*3)+1].second, z, 0.75);
        r.push_back(b);
        cubo *c=new cubo(vecxy[(i*3)+2].first, vecxy[(i*3)+2].second, z, 0.75);
        r.push_back(c);

        plano2.push_back(r);
    }
    rubik.push_back(plano2);


    z=0.25;
    vector<vector<cubo*>> plano3;
    for (int i = 0; i < 3; ++i)
    {
        vector<cubo*> r;
        cubo *a=new cubo(vecxy[(i*3)+0].first, vecxy[(i*3)+0].second, z, 0.75);
        r.push_back(a);
        cubo *b=new cubo(vecxy[(i*3)+1].first, vecxy[(i*3)+1].second, z, 0.75);
        r.push_back(b);
        cubo *c=new cubo(vecxy[(i*3)+2].first, vecxy[(i*3)+2].second, z, 0.75);
        r.push_back(c);

        plano3.push_back(r);
    }
    rubik.push_back(plano3);

    dar_tex_cara();
}

void rotarVec_Y(int pmedio){
    for (int i = 0; i < 2; ++i)
    {
        cubo *temp=rubik[0][pmedio][1];
        rubik[0][pmedio][1]=rubik[0][pmedio][2];

        cubo *temp2=rubik[0][pmedio][0];
        rubik[0][pmedio][0]=temp;

        temp=rubik[1][pmedio][0];
        rubik[1][pmedio][0]=temp2;

        temp2=rubik[2][pmedio][0];
        rubik[2][pmedio][0]=temp;

        temp=rubik[2][pmedio][1];
        rubik[2][pmedio][1]=temp2;

        temp2=rubik[2][pmedio][2];
        rubik[2][pmedio][2]=temp;

        temp=rubik[1][pmedio][2];
        rubik[1][pmedio][2]=temp2;

        rubik[0][pmedio][2]=temp;
    }    
}

void rotarVecY(int pmedio){
    for (int i = 0; i < 2; ++i)
    {
        cubo *temp=rubik[0][pmedio][1];
        rubik[0][pmedio][1]=rubik[0][pmedio][0];

        cubo *temp2=rubik[0][pmedio][2];
        rubik[0][pmedio][2]=temp;

        temp=rubik[1][pmedio][2];
        rubik[1][pmedio][2]=temp2;

        temp2=rubik[2][pmedio][2];
        rubik[2][pmedio][2]=temp;

        temp=rubik[2][pmedio][1];
        rubik[2][pmedio][1]=temp2;

        temp2=rubik[2][pmedio][0];
        rubik[2][pmedio][0]=temp;

        temp=rubik[1][pmedio][0];
        rubik[1][pmedio][0]=temp2;

        rubik[0][pmedio][0]=temp;
    }
    
}

void rotarVec_Z(int pmedio){

    for (int i = 0; i < 2; ++i)
    {
        cubo *temp=rubik[pmedio][0][1];
        rubik[pmedio][0][1]=rubik[pmedio][0][2];

        cubo *temp2=rubik[pmedio][0][0];
        rubik[pmedio][0][0]=temp;

        temp=rubik[pmedio][1][0];
        rubik[pmedio][1][0]=temp2;

        temp2=rubik[pmedio][2][0];
        rubik[pmedio][2][0]=temp;

        temp=rubik[pmedio][2][1];
        rubik[pmedio][2][1]=temp2;

        temp2=rubik[pmedio][2][2];
        rubik[pmedio][2][2]=temp;

        temp=rubik[pmedio][1][2];
        rubik[pmedio][1][2]=temp2;

        rubik[pmedio][0][2]=temp;
    }    
}

void rotarVecZ(int pmedio){

    for (int i = 0; i < 2; ++i)
    {
        cubo *temp=rubik[pmedio][0][1];
        rubik[pmedio][0][1]=rubik[pmedio][0][0];

        cubo *temp2=rubik[pmedio][0][2];
        rubik[pmedio][0][2]=temp;

        temp=rubik[pmedio][1][2];
        rubik[pmedio][1][2]=temp2;

        temp2=rubik[pmedio][2][2];
        rubik[pmedio][2][2]=temp;

        temp=rubik[pmedio][2][1];
        rubik[pmedio][2][1]=temp2;

        temp2=rubik[pmedio][2][0];
        rubik[pmedio][2][0]=temp;

        temp=rubik[pmedio][1][0];
        rubik[pmedio][1][0]=temp2;

        rubik[pmedio][0][0]=temp;
    }    
}

void rotarVec_X(int pmedio){

    for (int i = 0; i < 2; ++i)
    {
        cubo *temp=rubik[1][0][pmedio];
        rubik[1][0][pmedio]=rubik[2][0][pmedio];

        cubo *temp2=rubik[0][0][pmedio];
        rubik[0][0][pmedio]=temp;

        temp=rubik[0][1][pmedio];
        rubik[0][1][pmedio]=temp2;

        temp2=rubik[0][2][pmedio];
        rubik[0][2][pmedio]=temp;

        temp=rubik[1][2][pmedio];
        rubik[1][2][pmedio]=temp2;

        temp2=rubik[2][2][pmedio];
        rubik[2][2][pmedio]=temp;

        temp=rubik[2][1][pmedio];
        rubik[2][1][pmedio]=temp2;

        rubik[2][0][pmedio]=temp;
    }    
}

void rotarVecX(int pmedio){

    for (int i = 0; i < 2; ++i)
    {
        cubo *temp=rubik[1][0][pmedio];
        rubik[1][0][pmedio]=rubik[0][0][pmedio];

        cubo *temp2=rubik[2][0][pmedio];
        rubik[2][0][pmedio]=temp;

        temp=rubik[2][1][pmedio];
        rubik[2][1][pmedio]=temp2;

        temp2=rubik[2][2][pmedio];
        rubik[2][2][pmedio]=temp;

        temp=rubik[1][2][pmedio];
        rubik[1][2][pmedio]=temp2;

        temp2=rubik[0][2][pmedio];
        rubik[0][2][pmedio]=temp;

        temp=rubik[0][1][pmedio];
        rubik[0][1][pmedio]=temp2;

        rubik[0][0][pmedio]=temp;
    }    
}

cubo *an;

void display(GLFWwindow *window){

    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
    float radius = 5.0f;
    float camX   = sin(glfwGetTime()) * radius;
    float camZ   = cos(glfwGetTime()) * radius;
    view = glm::lookAt(glm::vec3(v_eye[0], v_eye[1], v_eye[2]), glm::vec3(vc[0], vc[1], vc[2]), glm::vec3(up[0], up[1], up[2]));
    
    
    for (int k = 0; k < 3; ++k)
    {
        for (int i = 0; i < 3; ++i)
        {
            for (int j = 0; j < 3; ++j)
            {
                rubik[k][i][j]->dibujar();
            }
            
        }
    }
    //an->dibujar();

    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();
}

float _an=5;
float ang=0;

int flag=0;

void rotar90Z(GLFWwindow* window){
    while(ang<90){
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                rubik[dim-1][j][i]->rotarZ(_an);
            }
        }
        usleep(25000);
        display(window);
        ang=ang+5;
    }

    rotarVecZ(dim-1);
    ang=0;
    flag++;
}

void rotar_Z90(GLFWwindow* window){
    while(ang<90){
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                rubik[dim-1][j][i]->rotarZ(_an*(-1));
            }
        }
        usleep(25000);
        display(window);
        ang=ang+5;
    }
    rotarVec_Z(dim-1);
    ang=0;
    flag++;
}

void rotarY90(GLFWwindow* window){
    while(ang<90){
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                rubik[j][dim-1][i]->rotarY(_an);
            }
        }
        usleep(25000);
        display(window);
        ang=ang+5;
    }
    rotarVecY(dim-1);
    ang=0;
    flag++; 
}

void rotar_Y90(GLFWwindow* window){
    while(ang<90){
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                rubik[j][dim-1][i]->rotarY(_an*(-1));
            }
        }
        usleep(25000);
        display(window);
        ang=ang+5;
    }
    rotarVec_Y(dim-1);
    ang=0;
    flag++;
}

void rotarX90(GLFWwindow* window){
    while(ang<90){
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                rubik[j][i][dim-1]->rotarX(_an);
            }
        }
        usleep(25000);
        display(window);
        ang=ang+5;
    }
    rotarVecX(dim-1);
    ang=0;
    flag++;
}

void rotar_X90(GLFWwindow* window){
    while(ang<90){
        for (int j = 0; j < 3; ++j)
        {
            for (int i = 0; i < 3; ++i)
            {
                rubik[j][i][dim-1]->rotarX(_an*(-1));
            }
        }
        usleep(25000);
        display(window);
        ang=ang+5;
    }
    rotarVec_X(dim-1);
    ang=0;
    flag++;
}

void rotarR(GLFWwindow* window){
    dim=1;
    rotar_X90(window);
}

void rotarL(GLFWwindow* window){
    dim=3;
    rotarX90(window);
}

void rotarF(GLFWwindow* window){
    dim=3;
    rotar_Z90(window);
}

void rotarB(GLFWwindow* window){
    dim=1;
    rotar90Z(window);
}

void rotarU(GLFWwindow* window){
    dim=1;
    rotar_Y90(window);
}

void rotarD(GLFWwindow* window){
    dim=3;
    rotarY90(window);
}


void rotarRP(GLFWwindow* window){
    dim=1;
    rotarX90(window);
}

void rotarLP(GLFWwindow* window){
    dim=3;
    rotar_X90(window);
}

void rotarFP(GLFWwindow* window){
    dim=3;
    rotar90Z(window);
}

void rotarBP(GLFWwindow* window){
    dim=1;
    rotar_Z90(window);
}

void rotarUP(GLFWwindow* window){
    dim=1;
    rotarY90(window);
}

void rotarDP(GLFWwindow* window){
    dim=3;
    rotar_Y90(window);
}

void rotarR2(GLFWwindow* window){
    dim=1;
    rotar_X90(window);
    rotar_X90(window);
}

void rotarL2(GLFWwindow* window){
    dim=3;
    rotarX90(window);
    rotarX90(window);
}

void rotarF2(GLFWwindow* window){
    dim=3;
    rotar_Z90(window);
    rotar_Z90(window);
}

void rotarB2(GLFWwindow* window){
    dim=1;
    rotar90Z(window);
    rotar90Z(window);
}

void rotarU2(GLFWwindow* window){
    dim=1;
    rotar_Y90(window);
    rotar_Y90(window);
}

void rotarD2(GLFWwindow* window){
    dim=3;
    rotarY90(window);
    rotarY90(window);
}

void desordenar(vector<string> des_orden, GLFWwindow* window){
    for (int i = 0; i < des_orden.size(); ++i)
    {
        if (des_orden[i]=="R")  rotarR(window);
        else if (des_orden[i]=="L")  rotarL(window);
        else if (des_orden[i]=="F")  rotarF(window);
        else if (des_orden[i]=="B")  rotarB(window);
        else if (des_orden[i]=="U")  rotarU(window);
        else if (des_orden[i]=="D")  rotarD(window);
        else if (des_orden[i]=="R'")  rotarRP(window);
        else if (des_orden[i]=="L'")  rotarLP(window);
        else if (des_orden[i]=="F'")  rotarFP(window);
        else if (des_orden[i]=="B'")  rotarBP(window);
        else if (des_orden[i]=="U'")  rotarUP(window);
        else if (des_orden[i]=="D'")  rotarDP(window);
        else if (des_orden[i]=="R2")  rotarR2(window);
        else if (des_orden[i]=="L2")  rotarL2(window);
        else if (des_orden[i]=="F2")  rotarF2(window);
        else if (des_orden[i]=="B2")  rotarB2(window);
        else if (des_orden[i]=="U2")  rotarU2(window);
        else if (des_orden[i]=="D2")  rotarD2(window);
    }
}

void ordenar(vector<string> des_orden, GLFWwindow* window){
    for (int i = 0; i < des_orden.size(); ++i)
    {
        if (des_orden[i]=="R")  rotarR(window);
        else if (des_orden[i]=="L")  rotarL(window);
        else if (des_orden[i]=="F")  rotarF(window);
        else if (des_orden[i]=="B")  rotarB(window);
        else if (des_orden[i]=="U")  rotarU(window);
        else if (des_orden[i]=="D")  rotarD(window);
        else if (des_orden[i]=="R'")  rotarRP(window);
        else if (des_orden[i]=="L'")  rotarLP(window);
        else if (des_orden[i]=="F'")  rotarFP(window);
        else if (des_orden[i]=="B'")  rotarBP(window);
        else if (des_orden[i]=="U'")  rotarUP(window);
        else if (des_orden[i]=="D'")  rotarDP(window);
        else if (des_orden[i]=="R2")  rotarR2(window);
        else if (des_orden[i]=="L2")  rotarL2(window);
        else if (des_orden[i]=="F2")  rotarF2(window);
        else if (des_orden[i]=="B2")  rotarB2(window);
        else if (des_orden[i]=="U2")  rotarU2(window);
        else if (des_orden[i]=="D2")  rotarD2(window);
    }
}

int main()
{
    srand(time(NULL));
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "LearnOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, controls);
    glfwSetScrollCallback(window, scroll_callback);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    l_giros_alg.push_back("R");     l_giros_alg.push_back("L");     l_giros_alg.push_back("F");     l_giros_alg.push_back("B");     l_giros_alg.push_back("U");     l_giros_alg.push_back("D");
    l_giros_alg.push_back("R'");     l_giros_alg.push_back("L'");     l_giros_alg.push_back("F'");     l_giros_alg.push_back("B'");     l_giros_alg.push_back("U'");     l_giros_alg.push_back("D'");
    l_giros_alg.push_back("R2");     l_giros_alg.push_back("L2");     l_giros_alg.push_back("F2");     l_giros_alg.push_back("B2");     l_giros_alg.push_back("U2");     l_giros_alg.push_back("D2");

    nombres.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/U.jpg"));
    nombres.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/S.jpg"));
    nombres.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/C2.jpg"));
    nombres.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/G.jpg"));
    nombres.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/P.jpg"));
    nombres.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/C.jpg"));
    nombres.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/negro.jpg"));

    n_color.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/rojo.png"));
    n_color.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/verde.png"));
    n_color.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/azul.png"));
    n_color.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/blanco.png"));
    n_color.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/naranja.png"));
    n_color.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/amarillo.png"));
    n_color.push_back(string("/home/angelcs/Documentos/SEMESTRE IX/COMP GRAFICA/GLFW_GLAD_GLUT_GLEW_cmake_project/src/CG-PRACTiCAP/gris.png"));


    v_eye[0]=0;
    v_eye[1]=1;
    v_eye[2]=3;

    up[0]=0;
    up[1]=1;
    up[2]=0;

    vc[0]=0;
    vc[1]=0;
    vc[2]=0;

    Adar_textura();
    crear_cubo();
    //an=new cubo(0.75, 0.75, -0.75, 0.75);

    // render loop
    // -----------

    while (!glfwWindowShouldClose(window))
    {
        display(window);
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}




void controls(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(action == GLFW_PRESS)
        if(key == GLFW_KEY_ESCAPE)
            glfwSetWindowShouldClose(window, GL_TRUE);
    else if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS )
    {
        float radius = 5.0f;
        v_eye[0] = sin(glfwGetTime())*radius;
        v_eye[2] = cos(glfwGetTime())*radius;

    }
    else if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS )
    {
        float radius = 5.0f;
        v_eye[0] = sin(glfwGetTime())*radius;
        v_eye[1] = cos(glfwGetTime())*radius;
    }

    else if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS )
    {
        flag=0;
        rotar90Z(window);
    }
    else if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS )
    {
        flag=0;
        rotar_Z90(window);
    }
    else if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS )
    {

        flag=0;
        rotarY90(window);               
    }
    else if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS )
    {

        flag=0;
        rotar_Y90(window);                
    }
    else if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS )
    {

        flag=0;
        rotarX90(window);
    }
    else if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS )
    {

        flag=0;
        rotar_X90(window);
    }

    else if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS )
    {
        vector<pair<float,float>> vecxy;
        vector<pair<float,float>> vecxy2;
        vector<vector<vector<cubo*>>> rubik2;

        vecxy2.push_back(pair<float,float>(1,1));
        vecxy2.push_back(pair<float,float>(0.25,1));
        vecxy2.push_back(pair<float,float>(-0.5,1));
        vecxy2.push_back(pair<float,float>(1,0.25));
        vecxy2.push_back(pair<float,float>(0.25,0.25));
        vecxy2.push_back(pair<float,float>(-0.5,0.25));
        vecxy2.push_back(pair<float,float>(1,-0.5));
        vecxy2.push_back(pair<float,float>(0.25,-0.5));
        vecxy2.push_back(pair<float,float>(-0.5,-0.5));

        float z=-1.0;

        vector<vector<cubo*>> plano1;
        for (int i = 0; i < 3; ++i)
        {
            vector<cubo*> r;
            cubo *a=new cubo(vecxy2[(i*3)+0].first, vecxy2[(i*3)+0].second, z, 1.0);
            r.push_back(a);
            cubo *b=new cubo(vecxy2[(i*3)+1].first, vecxy2[(i*3)+1].second, z, 1.0);
            r.push_back(b);
            cubo *c=new cubo(vecxy2[(i*3)+2].first, vecxy2[(i*3)+2].second, z, 1.0);
            r.push_back(c);

            plano1.push_back(r);
        }

        rubik2.push_back(plano1);


        z=-0.25;

        vector<vector<cubo*>> plano2;
        for (int i = 0; i < 3; ++i)
        {
            vector<cubo*> r;
            cubo *a=new cubo(vecxy2[(i*3)+0].first, vecxy2[(i*3)+0].second, z, 1.0);
            r.push_back(a);
            cubo *b=new cubo(vecxy2[(i*3)+1].first, vecxy2[(i*3)+1].second, z, 1.0);
            r.push_back(b);
            cubo *c=new cubo(vecxy2[(i*3)+2].first, vecxy2[(i*3)+2].second, z, 1.0);
            r.push_back(c);

            plano2.push_back(r);
        }
        rubik2.push_back(plano2);


        z=0.5;
        vector<vector<cubo*>> plano3;
        for (int i = 0; i < 3; ++i)
        {
            vector<cubo*> r;
            cubo *a=new cubo(vecxy2[(i*3)+0].first, vecxy2[(i*3)+0].second, z, 1.0);
            r.push_back(a);
            cubo *b=new cubo(vecxy2[(i*3)+1].first, vecxy2[(i*3)+1].second, z, 1.0);
            r.push_back(b);
            cubo *c=new cubo(vecxy2[(i*3)+2].first, vecxy2[(i*3)+2].second, z, 1.0);
            r.push_back(c);

            plano3.push_back(r);
        }
        rubik2.push_back(plano3);

        for (int i = 0; i < rubik.size(); ++i)
        {
            rubik[i]=rubik2[i];
        }
        dar_tex_cara();
        inicio=1.0;

    }

    else if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS )
      {
        vc[1]-=0.01;
      }
      
      else if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS )
      {
        vc[1]+=0.01;
      }

    else if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS )
    {
        dim=1;
    }
    else if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS )
    {
        dim=2;
    }
    else if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS )
    {
        dim=3;
    }
    else if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS ){
        //string s="R L U2 F U' D F2 R2 B2 L U2 F' B' U R2 D F2 U R2 U";
        vector<string> des_or;
        for (int i = 0; i < 15; ++i)
        {
            int a=rand()%18;
            Cgiros=Cgiros+" "+l_giros_alg[a];
            des_or.push_back(l_giros_alg[a]);
        }
        desordenar(des_or, window);
    }
    else if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS ){
        Cgiros='"'+Cgiros;
        string cadena="/home/angelcs/Escritorio/CUBICJ "+Cgiros+'"'+">SomeFileC.txt";
        cout<<cadena<<endl;
        system(cadena.c_str());
        string s=leer_archivo();
        cout<<s<<endl;
        vector<string> des_or=split(s,' ');
        ordenar(des_or, window);
    }
    else if (glfwGetKey(window, GLFW_KEY_P) == GLFW_PRESS )
    {
        rotarF2(window);
        rotarB2(window);
        rotarR2(window);
        rotarL2(window);
        rotarU2(window);
        rotarD2(window);
    }
}