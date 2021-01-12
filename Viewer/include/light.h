#pragma once
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

class light
{
public:
	light();
	//void Set_Color(glm::vec3 color);
	void Set_Ambient_Color(glm::vec3 color);
	void Set_Diffuse_Color(glm::vec3 color);
	void Set_Specular_Color(glm::vec3 color);

	void Set_Direction(glm::vec3 direction);
	void Set_Position(glm::vec4 position);
	void Set_Type(int type);
	
	

	 glm::vec3 Get_Ambient_Color() const;
	 glm::vec3 Get_Diffuse_Color() const;
	 glm::vec3 Get_Specular_Color() const;

	const glm::vec3 Get_Direction() const;
	const glm::vec4 Get_Position() const;
	const int Get_Type() const;

	void Set_Tw_mat(glm::mat4x4& transformation);
	void Set_Rw_mat(glm::mat4x4& transformation, int flag);
	void Set_Tm_mat(glm::mat4x4& transformation);
	void Set_Rm_mat(glm::mat4x4& transformation, int flag);
	void Set_transmatrix();
	const glm::mat4x4 Get_transmatrix() const;
	const float Get_alpha() const;
	void Set_alpha(float a);
private:
	glm::vec3 Ambient_Color;
	glm::vec3 Diffuse_Color;
	glm::vec3 Specular_Color;

	glm::vec3 Direction;
	glm::vec4 Position = glm::vec4(0,0, 0 ,1);
	
	int Type; // 1 (Parallel) , 2 (Point) , 3 (Ambient)

	float alpha;
	glm::mat4x4 Tm = glm::mat4(1.0f);
	glm::mat4x4 Tw = glm::mat4(1.0f);
	glm::mat4x4 Rw = glm::mat4(1.0f);
	glm::mat4x4 Rw_x = glm::mat4(1.0f);
	glm::mat4x4 Rw_y = glm::mat4(1.0f);
	glm::mat4x4 Rw_z = glm::mat4(1.0f);
	glm::mat4x4 Rm = glm::mat4(1.0f);
	glm::mat4x4 Rm_x = glm::mat4(1.0f);
	glm::mat4x4 Rm_y = glm::mat4(1.0f);
	glm::mat4x4 Rm_z = glm::mat4(1.0f);
	glm::mat4x4 transmatrix = glm::mat4(1.0f);
};

