#include <iostream>
#include <math.h>
using namespace std;

class matriz
{
public:
	matriz(){};
	void mul_mat_mat(float A[4][4], float B[4][4], float C[4][4], int tam ){

		for(int i=0; i<tam; ++i)
	        for(int j=0; j<tam; ++j)
	        {
	            C[i][j]=0;
	        }

		for(int i=0; i<tam; ++i){
	        for(int j=0; j<tam; ++j){
	            for(int z=0; z<tam; ++z){
	                C[i][j] += A[i][z] * B[z][j];
	            }
	        }
	    }
	}

	void mat_vec(float mat[4][4], float vec[4]){
		float comprueba[4];
	    for (int i = 0; i < 4; i++) {
			comprueba[i] = 0;
			for (int j = 0; j < 4; j++) {
			    comprueba[i] += mat[i][j] * vec[j];
			}
	    }
	    vec[0]=comprueba[0];	vec[1]=comprueba[1];	vec[2]=comprueba[2]; vec[3]=comprueba[3];
	}

	void mul_rotZ(float respuesta[4], int angle){
		float PI=3.1415926;
		float angulo = (PI*angle)/180.0;
		float mat[4][4]={
			{cos(angulo), -sin(angulo),0,0},
			{sin(angulo), cos(angulo),0, 0},
			{0, 0, 1, 0},
			{0, 0, 0, 1}
		};
		mat_vec(mat, respuesta);
	}

	void mul_rotX(float respuesta[4], int angle){
		float PI=3.1415926;
		float angulo = (PI*angle)/180.0;
		float mat[4][4]={
			{1,0,0,0},
			{0, cos(angulo), -sin(angulo), 0},
			{0, sin(angulo), cos(angulo), 0},
			{0, 0, 0, 1}
		};
		mat_vec(mat, respuesta);
	}

	void mul_rotY(float respuesta[4], int angle){
		float PI=3.1415926;
		float angulo = (PI*angle)/180.0;
		float mat[4][4]={
			{cos(angulo), 0, sin(angulo),0},
			{0,1,0, 0},
			{-sin(angulo), 0, cos(angulo), 0},
			{0, 0, 0, 1}
		};
		mat_vec(mat, respuesta);
	}

	void mul_traslacion(float entrada[4][4], float respuesta[4][4], float Tx, float Ty, float Tz){
		float mat[4][4]={
			{1, 0, 0, Tx},
			{0, 1, 0, Ty},
			{0, 0, 1, Tz},
			{0, 0, 0, 1}
		};
		mul_mat_mat(mat, entrada, respuesta, 4);
	}

	void mul_scala(float entrada[4][4], float respuesta[4][4], float Sx, float Sy, float Sz){
		float mat[4][4]={
			{Sx, 0, 0, 0},
			{0, Sy, 0, 0},
			{0, 0, Sz, 0},
			{0, 0, 0, 1}
		};
		mul_mat_mat(mat, entrada, respuesta, 4);
	}

	void print_mat(float A[4][4]){
		for (int i = 0; i < 4; ++i)
	    {
	    	for (int j = 0; j < 4; ++j)
	    	{
	    		cout<<A[i][j]<<" ";
	    	}
	    	cout<<endl;
	    }

	}
	void ini_cero(float A[4][4]){
		for (int i = 0; i < 4; ++i)
	    {
	    	for (int j = 0; j < 4; ++j)
	    	{
	    		A[i][j]=0;
	    	}
	    }
	}
	void vec_to_point(float A[9], float in[4][4]){
		float vec[4];
		float vec1[4];
		float vec2[4];

		vec[0]=A[0] ; vec[1]=A[1] ; vec[2]=A[2] ;
		vec1[0]=A[3] ; vec1[1]=A[4] ; vec1[2]=A[5] ;
		vec2[0]=A[6] ; vec2[1]=A[7] ; vec2[2]=A[8] ;

		vec[3]=1;		vec1[3]=1;			vec2[3]=1;

		mat_vec(in, vec);
		A[0]=vec[0];		A[1]=vec[1];		A[2]=vec[2];

		mat_vec(in, vec1);
		A[3]=vec1[0];		A[4]=vec1[1];		A[5]=vec1[2];

		mat_vec(in, vec2);
		A[6]=vec2[0];		A[7]=vec2[1];		A[8]=vec2[2];

	}

	void vec_to_point2(float A[9], float in[4][4]){
		float vec[4];

		vec[0]=A[0] ; vec[1]=A[1] ; vec[2]=A[2] ;

		vec[3]=1;

		mat_vec(in, vec);
		A[0]=vec[0];		A[1]=vec[1];		A[2]=vec[2];

	}

	~matriz(){};
	
};