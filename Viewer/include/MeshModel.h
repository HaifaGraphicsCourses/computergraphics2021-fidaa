#pragma once
#include <glm/glm.hpp>
#include <string>
#include "Face.h"

class MeshModel
{
public:
	MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, glm::mat4x4 T,float MAX, float maxX, float maxY, float maxZ, float minX, float minY, float minZ);
	virtual ~MeshModel();
	const Face& GetFace(int index) const;
	int GetFacesCount() const;
	const std::string& GetModelName() const;
	const glm::vec3 GetVertex(int index) const;
	const glm::mat4x4 GetSTmat() const;
	void PrintFaces() const;
	void PrintVertices()const;
	void Set_Tw_mat(glm::mat4x4& transformation);
	void Set_Rw_mat(glm::mat4x4& transformation, int flag);
	glm::mat4x4 Get_Rw_mat() const;
	void Set_Sw_mat(glm::mat4x4& transformation);
	void Set_Tm_mat(glm::mat4x4& transformation);
	void Set_Rm_mat(glm::mat4x4& transformation, int flag);
	glm::mat4x4 Get_Rm_mat() const;
	void Set_Sm_mat(glm::mat4x4& transformation);
	void Set_transmatrix();
	const glm::mat4x4 Get_transmatrix() const;
	
	void printmat() const;
	void Set_Reset(int r);

	void Set_showbox(int b);
	int Get_showbox()const ;
	float Get_maxX() const;
	float Get_maxY() const;
	float Get_maxZ() const;
	float Get_minX() const;
	float Get_minY() const;
	float Get_minZ() const;
	
	void Set_facenormals(int fn);
	void Set_vernormals(int vn);
	int Get_facenormals()const;
	int Get_vernormals()const;
	const glm::vec3 Get_normalvertex(int index) const;


private:
	std::vector<Face> faces_;
	std::vector<glm::vec3> vertices_;
	std::vector<glm::vec3> normals_;
	std::string model_name_;
	glm::mat4x4 STmat;
	glm::mat4x4 Tm = glm::mat4(1.0f);
	glm::mat4x4 Tw = glm::mat4(1.0f);
	glm::mat4x4 Rw = glm::mat4(1.0f);
	glm::mat4x4 Rw_x = glm::mat4(1.0f);
	glm::mat4x4 Rw_y = glm::mat4(1.0f);
	glm::mat4x4 Rw_z = glm::mat4(1.0f);

	glm::mat4x4 Sw = glm::mat4(1.0f);
	glm::mat4x4 Rm = glm::mat4(1.0f);
	glm::mat4x4 Rm_x = glm::mat4(1.0f);
	glm::mat4x4 Rm_y = glm::mat4(1.0f);
	glm::mat4x4 Rm_z = glm::mat4(1.0f);
	glm::mat4x4 Sm = glm::mat4(1.0f);
	glm::mat4x4 transmatrix= glm::mat4(1.0f);

	float maxX = 0, minX = 0, maxY = 0, minY = 0, maxZ = 0, minZ = 0, MAX = 0;
	int reset = 0;
	int showbox = 0;
	int facenormals = 0;
	int vernormals = 0;
};
