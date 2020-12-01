#include "Camera.h"

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
{
	
}

Camera::~Camera()
{
	
}

const glm::mat4x4& Camera::GetProjectionTransformation() const
{

	return projection_transformation_;
}

const glm::mat4x4& Camera::GetViewTransformation() const
{
	return view_transformation_;
}


void Camera::Set_Tw_mat(glm::mat4x4& transformation)
{
	Tw = transformation;
}

void Camera::Set_Tm_mat(glm::mat4x4& transformation)
{
	Tm = transformation;
}


void Camera::Set_Rw_mat(glm::mat4x4& transformation, int flag)
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

void Camera::Set_Rm_mat(glm::mat4x4& transformation, int flag)
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
	Rm = Rm_z * Rm_y * Rm_x;

}

void Camera::Set_transmatrix()
{
	transmatrix =  Rw * Tw  * Rm * Tm;
	transmatrix = glm::inverse(transmatrix);
}
const glm::mat4x4 Camera::Get_Invtransmatrix()const
{
	return transmatrix;
}

void Camera::Set_OrthoGraphic(int OG,float OV)
{
	OrthoG = OG;
	w = OV;
	h = w / aspectrat;
	left = -w / 2;
	right = w / 2;
	bottom = -h / 2;
	top = h / 2;
	projection_transformation_=glm::ortho(left, right, bottom, top,p_near,p_far);
}

int Camera::Get_OrthoGraphic()const
{
	return OrthoG;
}


void Camera::Set_Eye(float x, float y, float z)
{
	eye_vec = { x,y,z };
}

void Camera::Set_At(float x, float y, float z)
{
	at_vec = { x,y,z };
}

void Camera::Set_Up(float x, float y, float z)
{
	up_vec = { x,y,z };
}
void Camera::SetCameraLookAt()
{
	lookat = glm::lookAt(eye_vec, at_vec, up_vec);
}



const glm::mat4x4 Camera::Get_Lookat()const
{
	return lookat;
}

void Camera::Set_Perspective(int OG, float persp_val)
{
	OrthoG = OG;
	fovy = persp_val;
	projection_transformation_ = glm::perspective(fovy, aspectrat, p_near, p_far);
	
}