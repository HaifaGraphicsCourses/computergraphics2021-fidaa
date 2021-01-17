#include "light.h"

light::light()
{
}


void  light::Set_Ambient_Color(glm::vec3 color)
{
	this->Ambient_Color = color;
}


void  light::Set_Diffuse_Color(glm::vec3 color)
{
	this->Diffuse_Color = color;
}


void  light::Set_Specular_Color(glm::vec3 color)
{
	this->Specular_Color = color;
}


void light::Set_Direction(glm::vec3 direction)
{
	this->Direction = direction;
}

void light::Set_Position(glm::vec4 position)
{
	this->Position = position;
}

void light::Set_Type(int type)
{
	this->Type = type;
}


 glm::vec3 light::Get_Ambient_Color() const
{
	return this->Ambient_Color;
}


 glm::vec3 light::Get_Diffuse_Color() const
{
	return this->Diffuse_Color;
}

 glm::vec3 light::Get_Specular_Color() const
{
	return this->Specular_Color;
}
 glm::vec3 light::Get_Direction() const
{
	return this->Direction;
}

const glm::vec4 light::Get_Position() const
{
	return this->Position;
}

const int light::Get_Type() const
{
	return this->Type;
}

void light::Set_Tw_mat(glm::mat4x4& transformation)
{
	Tw = transformation;
}

void light::Set_Tm_mat(glm::mat4x4& transformation)
{
	Tm = transformation;
}


void light::Set_Rw_mat(glm::mat4x4& transformation, int flag)
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

void light::Set_Rm_mat(glm::mat4x4& transformation, int flag)
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
	Rm = Rm_x* Rm_z * Rm_y;

}

void light::Set_transmatrix()
{
	this->transmatrix = Rw * Tw * Rm * Tm;
}

const glm::mat4x4 light::Get_transmatrix() const
{

	return this->transmatrix;

}

const float light::Get_alpha() const
{
	return alpha;
}

void light::Set_alpha(float a)
{
	alpha = a;
}

glm::vec3 light::Get_parallel() const
{
	return parallel;
}