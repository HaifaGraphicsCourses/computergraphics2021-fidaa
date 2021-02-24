#include "light.h"

#include <iostream>
#include <fstream>
#include <sstream>

light::light(glm::vec3 p, int type)
{
	Ambient_Color = glm::vec3(0.5, 0.5, 0.5);
	Diffuse_Color = glm::vec3(0.5, 0.5, 0.5);
	Specular_Color = glm::vec3(0.5, 0.5, 0.5);
	Direction = glm::vec3(0, 0, -1);
	Position = p;
	Type = type;
	if (Type == 2) //point
	{
		float point[3] = { p.x,p.y,p.z };
		glGenVertexArrays(1, &Vao_l);
		glGenBuffers(1, &Vbo_l);

		glBindVertexArray(Vao_l);
		glBindBuffer(GL_ARRAY_BUFFER, Vbo_l);
		glBufferData(GL_ARRAY_BUFFER, 3 * sizeof(Vertex), point, GL_STATIC_DRAW);

		// Vertex Positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);

		// Vertex Texture Coords
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);
		
		// unbind to make sure other code does not change it somewhere else
		glBindVertexArray(0);
		glGenVertexArrays(1, &Vao_l);
		
	}
	else
	{
		glm::vec3 direction = normalize(Direction);
		direction = 0.014f * direction + parallel;
		float point[24] = { parallel.x, parallel.y, parallel.z,direction.x, direction.y, direction.z,
							parallel.x + 0.005, parallel.y, parallel.z,direction.x + 0.005, direction.y, direction.z,
							parallel.x + 0.006, parallel.y, parallel.z,direction.x + 0.006, direction.y, direction.z,
							parallel.x + 0.007, parallel.y, parallel.z,direction.x + 0.007, direction.y, direction.z };
		glGenVertexArrays(1, &Vao_l);
		glGenBuffers(1, &Vbo_l);
		glBindVertexArray(Vao_l);
		glBindBuffer(GL_ARRAY_BUFFER, Vbo_l);
		glBufferData(GL_ARRAY_BUFFER, sizeof(point), &point, GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	}
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

void light::Set_Position(glm::vec3 position)
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

const glm::vec3 light::Get_Position() const
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


GLuint light::GetVAO() const
{
	return Vao_l;
}