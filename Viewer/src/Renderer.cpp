#define _USE_MATH_DEFINES
#include <cmath>
#include <algorithm>

#include "Renderer.h"
#include "InitShader.h"

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
	
	const glm::ivec3 c1(1, 0, 1);
	/*
	int r = 100, a = 50;
	float temp = 2.f * M_PI / a;
	const glm::ivec2 p1(400, 500);
	for (int i = 0; i <= a; i++)
	{
		glm::ivec2 p2 = glm::ivec2(p1.x + r * sin(temp * i), p1.y + r*cos(temp * i));
		DrawLine(p1, p2,c1);
	
	}
	glm::ivec2 D1(100, 200);
	glm::ivec2 D2(400, 200);
	glm::ivec2 D3(150, 300);
	glm::ivec2 D4(350, 300);
	glm::ivec2 D5(250, 20);
	glm::ivec2 D6(250, 300);
	glm::ivec2 D7(200, 200);
	glm::ivec2 D8(300, 200);

	DrawLine(D1, D2, c1);
	DrawLine(D1, D5, c1);
	DrawLine(D1, D3, c1);
	DrawLine(D2, D5, c1);
	DrawLine(D2, D4, c1);
	DrawLine(D3, D4, c1);
	DrawLine(D3, D7, c1);
	DrawLine(D6, D7, c1);
	DrawLine(D6, D8, c1);
	DrawLine(D4, D8, c1);*/

	glm::ivec2 D1(500, 0);
	glm::ivec2 D2(700, 300);
	glm::ivec2 D3(700, 600);
	glm::ivec2 D4(950, 700);
	glm::ivec2 D5(800, 500);
	glm::ivec2 D6(850, 200);
	glm::ivec2 D7(550, 150);
	glm::ivec2 D8(580, 240);

	glm::ivec2 D9(300, 300);
	glm::ivec2 D10(300, 600);
	glm::ivec2 D11(50, 700);
	glm::ivec2 D12(200, 500);
	glm::ivec2 D13(150, 200);
	glm::ivec2 D14(450, 150);
	glm::ivec2 D15(420, 240);
	



	
	DrawLine(D1, D2, c1);
	DrawLine(D1, D3, c1);
	DrawLine(D3, D4, c1);
	DrawLine(D3, D5, c1);
	DrawLine(D4, D5, c1);
	DrawLine(D5, D6, c1);
	DrawLine(D6, D7, c1);
	DrawLine(D2, D6, c1);
	DrawLine(D2, D5, c1);
	DrawLine(D2, D8, c1);
	DrawLine(D2, D3, c1);

	DrawLine(D1, D9, c1);
	DrawLine(D1, D10, c1);
	DrawLine(D10, D11, c1);
	DrawLine(D11, D12, c1);
	DrawLine(D12, D13, c1);
	DrawLine(D13, D14, c1);
	DrawLine(D12, D10, c1);
	DrawLine(D10, D3, c1);
	DrawLine(D9, D12, c1);
	DrawLine(D13, D9, c1);
	DrawLine(D9, D10, c1);
	DrawLine(D9, D15, c1);

	


}

int Renderer::GetViewportWidth() const
{
	return viewport_width_;
}

int Renderer::GetViewportHeight() const
{
	return viewport_height_;
}