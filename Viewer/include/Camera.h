#pragma once
#include <glm/glm.hpp>

class Camera
{
public:
	Camera();
	virtual ~Camera();

	void SetCameraLookAt();

	const glm::mat4x4& GetProjectionTransformation() const;
	const glm::mat4x4& GetViewTransformation() const;

	void Set_Tw_mat(glm::mat4x4& transformation);
	void Set_Rw_mat(glm::mat4x4& transformation, int flag);
	void Set_Tm_mat(glm::mat4x4& transformation);
	void Set_Rm_mat(glm::mat4x4& transformation, int flag);
	void Set_transmatrix();
	void Set_Eye(float x,float y,float z);
	void Set_At(float x, float y, float z);
	void Set_Up(float x, float y, float z);
	
	const glm::mat4x4 Get_Lookat()const;
	const glm::mat4x4 Get_Invtransmatrix()const ;
	void Set_OrthoGraphic(int OG, float ortho_val);
	void Set_Perspective(int OG, float persp_val);
	int  Get_OrthoGraphic()const;
	





private:
	float w = 1920;
	float h = 1080;
	float aspectrat = w / h;
	float fovy = 45;
	glm::mat4x4 view_transformation_;
	glm::mat4x4 projection_transformation_;
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
	glm::mat4x4 lookat = glm::mat4(1.0f);
	int OrthoG = 0;
	float ortho_val = 0.5;
	float left = -w / 2, right = w / 2;
	float  bottom = -h / 2, top = h / 2;
	float p_near = -0.1;
	float p_far = 1000;
	glm::mat4x4 orthomatrix = glm::mat4(1.0f); /**/

	//lookat

	glm::vec3 eye_vec = { 0.0f, 0.0f, 0.5f };
	glm::vec3 at_vec = { 0.0f, 0.0f, 0.5f };
	glm::vec3 up_vec = { 0.0f, 1.0f, 0.5f };

};
