#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define INDEX(width,x,y,c) ((x)+(y)*(width))*3+(c)
#define Z_INDEX(width,x,y) ((x)+(y)*(width))

Renderer::Renderer(int viewport_width, int viewport_height) :
	viewport_width_(viewport_width),
	viewport_height_(viewport_height)
{
	InitOpenGLRendering();
	CreateBuffers(viewport_width, viewport_height);
}

Renderer::~Renderer()
{
	delete[] color_buffer_;
}

void Renderer::PutPixel(int i, int j, const glm::vec3& color)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	
	color_buffer_[INDEX(viewport_width_, i, j, 0)] = color.x;
	color_buffer_[INDEX(viewport_width_, i, j, 1)] = color.y;
	color_buffer_[INDEX(viewport_width_, i, j, 2)] = color.z;
}

void Renderer::DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color)
{
	// TODO: Implement bresenham algorithm #################################################################
	// 0 < a < 1
	int dx = p2.x - p1.x;
	int dy = p2.y - p1.y;
	if (abs(dy) < abs(dx))
	{
		if (p1.x < p2.x)
		{
			int x1 = p1.x;
			int y1 = p1.y;
			int yi = 1;
			int e = -dx;
			if (dy < 0)
			{
				yi = -1;
				dy = -dy;
			}
			while (x1 <= p2.x)
			{
				if (e > 0)
				{
					y1 += yi;
					e -= 2 * dx;
				}
				PutPixel(x1, y1, color);
				x1 += 1;
				e += 2 * dy;

			}
		}
		else if (p1.x > p2.x)
		{
			int x2 = p2.x;
			int y2 = p2.y;
			int yi = 1;
			int e;
			dx = p1.x - p2.x;
			dy = p1.y - p2.y;
			e = -dx;
			
			if (dy < 0)
			{
				yi = -1;
				dy = -dy;
			}
			while (x2 <= p1.x)
			{
				if (e > 0)
				{
					y2 += yi;
					e -= 2 * dx;
				}
				PutPixel(x2, y2, color);
				x2 += 1;
				e += 2 * dy;

			}

		}
	}
	else
	{
		if (p1.y < p2.y)
		{

			int x1 = p1.x;
			int y1 = p1.y;
			int xi = 1;
			int e;
			dx = p2.x - p1.x;
			dy = p2.y - p1.y;
			e = -dy;

			if (dx < 0)
			{
				xi = -1;
				dx = -dx;
			}
			while (y1 <= p2.y)
			{
				if (e > 0)
				{
					x1 += xi;
					e -= 2 * dy;
				}
				PutPixel(x1, y1, color);
				y1 += 1;
				e += 2 * dx;

			}
			

		}
		else if (p1.y > p2.y)
		{
			int x2 = p2.x;
			int y2 = p2.y;
			int xi = 1;
			int e;
			dx = p1.x - p2.x;
			dy = p1.y - p2.y;
			e = -dy;

			if (dx < 0)
			{
				xi = -1;
				dx = -dx;
			}
			while (y2 <= p1.y)
			{
				if (e > 0)
				{
					x2 += xi;
					e -= 2 * dy;
				}
				PutPixel(x2, y2, color);
				y2 += 1;
				e += 2 * dx;

			}



		}

	}
	// https://en.wikipedia.org/wiki/Bresenham%27s_line_algorithm
}

void Renderer::CreateBuffers(int w, int h)
{
	CreateOpenGLBuffer(); //Do not remove this line.
	color_buffer_ = new float[3 * w * h];
	ClearColorBuffer(glm::vec3(0.0f, 0.0f, 0.0f));
}

//##############################
//##OpenGL stuff. Don't touch.##
//##############################

// Basic tutorial on how opengl works:
// http://www.opengl-tutorial.org/beginners-tutorials/tutorial-2-the-first-triangle/
// don't linger here for now, we will have a few tutorials about opengl later.
void Renderer::InitOpenGLRendering()
{
	// Creates a unique identifier for an opengl texture.
	glGenTextures(1, &gl_screen_tex_);

	// Same for vertex array object (VAO). VAO is a set of buffers that describe a renderable object.
	glGenVertexArrays(1, &gl_screen_vtc_);

	GLuint buffer;

	// Makes this VAO the current one.
	glBindVertexArray(gl_screen_vtc_);

	// Creates a unique identifier for a buffer.
	glGenBuffers(1, &buffer);

	// (-1, 1)____(1, 1)
	//	     |\  |
	//	     | \ | <--- The exture is drawn over two triangles that stretch over the screen.
	//	     |__\|
	// (-1,-1)    (1,-1)
	const GLfloat vtc[]={
		-1, -1,
		 1, -1,
		-1,  1,
		-1,  1,
		 1, -1,
		 1,  1
	};

	const GLfloat tex[]={
		0,0,
		1,0,
		0,1,
		0,1,
		1,0,
		1,1};

	// Makes this buffer the current one.
	glBindBuffer(GL_ARRAY_BUFFER, buffer);

	// This is the opengl way for doing malloc on the gpu. 
	glBufferData(GL_ARRAY_BUFFER, sizeof(vtc)+sizeof(tex), NULL, GL_STATIC_DRAW);

	// memcopy vtc to buffer[0,sizeof(vtc)-1]
	glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vtc), vtc);

	// memcopy tex to buffer[sizeof(vtc),sizeof(vtc)+sizeof(tex)]
	glBufferSubData(GL_ARRAY_BUFFER, sizeof(vtc), sizeof(tex), tex);

	// Loads and compiles a sheder.
	GLuint program = InitShader( "vshader.glsl", "fshader.glsl" );

	// Make this program the current one.
	glUseProgram(program);

	// Tells the shader where to look for the vertex position data, and the data dimensions.
	GLint  vPosition = glGetAttribLocation( program, "vPosition" );
	glEnableVertexAttribArray( vPosition );
	glVertexAttribPointer( vPosition,2,GL_FLOAT,GL_FALSE,0,0 );

	// Same for texture coordinates data.
	GLint  vTexCoord = glGetAttribLocation( program, "vTexCoord" );
	glEnableVertexAttribArray( vTexCoord );
	glVertexAttribPointer( vTexCoord,2,GL_FLOAT,GL_FALSE,0,(GLvoid *)sizeof(vtc) );

	//glProgramUniform1i( program, glGetUniformLocation(program, "texture"), 0 );

	// Tells the shader to use GL_TEXTURE0 as the texture id.
	glUniform1i(glGetUniformLocation(program, "texture"),0);
}

void Renderer::CreateOpenGLBuffer()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// malloc for a texture on the gpu.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, viewport_width_, viewport_height_, 0, GL_RGB, GL_FLOAT, NULL);
	glViewport(0, 0, viewport_width_, viewport_height_);
}

void Renderer::SwapBuffers()
{
	// Makes GL_TEXTURE0 the current active texture unit
	glActiveTexture(GL_TEXTURE0);

	// Makes glScreenTex (which was allocated earlier) the current texture.
	glBindTexture(GL_TEXTURE_2D, gl_screen_tex_);

	// memcopy's colorBuffer into the gpu.
	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, viewport_width_, viewport_height_, GL_RGB, GL_FLOAT, color_buffer_);

	// Tells opengl to use mipmapping
	glGenerateMipmap(GL_TEXTURE_2D);

	// Make glScreenVtc current VAO
	glBindVertexArray(gl_screen_vtc_);

	// Finally renders the data.
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

void Renderer::ClearColorBuffer(const glm::vec3& color)
{
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			PutPixel(i, j, color);
		}
	}
}

void Renderer::Render(const Scene& scene)
{
	int half_width = viewport_width_ / 2;
	int half_height = viewport_height_ / 2;

	const glm::ivec3 c1(0, 0,1);
	int index0, index1, index2;
	glm::vec3 v0, v1, v2;

	glm::vec4 V0new, V1new, V2new;
	glm::vec4 UpL1, UpL2, UpR1, UpR2, DnL1, DnL2, DnR1, DnR2;
	float maxX = 0, minX = 0, maxY = 0, minY = 0, maxZ = 0, minZ = 0;
	glm::mat4x4 inverse, transmat, projection, st_view, lookat;
	if (scene.GetModelCount())
	{
		auto model = scene.GetActiveModel();
		
		inverse = scene.GetActiveCamera().Get_Invtransmatrix();
		projection = scene.GetActiveCamera().GetProjectionTransformation();
		lookat = scene.GetActiveCamera().Get_Lookat();
		st_view = glm::scale(glm::vec3(half_width , half_height, 1)) * glm::translate(glm::vec3(1, 1, 0));
			for (int i = 0; i < scene.GetActiveModel().GetFacesCount(); i++)   // #2
			{
				index0 = scene.GetActiveModel().GetFace(i).GetVertexIndex(0);
				index1 = scene.GetActiveModel().GetFace(i).GetVertexIndex(1);
				index2 = scene.GetActiveModel().GetFace(i).GetVertexIndex(2);
				v0 = scene.GetActiveModel().GetVertex(index0);
				v1 = scene.GetActiveModel().GetVertex(index1);
				v2 = scene.GetActiveModel().GetVertex(index2);
			
				transmat = scene.GetActiveModel().Get_transmatrix() ;
				
				V0new = projection * lookat * inverse* transmat * glm::vec4(v0, 1);
				V1new = projection * lookat * inverse* transmat* glm::vec4(v1, 1);
				V2new = projection * lookat * inverse*transmat * glm::vec4(v2, 1);
				if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
				{
					V0new /= V0new.w;
					V1new /= V1new.w;
					V2new /= V2new.w;
				}
				V0new = st_view * V0new;
				V1new = st_view * V1new;
				V2new = st_view * V2new;
				DrawLine(glm::ivec2(V0new.x , V0new.y ), glm::ivec2(V1new.x , V1new.y ), c1);
				DrawLine(glm::ivec2(V0new.x , V0new.y ), glm::ivec2(V2new.x , V2new.y ), c1);
				DrawLine(glm::ivec2(V1new.x , V1new.y ), glm::ivec2(V2new.x , V2new.y ), c1);
				

			}
			if (scene.GetActiveModel().Get_showbox())
			{
				maxX = scene.GetActiveModel().Get_maxX();
				maxY = scene.GetActiveModel().Get_maxY();
				maxZ = scene.GetActiveModel().Get_maxZ();
				minX = scene.GetActiveModel().Get_minX();
				minY = scene.GetActiveModel().Get_minY();
				minZ = scene.GetActiveModel().Get_minZ();


				UpR1 = transmat * glm::vec4(maxX, maxY, maxZ, 1);
				UpR2 = transmat * glm::vec4(maxX, maxY, minZ, 1);
				UpL1 = transmat * glm::vec4(minX, maxY, maxZ, 1);
				UpL2 = transmat * glm::vec4(minX, maxY, minZ, 1);

				DnR1 = transmat * glm::vec4(maxX, minY, maxZ, 1);
				DnR2 = transmat * glm::vec4(maxX, minY, minZ, 1);
				DnL1 = transmat * glm::vec4(minX, minY, maxZ, 1);
				DnL2 = transmat * glm::vec4(minX, minY, minZ, 1);
				//R1
				DrawLine(glm::ivec2(UpR1.x / UpR1.w, UpR1.y / UpR1.w), glm::ivec2(UpR2.x / UpR2.w, UpR2.y / UpR2.w), c1);
				DrawLine(glm::ivec2(UpR1.x / UpR1.w, UpR1.y / UpR1.w), glm::ivec2(UpL1.x / UpL1.w, UpL1.y / UpL1.w), c1);
				DrawLine(glm::ivec2(UpR1.x / UpR1.w, UpR1.y / UpR1.w), glm::ivec2(DnR1.x / DnR1.w, DnR1.y / DnR1.w), c1);

				DrawLine(glm::ivec2(UpL2.x / UpL2.w, UpL2.y / UpL2.w), glm::ivec2(UpR2.x / UpR2.w, UpR2.y / UpR2.w), c1);
				DrawLine(glm::ivec2(UpL2.x / UpL2.w, UpL2.y / UpL2.w), glm::ivec2(UpL1.x / UpL1.w, UpL1.y / UpL1.w), c1);
				DrawLine(glm::ivec2(UpL2.x / UpL2.w, UpL2.y / UpL2.w), glm::ivec2(DnL2.x / DnL2.w, DnL2.y / DnL2.w), c1);

				DrawLine(glm::ivec2(DnL1.x / DnL1.w, DnL1.y / DnL1.w), glm::ivec2(DnR1.x / DnR1.w, DnR1.y / DnR1.w), c1);
				DrawLine(glm::ivec2(DnL1.x / DnL1.w, DnL1.y / DnL1.w), glm::ivec2(UpL1.x / UpL1.w, UpL1.y / UpL1.w), c1);
				DrawLine(glm::ivec2(DnL1.x / DnL1.w, DnL1.y / DnL1.w), glm::ivec2(DnL2.x / DnL2.w, DnL2.y / DnL2.w), c1);


				DrawLine(glm::ivec2(DnR2.x / DnR2.w, DnR2.y / DnR2.w), glm::ivec2(DnR1.x / DnR1.w, DnR1.y / DnR1.w), c1);
				DrawLine(glm::ivec2(DnR2.x / DnR2.w, DnR2.y / DnR2.w), glm::ivec2(UpR2.x / UpR2.w, UpR2.y / UpR2.w), c1);
				DrawLine(glm::ivec2(DnR2.x / DnR2.w, DnR2.y / DnR2.w), glm::ivec2(DnL2.x / DnL2.w, DnL2.y / DnL2.w), c1);
			}
			//auto model = scene.GetActiveModel();
			if (model.Get_facenormals())
			{
				for (int i = 0; i < model.GetFacesCount(); ++i)
				{
					
					int Ver1 = model.GetFace(i).GetVertexIndex(0);
					int Ver2 = model.GetFace(i).GetVertexIndex(1);
					int Ver3 = model.GetFace(i).GetVertexIndex(2);
					glm::vec3 v1tp = model.GetVertex(Ver1);
					glm::vec3 v2tp = model.GetVertex(Ver2);
					glm::vec3 v3tp = model.GetVertex(Ver3);
					glm::vec3 faceNormal = normalize(cross(glm::vec3(v1tp - v2tp), glm::vec3(v1tp - v3tp)));
					glm::vec4 v1 = transmat * glm::vec4(model.GetVertex(Ver1), 1);
					glm::vec4 v2 = transmat * glm::vec4(model.GetVertex(Ver2), 1);
					glm::vec4 v3 = transmat * glm::vec4(model.GetVertex(Ver3), 1);
					glm::vec3 Center = (v1 + v2 + v3) / 3.0f;
					glm::vec4 normalvec = glm::scale(glm::vec3(50,50,50)) * model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(faceNormal, 1) + glm::vec4(Center, 0);
					DrawLine(glm::ivec2(Center.x, Center.y), glm::ivec2(normalvec.x, normalvec.y), c1);
				}
			}
			if (model.Get_vernormals())
			{
				for (int i = 0; i < model.GetFacesCount(); i++)
				{
					int Ver1 = model.GetFace(i).GetVertexIndex(0);
					int Ver2 = model.GetFace(i).GetVertexIndex(1);
					int Ver3 = model.GetFace(i).GetVertexIndex(2);
					int Normali1 = model.GetFace(i).GetNormalIndex(0);
					int Normali2 = model.GetFace(i).GetNormalIndex(1);
					int Normali3 = model.GetFace(i).GetNormalIndex(2);
					glm::vec4 v1 = transmat * glm::vec4(model.GetVertex(Ver1), 1);
					glm::vec4 v2 = transmat * glm::vec4(model.GetVertex(Ver2), 1);
					glm::vec4 v3 = transmat * glm::vec4(model.GetVertex(Ver3), 1);
					glm::vec4 nv1 = glm::scale(glm::vec3(50, 50, 50)) * model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali1), 1);
					glm::vec4 nv2 = glm::scale(glm::vec3(50, 50, 50)) * model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali2), 1);
					glm::vec4 nv3 = glm::scale(glm::vec3(50, 50, 50)) * model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali3), 1);
					DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(nv1.x + v1.x, nv1.y + v1.y), c1);
					DrawLine(glm::ivec2(v2.x, v2.y), glm::ivec2(nv2.x + v2.x, nv2.y + v2.y), c1);
					DrawLine(glm::ivec2(v3.x, v3.y), glm::ivec2(nv3.x + v3.x, nv3.y + v3.y), c1);
				}
			}
			// #1 
		/*
		model.PrintFaces();

		model.PrintVertices();
		exit(0); */
	}
	return;

}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}

void Renderer::SetViewportWidth(int New_width)
{
	viewport_width_ = New_width;
	

}

void Renderer::SetViewportHeight(int New_height)
{
	viewport_height_ = New_height;

}

