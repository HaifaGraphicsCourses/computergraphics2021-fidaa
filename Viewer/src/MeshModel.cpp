#include "MeshModel.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <random>


#include "Utils.h"
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
/*
MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, glm::mat4x4 T, float MAX, float maxX, float maxY, float maxZ, float minX, float minY, float minZ) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(model_name),
	STmat(T),
	MAX(MAX),
	maxX(maxX),
	maxY(maxY),
	maxZ(maxZ),
	minX(minX),
	minY(minY),
	minZ(minZ)
{

}

*/


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& modelName, glm::mat4x4 T, float MAX, float maxX, float maxY, float maxZ, float minX, float minY, float minZ) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	model_name_(modelName),
	STmat(T),
	MAX(MAX),
	maxX(maxX),
	maxY(maxY),
	maxZ(maxZ),
	minX(minX),
	minY(minY),
	minZ(minZ),
	modelName(modelName)
{
	
	std::random_device rd;
	std::mt19937 mt(rd());
	std::uniform_real_distribution<double> dist(0, 1);
	color = glm::vec3(dist(mt), dist(mt), dist(mt));

	modelVertices.reserve(3 * faces.size());
	for (int i = 0; i < faces.size(); i++)
	{
		Face currentFace = faces.at(i);
		for (int j = 0; j < 3; j++)
		{
			int vertexIndex = currentFace.GetVertexIndex(j) - 1;
			int normalIndex = currentFace.GetNormalIndex(j) - 1;
			
			Vertex vertex;
			vertex.position = vertices[vertexIndex];
			if(normals.size())
				vertex.normal = normals[normalIndex];

			if (textureCoords.size() > 0)
			{
				int textureCoordsIndex = currentFace.GetTextureIndex(j) - 1;
				vertex.textureCoords = textureCoords[textureCoordsIndex];
			}

			modelVertices.push_back(vertex);
		}
	}

	glGenVertexArrays(1, &vao);
	glGenBuffers(1, &vbo);

	glBindVertexArray(vao);
	glBindBuffer(GL_ARRAY_BUFFER, vbo);
	glBufferData(GL_ARRAY_BUFFER, modelVertices.size() * sizeof(Vertex), &modelVertices[0], GL_STATIC_DRAW);

	// Vertex Positions
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Normals attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	// Vertex Texture Coords
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);

	// unbind to make sure other code does not change it somewhere else
	glBindVertexArray(0);
}

MeshModel::~MeshModel()
{
	glDeleteVertexArrays(1, &vao);
	glDeleteBuffers(1, &vbo);
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}
const glm::vec3 MeshModel::GetVertex(int index) const
{
	return vertices_[index - 1];
}

const glm::mat4x4 MeshModel::GetSTmat() const
{
	return STmat;
}
void MeshModel::PrintFaces() const
{
	int index;
	for (int i = 0; i <faces_.size(); i++)
	{
		std::cout << "the vartices of face" <<i<< ":";
		index = faces_[i].GetVertexIndex(0);
		std::cout <<'(' << vertices_[index-1].x << ','<< vertices_[index-1].y << ','<< vertices_[index-1].z << ')';
		index = faces_[i].GetVertexIndex(1);
		std::cout << '(' << vertices_[index-1].x << ',' << vertices_[index-1].y << ',' << vertices_[index-1].z << ')';
		index = faces_[i].GetVertexIndex(2);
		std::cout << '(' << vertices_[index-1].x << ',' << vertices_[index-1].y << ',' << vertices_[index-1].z << ')';
		std::cout << "\n";

	}
	
	return;
}
void MeshModel::PrintVertices()const
{
	
	std::cout << "all the  vertices:";
	for (int i = 1; i < vertices_.size(); i++)
	{
		std::cout << '(' << vertices_[i - 1].x << ',' << vertices_[i - 1].y << ',' << vertices_[i - 1].z << ')';
		std::cout << "\n";

	}
}
void MeshModel::Set_Tw_mat(glm::mat4x4& transformation)
{
	Tw = transformation;
}

void MeshModel::Set_Sw_mat(glm::mat4x4& transformation)
{
	this->Sw = transformation;

}

void MeshModel::Set_Tm_mat(glm::mat4x4& transformation)
{
	Tm = transformation;
	//std::cout <<"im in the set" << x << y << z << "\n";

}

void MeshModel::Set_Sm_mat(glm::mat4x4& transformation)
{
	Sm = transformation;

}

void MeshModel::Set_Rw_mat(glm::mat4x4& transformation,int flag)
{
	if (flag == 0)
	{
		Rw_x = transformation;
	}
	if (flag == 1)
	{
		Rw_y = transformation;
	}
	if (flag == 2)
	{
		Rw_z = transformation;
	}
	Rw = Rw_z * Rw_y * Rw_x;

}

void MeshModel::Set_Rm_mat(glm::mat4x4& transformation, int flag)
{
	if (flag == 0)
	{
		Rm_x = transformation;
	}
	if (flag == 1)
	{
		Rm_y = transformation;
	}
	if (flag == 2)
	{
		Rm_z = transformation;
	}
	Rm = Rm_z*Rm_y*Rm_x;

}

void MeshModel::Set_transmatrix()
{
	this->transmatrix = Sw * Rw * Tw * Sm * Rm * Tm;
}
const glm::mat4x4 MeshModel::Get_transmatrix() const
{

	return this->transmatrix;
	
}
void MeshModel::printmat() const
{
	
	for (int j = 0; j < 4; j++)
	{
		for (int k = 0; k < 4; k++)
		{
			std::cout << transmatrix[j][k] << ",";

		}
		std::cout << "\n";
	}

}

void MeshModel::Set_Reset(int r)
{
	reset = r;
	if (reset)
	{
		transmatrix = glm::mat4(1.0f);
		Sw = glm::mat4(1.0f);
		Sm = glm::mat4(1.0f);
		Rw = glm::mat4(1.0f);
		Rm = glm::mat4(1.0f);
		Tw = glm::mat4(1.0f);
		Tm = glm::mat4(1.0f);
	}
	
}

float MeshModel::Get_maxX() const
{
	return maxX;
}


float MeshModel::Get_maxY() const
{
	return maxY;
}


float MeshModel::Get_maxZ() const
{
	return maxZ;
}

float MeshModel::Get_minX() const
{
	return minX;
}


float MeshModel::Get_minY() const
{
	return minY;
}

float MeshModel::Get_minZ() const
{
	return minZ;
}

void MeshModel::Set_showbox(int b)
{
	showbox = b;
}

int MeshModel::Get_showbox()const
{
	return showbox;
}

void MeshModel::Set_facenormals(int fn)
{
	facenormals = fn;
}

int MeshModel::Get_facenormals()const
{
	return facenormals;
}

void MeshModel::Set_vernormals(int vn)
{
	vernormals = vn;
}

int MeshModel::Get_vernormals()const
{
	return vernormals;
}

glm::mat4x4 MeshModel::Get_Rw_mat() const
{
	return Rw;
}

glm::mat4x4 MeshModel::Get_Rm_mat() const
{
	return Rm;
}
const glm::vec3 MeshModel::Get_normalvertex(int index) const
{
	return normals_[index - 1];
}

float MeshModel::Get_orthoGraphicVal()
{
	float ortho_val=0;
	if (model_name_ == "banana.obj")
	{
		ortho_val = 0.5f;
	}
	if (model_name_ == "Sphere.obj")
	{
		ortho_val = 0.5f;
	}
	if (model_name_ == "beethoven.obj")
	{
		ortho_val = 25.0f;
	}
	if (model_name_ == "bishop.obj")
	{
		ortho_val = 0.5f;
	}
	if (model_name_ == "bunny.obj")
	{
		ortho_val = 5.0f;
	}
	if (model_name_ == "camera.obj")
	{
		ortho_val = 10.0f;
	}
	if (model_name_ == "chain.obj")
	{
		ortho_val = 15.0f;
	}
	if (model_name_ == "cow.obj")
	{
		ortho_val = 20.0f;
	}
	if (model_name_ == "demo.obj")
	{
		ortho_val = 20.0f;
	}
	if (model_name_ == "dolphin.obj")
	{
		ortho_val = 800.0f;
	}
	if (model_name_ == "pawn.obj")
	{
		ortho_val = 0.3f;
	}
	if (model_name_ == "teapot.obj")
	{
		ortho_val = 15.0f;
	}
	return ortho_val;
}



float MeshModel::Get_Zeye()
{
	float eye_z = 0;
	if (model_name_ == "Sphere.obj")
	{
		eye_z = 0.5f;
	}
	if (model_name_ == "banana.obj")
	{
		eye_z = 0.5f;
	}
	if (model_name_ == "beethoven.obj")
	{
		eye_z = 30.0f;
	}
	if (model_name_ == "bishop.obj")
	{
		eye_z = 0.5f;
	}
	if (model_name_ == "bunny.obj")
	{
		eye_z = 3.0f;
	}
	if (model_name_ == "camera.obj")
	{
		eye_z = 10.0f;
	}
	if (model_name_ == "chain.obj")
	{
		eye_z = 10.0f;
	}
	if (model_name_ == "cow.obj")
	{
		eye_z = 10.0f;
	}
	if (model_name_ == "demo.obj")
	{
		eye_z = 10.0f;
	}
	if (model_name_ == "dolphin.obj")
	{
		eye_z = 900.0f;
	}
	if (model_name_ == "pawn.obj")
	{
		eye_z = 0.5f;
	}
	if (model_name_ == "teapot.obj")
	{
		eye_z = 10.0f;
	}
	return eye_z;
}


void MeshModel::Set_colorsvar(int c)
{
	this->colors = c;
}
int MeshModel::Get_colorsvar() const
{
	return colors;
}
void MeshModel::Set_modelAmbient_Color(glm::vec3 color)
{
	Ambient_Color = color;
}
void MeshModel::Set_modelDiffuse_Color(glm::vec3 color)
{
	Diffuse_Color = color;
}
void MeshModel::Set_modelSpecular_Color(glm::vec3 color)
{
	Specular_Color = color;
}
 glm::vec3 MeshModel::Get_modelAmbient_Color() const
{
	return Ambient_Color;
}
 glm::vec3 MeshModel::Get_modelDiffuse_Color() const
{
	return Diffuse_Color;
}
 glm::vec3 MeshModel::Get_modelSpecular_Color() const
{
	return Specular_Color;
}


 GLuint MeshModel::GetVAO() const
 {
	 return vao;
 }

 const std::vector<Vertex>& MeshModel::GetModelVertices()
 {
	 return modelVertices;
 }
