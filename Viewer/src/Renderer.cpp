#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <imgui/imgui.h>

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
	delete[] z_buffer_;
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
	float z = 0;
	
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
	z_buffer_ = new float[w * h];
	Set_ZBuffertoMax();
	
	
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
	glm::vec3 color;
	

	 //glm::ivec3 c1(0, 0,1);
	int index0, index1, index2;
	glm::vec3 v0, v1, v2;
	glm::vec3 c0, c1, c2;

	glm::vec4 V0new, V1new, V2new;
	glm::vec4 UpL1, UpL2, UpR1, UpR2, DnL1, DnL2, DnR1, DnR2;
	float maxX = 0, minX = 0, maxY = 0, minY = 0, maxZ = 0, minZ = 0;
	glm::mat4x4 inverse, transmat, projection, st_view, lookat;
	int r = 10, g = 10, b = 10;
	float a = 1.0f / 255.0f;
	
	Set_ZBuffertoMax();
	if (scene.GetModelCount())
	{
		auto model = scene.GetActiveModel();
		inverse = scene.GetActiveCamera().Get_Invtransmatrix();
		projection = scene.GetActiveCamera().GetProjectionTransformation();
		lookat = scene.GetActiveCamera().Get_Lookat();
		st_view = glm::scale(glm::vec3(half_width, half_height, 1)) * glm::translate(glm::vec3(1, 1, 1));
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
				V2new = projection * lookat * inverse* transmat * glm::vec4(v2, 1);
				if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
				{
					V0new /= V0new.w;
					V1new /= V1new.w;
					V2new /= V2new.w;
					st_view = glm::scale(glm::vec3(half_width, half_height, 1)) * glm::translate(glm::vec3(1, 1, 0));
				}
				V0new = st_view * V0new;
				V1new = st_view * V1new;
				V2new = st_view * V2new;
				
				
				Scan_andset_Zbuffer(glm::vec3(V0new.x, V0new.y, V0new.z), glm::vec3(V1new.x, V1new.y, V1new.z), glm::vec3(V2new.x, V2new.y, V2new.z));
				if (scene.GetActiveModel().Get_colorsvar())
				{
					fillthewith_RandomColor(glm::vec3(V0new.x, V0new.y, V0new.z), glm::vec3(V1new.x, V1new.y, V1new.z), glm::vec3(V2new.x, V2new.y, V2new.z));
				}
				
			}
			if (scene.Get_count_oflights() != 0)
			{
				DrawLights(scene, inverse, lookat, projection, st_view);
			}
			for (int i = 0; i < scene.GetActiveModel().GetFacesCount(); i++)  
			{
				index0 = scene.GetActiveModel().GetFace(i).GetVertexIndex(0);
				index1 = scene.GetActiveModel().GetFace(i).GetVertexIndex(1);
				index2 = scene.GetActiveModel().GetFace(i).GetVertexIndex(2);
				v0 = scene.GetActiveModel().GetVertex(index0);
				v1 = scene.GetActiveModel().GetVertex(index1);
				v2 = scene.GetActiveModel().GetVertex(index2);

				transmat = scene.GetActiveModel().Get_transmatrix();

				V0new = projection * lookat * inverse * transmat * glm::vec4(v0, 1);
				V1new = projection * lookat * inverse * transmat * glm::vec4(v1, 1);
				V2new = projection * lookat * inverse * transmat * glm::vec4(v2, 1);
				if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
				{
					V0new /= V0new.w;
					V1new /= V1new.w;
					V2new /= V2new.w;
					st_view = glm::scale(glm::vec3(half_width, half_height, 1)) * glm::translate(glm::vec3(1, 1, 0));
				}
				V0new = st_view * V0new;
				V1new = st_view * V1new;
				V2new = st_view * V2new;
				int Ver1 = model.GetFace(i).GetVertexIndex(0);
				int Ver2 = model.GetFace(i).GetVertexIndex(1);
				int Ver3 = model.GetFace(i).GetVertexIndex(2);
				glm::vec3 v1tp = model.GetVertex(Ver1);
				glm::vec3 v2tp = model.GetVertex(Ver2);
				glm::vec3 v3tp = model.GetVertex(Ver3);
				glm::vec3 faceNormal = normalize(cross(glm::vec3(v1tp - v2tp), glm::vec3(v1tp - v3tp)));
				glm::vec3 Center = (V0new + V1new + V2new) / 3.0f;
				glm::vec4 normalvec = model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(faceNormal, 1);
				int Normali1 = model.GetFace(i).GetNormalIndex(0);
				int Normali2 = model.GetFace(i).GetNormalIndex(1);
				int Normali3 = model.GetFace(i).GetNormalIndex(2);
				glm::vec4 nv1 =  model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali1), 1);
				glm::vec4 nv2 =  model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali2), 1);
				glm::vec4 nv3 =  model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali3), 1);
				color = glm::vec3(0.f, 0.f, 0.f);
				c0 = glm::vec3(0.f, 0.f, 0.f);
				c1 = glm::vec3(0.f, 0.f, 0.f);
				c2 = glm::vec3(0.f, 0.f, 0.f);
				for (int i = 0; i < scene.Get_count_oflights(); i++)
				{
					light& light = scene.GetLight(i);
					glm::mat4x4 transformations = light.Get_transmatrix();
					glm::vec4 position = projection * lookat * inverse * transformations * light.Get_Position();
					glm::vec3 direction;
					if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
						position /= position.w;
					position = st_view * position;
					if (scene.GetshadingLight() == 0) //flat
					{
						if (light.Get_Type() == 2) //point
							direction = glm::normalize(glm::vec3(Center) - glm::vec3(position));
						else
							direction = glm::normalize(light.Get_Direction());
						color += Ambient_color(light.Get_Ambient_Color(), model.Get_modelAmbient_Color());
						color += Diffuse_Color(faceNormal, direction, model.Get_modelDiffuse_Color(), light);
						color += Specular_Color(faceNormal, direction, glm::normalize(scene.GetActiveCamera().Get_Eye()), model.Get_modelSpecular_Color(), light);
					}
					if (scene.GetshadingLight() == 1) //gouraud
					{
						glm::vec3 d1, d2, d3;
						if (light.Get_Type() == 2) //point
						{
							
							d1 = glm::normalize(glm::vec3(V0new) - glm::vec3(position));
							d2 = glm::normalize(glm::vec3(V1new) - glm::vec3(position));
							d3 = glm::normalize(glm::vec3(V2new) - glm::vec3(position));


						}
						else
							d1= glm::normalize(light.Get_Direction()); d2= glm::normalize(light.Get_Direction()); d3= glm::normalize(light.Get_Direction());
						c0 = Diffuse_Color((nv1), d1, model.Get_modelDiffuse_Color(), light);
						c1 = Diffuse_Color((nv2), d2, model.Get_modelDiffuse_Color(), light);
						c2 = Diffuse_Color((nv3), d3, model.Get_modelDiffuse_Color(), light);
						c0 += Ambient_color(light.Get_Ambient_Color(),model.Get_modelAmbient_Color());
						c1 += Ambient_color(light.Get_Ambient_Color(),model.Get_modelAmbient_Color());
						c2 += Ambient_color(light.Get_Ambient_Color(),model.Get_modelAmbient_Color());
						c0 += Specular_Color(glm::vec3(nv1), d1, glm::normalize(scene.GetActiveCamera().Get_Eye()), model.Get_modelSpecular_Color(), light);
						c1 += Specular_Color(glm::vec3(nv2), d2, glm::normalize(scene.GetActiveCamera().Get_Eye()), model.Get_modelSpecular_Color(), light);
						c2 += Specular_Color(glm::vec3(nv3), d3, glm::normalize(scene.GetActiveCamera().Get_Eye()), model.Get_modelSpecular_Color(), light);
						filltheTriangle_gouraud(V0new, V1new, V2new, c0, c1, c2);
					}
					if (scene.GetshadingLight() == 2) //phong
					{
				}
				if(scene.GetshadingLight()==0)//flat
			     	filltheTriangle_Flat(V0new, V1new, V2new, color);
			}

			int c = scene.GetActiveModel().Get_colorsvar();
		/*	if (!scene.GetActiveModel().Get_colorsvar())
			{
				filltheTriangles();
			}
			*/
			
			if (scene.GetActiveModel().Get_showbox())
			{
				maxX = scene.GetActiveModel().Get_maxX();
				maxY = scene.GetActiveModel().Get_maxY();
				maxZ = scene.GetActiveModel().Get_maxZ();
				minX = scene.GetActiveModel().Get_minX();
				minY = scene.GetActiveModel().Get_minY();
				minZ = scene.GetActiveModel().Get_minZ();


				UpR1 = projection * lookat * inverse * transmat * glm::vec4(maxX, maxY, maxZ, 1);
				UpR2 = projection * lookat * inverse * transmat * glm::vec4(maxX, maxY, minZ, 1);
				UpL1 = projection * lookat * inverse * transmat * glm::vec4(minX, maxY, maxZ, 1);
				UpL2 = projection * lookat * inverse * transmat * glm::vec4(minX, maxY, minZ, 1);

				DnR1 = projection * lookat * inverse * transmat * glm::vec4(maxX, minY, maxZ, 1);
				DnR2 = projection * lookat * inverse * transmat * glm::vec4(maxX, minY, minZ, 1);
				DnL1 = projection * lookat * inverse * transmat * glm::vec4(minX, minY, maxZ, 1);
				DnL2 = projection * lookat * inverse * transmat * glm::vec4(minX, minY, minZ, 1);
				if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
				{
					UpR1 /= UpR1.w;
					UpR2 /= UpR2.w;
					UpL1 /= UpL1.w;
					UpL2 /= UpL2.w;

					DnR1 /= DnR1.w;
					DnR2 /= DnR2.w;
					DnL1 /= DnL1.w;
					DnL2 /= DnL2.w;


				}
					UpR1 = st_view * UpR1;
					UpR2 = st_view * UpR2;
					UpL1 = st_view * UpL1;
					UpL2 = st_view * UpL2;

					DnR1 = st_view * DnR1;
					DnR2 = st_view * DnR2;
					DnL1 = st_view * DnL1;
					DnL2 = st_view * DnL2;



					DrawLine(glm::ivec2(UpR1.x, UpR1.y), glm::ivec2(UpR2.x, UpR2.y), c1);
					DrawLine(glm::ivec2(UpR1.x, UpR1.y), glm::ivec2(UpL1.x, UpL1.y), c1);
					DrawLine(glm::ivec2(UpR1.x, UpR1.y), glm::ivec2(DnR1.x, DnR1.y), c1);

					DrawLine(glm::ivec2(UpL2.x, UpL2.y), glm::ivec2(UpR2.x, UpR2.y), c1);
					DrawLine(glm::ivec2(UpL2.x, UpL2.y), glm::ivec2(UpL1.x, UpL1.y), c1);
					DrawLine(glm::ivec2(UpL2.x, UpL2.y), glm::ivec2(DnL2.x, DnL2.y), c1);

					DrawLine(glm::ivec2(DnL1.x, DnL1.y), glm::ivec2(DnR1.x, DnR1.y), c1);
					DrawLine(glm::ivec2(DnL1.x, DnL1.y), glm::ivec2(UpL1.x, UpL1.y), c1);
					DrawLine(glm::ivec2(DnL1.x, DnL1.y), glm::ivec2(DnL2.x, DnL2.y), c1);


					DrawLine(glm::ivec2(DnR2.x, DnR2.y), glm::ivec2(DnR1.x, DnR1.y), c1);
					DrawLine(glm::ivec2(DnR2.x, DnR2.y), glm::ivec2(UpR2.x, UpR2.y), c1);
					DrawLine(glm::ivec2(DnR2.x, DnR2.y), glm::ivec2(DnL2.x, DnL2.y), c1);
				
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
					glm::vec4 v1 =  projection * lookat * inverse * transmat * glm::vec4(model.GetVertex(Ver1), 1);
					glm::vec4 v2 =  projection * lookat * inverse * transmat * glm::vec4(model.GetVertex(Ver2), 1);
					glm::vec4 v3 =  projection * lookat * inverse * transmat * glm::vec4(model.GetVertex(Ver3), 1);
					if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
					{
						v1 /= v1.w;
						v2 /= v2.w;
						v3 /= v3.w;
					}
					v1 = st_view * v1;
					v2 = st_view * v2;
					v3 = st_view * v3;
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
					glm::vec4 v1 =  projection * lookat * inverse * transmat * glm::vec4(model.GetVertex(Ver1), 1);
					glm::vec4 v2 =  projection * lookat * inverse * transmat * glm::vec4(model.GetVertex(Ver2), 1);
					glm::vec4 v3 =  projection * lookat * inverse * transmat * glm::vec4(model.GetVertex(Ver3), 1);
					if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
					{
						v1 /= v1.w;
						v2 /= v2.w;
						v3 /= v3.w;
					}

					v1 = st_view * v1;
					v2 = st_view * v2;
					v3 = st_view * v3;
					glm::vec4 nv1 = glm::scale(glm::vec3(50, 50, 50)) * model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali1), 1);
					glm::vec4 nv2 = glm::scale(glm::vec3(50, 50, 50)) * model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali2), 1);
					glm::vec4 nv3 = glm::scale(glm::vec3(50, 50, 50)) * model.Get_Rw_mat() * model.Get_Rm_mat() * glm::vec4(model.Get_normalvertex(Normali3), 1);
					DrawLine(glm::ivec2(v1.x, v1.y), glm::ivec2(nv1.x + v1.x, nv1.y + v1.y), c1);
					DrawLine(glm::ivec2(v2.x, v2.y), glm::ivec2(nv2.x + v2.x, nv2.y + v2.y), c1);
					DrawLine(glm::ivec2(v3.x, v3.y), glm::ivec2(nv3.x + v3.x, nv3.y + v3.y), c1);
				}
			}
			
		
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

void Renderer::Scan_andset_Zbuffer(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	/*glm::vec3 _color = glm::vec3(0.1,0.5, 1);
	if (p1.x >= p2.x && p1.x >= p3.x)
	{
		maxX = p1.x;
	}
	if (p2.x >= p1.x && p2.x >= p3.x)
	{
		maxX = p2.x;
	}
	if (p3.x >= p2.x && p3.x >= p1.x)
	{
		maxX = p3.x;
	}
	//minx
	if (p1.x <= p2.x && p1.x <= p3.x)
	{
		minX = p1.x;
	}
	if (p2.x <= p1.x && p2.x <= p3.x)
	{
		minX = p2.x;
	}
	if (p3.x <= p2.x && p3.x <= p1.x)
	{
		minX = p3.x;
	}
	//maxy
	if (p1.y >= p2.y && p1.y >= p3.y)
	{
		maxY = p1.y;
	}
	if (p2.y >= p1.y && p2.y >= p3.y)
	{
		maxY = p2.y;
	}
	if (p3.y >= p2.y && p3.y >= p1.y)
	{
		maxY = p3.y;
	}
	//miny
	if (p1.y <= p2.y && p1.y <= p3.y)
	{
		minY = p1.y;
	}
	if (p2.y <= p1.y && p2.y <= p3.y)
	{
		minY = p2.y;
	}
	if (p3.y <= p2.y && p3.y <= p1.y)
	{
		minY = p3.y;
	}
	*/
	float minY = std::min(std::min(p1.y, p2.y), p3.y);
	float maxY = std::max(std::max(p1.y, p2.y), p3.y);
	float minX = std::min(std::min(p1.x, p2.x), p3.x);
	float maxX = std::max(std::max(p1.x, p2.x), p3.x);
	for (int j = maxY; j >= minY; j--)
	{
		for (int i = minX; i <= maxX; i++)
		{
			if (IsInsidetheTrianle(i, j,p1.x, p1.y, p2.x, p2.y, p3.x, p3.y))
			{
				glm::vec3 z= Calc_z(i, j, p1, p2, p3,p1,p2,p3);
				if (z.z <= Get_Z_value(i,j))
				{
					maxbufferZ = std::max(maxbufferZ, z.z);
					minbufferZ = std::min(minbufferZ, z.z);
					Set_Z_value(i, j, z.z);
			       
				}
				
			}
		}
		
	}
	
}

void Renderer::Set_ZBuffertoMax()
{
	minbufferZ = FLT_MAX;
	maxbufferZ = FLT_MIN;
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			z_buffer_[Z_INDEX(viewport_width_, i, j)] = FLT_MAX;

			color_buffer_[INDEX(viewport_width_, i, j, 0)] = 0.f;
			color_buffer_[INDEX(viewport_width_, i, j, 1)] = 0.f;
			color_buffer_[INDEX(viewport_width_, i, j, 2)] = 0.f;
		}
	}
}

glm::vec3 Renderer::Calc_z(int x, int y, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& val1, const glm::vec3& val2, const glm::vec3& val3)
{
	float A1 = Calc_area(x, y, p1.x, p1.y, p2.x, p2.y);
    float A2 = Calc_area(x, y, p1.x, p1.y, p3.x, p3.y);
	float A3 = Calc_area(x, y, p3.x, p3.y, p2.x, p2.y);
	float A = A1 + A2 + A3;
	glm::vec3 alpha = (A1 / A) * val3 ;
	glm::vec3 beta = (A2 / A )* val2;
	glm::vec3 gamma = (A3 / A) * val1;
	return (alpha + beta + gamma);
}

float Renderer::Calc_area(int x1, int y1, float x2, float y2, float x3, float y3)
{
	return abs((x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2)) / 2.0); //https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
}

/* A function to check whether point P(x, y) lies inside the triangle formed
   by A(x1, y1), B(x2, y2) and C(x3, y3) */ //https://www.geeksforgeeks.org/check-whether-a-given-point-lies-inside-a-triangle-or-not/
bool Renderer::IsInsidetheTrianle( int x, int y ,int x1, int y1, int x2, int y2, int x3, int y3)
{
	
	
	float A = Calc_area(x1, y1, x2, y2, x3, y3);

	float A1 = Calc_area(x, y, x2, y2, x3, y3);

	float A2 = Calc_area(x1, y1, x, y, x3, y3);


	float A3 = Calc_area(x1, y1, x2, y2, x, y);

	return (A == A1 + A2 + A3); 
	
}


void Renderer::Set_Z_value(int i, int j, float z)
{
	if (i < 0) return; if (i >= viewport_width_) return;
	if (j < 0) return; if (j >= viewport_height_) return;
	 this->z_buffer_[Z_INDEX(viewport_width_, i , j)] = z;
}

float Renderer::Get_Z_value(int i, int j)
{
	if (i < 0) return 0; if (i >= viewport_width_) return 0;
	if (j < 0) return 0; if (j >= viewport_height_) return 0;
	return this->z_buffer_[Z_INDEX(viewport_width_, i, j)];
}

void Renderer::filltheTriangles()
{
	glm::vec3 _color;
	
	for (int i = 0; i < viewport_width_; i++)
	{
		for (int j = 0; j < viewport_height_; j++)
		{
			float z = Get_Z_value(i, j);
			if (z != FLT_MAX)
			{
				
					_color = Get_GrayColor(z);
					PutPixel(i, j, _color);
				
				
			}

		}
	}

}

glm::vec3 Renderer::Get_GrayColor(float z)
{
	float a = 1 / (maxbufferZ - minbufferZ);
    float b = -1 * a * minbufferZ;
    float c = 1 - (a * z + b);
    glm::vec3 _color(c, c, c);
	 return _color;
}

void Renderer::fillthewith_RandomColor(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3)
{
	float minY = std::min(std::min(p1.y, p2.y), p3.y);
	float maxY = std::max(std::max(p1.y, p2.y), p3.y);
	float minX = std::min(std::min(p1.x, p2.x), p3.x);
	float maxX = std::max(std::max(p1.x, p2.x), p3.x);
	float temp = 255 * 255;
	glm::vec3 random_color = glm::vec3(static_cast <float>(rand() / temp), static_cast <float>( rand() / temp), static_cast <float>( rand() / temp));
	for (int j = maxY; j >= minY; j--)
	{
		for (int i = minX; i <= maxX; i++)
		{
			if (IsInsidetheTrianle(i, j, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y))
			{
				glm::vec3 z = Calc_z(i, j, p1, p2, p3,p1,p2,p3);
				if (z.z <= Get_Z_value(i, j))
				{
					PutPixel(i, j, random_color);
				}

			}
		}
	}
}

void Renderer::DrawLights(const Scene& scene, glm::mat4x4& inverse, glm::mat4x4& lookat, glm::mat4x4& projection, glm::mat4x4& st_view)
{
	int count = scene.Get_count_oflights();
	glm::mat4x4 transmat;

	for (int i = 0; i < count; i++)
	{
		int t = scene.GetLight(i).Get_Type();
		if (t == 2) //point
		{
			transmat = scene.GetLight(i).Get_transmatrix();

			glm::vec4 pos = projection * lookat * inverse * transmat * scene.GetLight(i).Get_Position();
			if (!(scene.GetActiveCamera().Get_OrthoGraphic()))
			{
				pos /= pos.w;
			}
			pos = st_view * pos;
			glm::vec3 v1_light(pos.x, pos.y + 20, -pos.z);
			glm::vec3 v2_light(pos.x + 20, pos.y -20, -pos.z);
			glm::vec3 v3_light(pos.x - 20, pos.y - 20, -pos.z);
			float minY = std::min(std::min(v1_light.y, v2_light.y), v3_light.y);
			float maxY = std::max(std::max(v1_light.y, v2_light.y), v3_light.y);
			float minX = std::min(std::min(v1_light.x, v2_light.x), v3_light.x);
			float maxX = std::max(std::max(v1_light.x, v2_light.x), v3_light.x);

			glm::vec3 white_color = glm::vec3(1, 1, 1);
			for (int j = maxY; j >= minY; j--)
			{
				for (int i = minX; i <= maxX; i++)
				{
					if (IsInsidetheTrianle(i, j, v1_light.x, v1_light.y, v2_light.x, v2_light.y, v3_light.x, v3_light.y))
					{
						glm::vec3 z = Calc_z(i, j, v1_light, v2_light, v3_light, v1_light, v2_light, v3_light);
						if (z.z <= Get_Z_value(i, j))
						{
							PutPixel(i, j, white_color);
							Set_Z_value(i, j, z.z);
						}

					}
				}
			}

		}
	}

}

glm::vec3 Renderer::Ambient_color(glm::vec3& light, glm::vec3& model)
{
	return glm::vec3(light.x * model.x, light.y * model.y, light.z * model.z);
}

glm::vec3 Renderer::Diffuse_Color(glm::vec3 n, glm::vec3& i , glm::vec3& color, light& light) 
{
	glm::vec3 Lcolor = light.Get_Diffuse_Color();
	glm::vec3 Color(Lcolor.x * color.x, Lcolor.y * color.y, Lcolor.z * color.z);
	float I_n = glm::dot(-n, i);
	return Color * I_n;
}
glm::vec3 Renderer::Specular_Color(glm::vec3& n, glm::vec3& i, glm::vec3& v,glm::vec3& color, light& light)
{
	int alpha = light.Get_alpha();
	glm::vec3 Lcolor = light.Get_Specular_Color();
	glm::vec3 Color(Lcolor.x * color.x, Lcolor.y * color.y, Lcolor.z * color.z);
	glm::vec3 r = (2.f * glm::dot(-n, i) * n - i);
	float t = std::pow(std::max(0.f, glm::dot(r, v)), alpha); //
	return t * Color;
}

void Renderer::filltheTriangle_Flat(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3 color)
{
	float minY = std::min(std::min(p1.y, p2.y), p3.y);
	float maxY = std::max(std::max(p1.y, p2.y), p3.y);
	float minX = std::min(std::min(p1.x, p2.x), p3.x);
	float maxX = std::max(std::max(p1.x, p2.x), p3.x);
	for (int j = maxY; j >= minY; j--)
	{
		for (int i = minX; i <= maxX; i++)
		{
			if (IsInsidetheTrianle(i, j, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y))
			{
				glm::vec3 z = Calc_z(i, j, p1, p2, p3, p1, p2, p3);
				if (z.z <= Get_Z_value(i, j))
				{
					//if (i < 0) return; if (i >= viewport_width_) return;
					//if (j < 0) return; if (j >= viewport_height_) return;
					
					color_buffer_[INDEX(viewport_width_, i, j, 0)] += color.x;
					color_buffer_[INDEX(viewport_width_, i, j, 1)] += color.y;
					color_buffer_[INDEX(viewport_width_, i, j, 2)] += color.z;

					if (color.x > 1.f)
						color_buffer_[INDEX(viewport_width_, i, j, 0)] = 1.f;

					if (color.y > 1.f)
						color_buffer_[INDEX(viewport_width_, i, j, 1)] = 1.f;

					if (color.z > 1.f)
						color_buffer_[INDEX(viewport_width_, i, j, 2)] = 1.f;

				}

			}
		}

	}
}
void Renderer::filltheTriangle_gouraud(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& c1, glm::vec3& c2, glm::vec3& c3)
{
	float minY = std::min(std::min(p1.y, p2.y), p3.y);
	float maxY = std::max(std::max(p1.y, p2.y), p3.y);
	float minX = std::min(std::min(p1.x, p2.x), p3.x);
	float maxX = std::max(std::max(p1.x, p2.x), p3.x);
	for (int j = maxY; j >= minY; j--)
	{
		for (int i = minX; i <= maxX; i++)
		{
			if (IsInsidetheTrianle(i, j, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y))
			{
				glm::vec3 z = Calc_z(i, j, p1, p2, p3, p1, p2, p3);
				if (z.z <= Get_Z_value(i, j))
				{
					//if (i < 0) return; if (i >= viewport_width_) return;
					//if (j < 0) return; if (j >= viewport_height_) return;
					glm::vec3 c = Calc_z(i, j, p1, p2, p3, c1, c2, c3);
					color_buffer_[INDEX(viewport_width_, i, j, 0)] += c.x;
					color_buffer_[INDEX(viewport_width_, i, j, 1)] += c.y;
					color_buffer_[INDEX(viewport_width_, i, j, 2)] += c.z;

					if (c.x > 1.f)
						color_buffer_[INDEX(viewport_width_, i, j, 0)] = 1.f;
						
					if (c.y > 1.f)
						color_buffer_[INDEX(viewport_width_, i, j, 1)] = 1.f;
					
					if (c.z > 1.f)
						color_buffer_[INDEX(viewport_width_, i, j, 2)] = 1.f;
					
						
					

				}

			}
		}

	}
}