#pragma once
#include "Scene.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

class Renderer
{
public:
	Renderer(int viewportWidth, int viewportHeight);
	
	virtual ~Renderer();
	void Render(const Scene& scene);
	void SwapBuffers();
	void ClearColorBuffer(const glm::vec3& color);
	int GetViewportWidth() const;
	int GetViewportHeight() const;
	void Scan_andset_Zbuffer(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	void Set_ZBuffertoMax();
	void Set_Z_value(int i, int j, float z);
	float Get_Z_value(int i, int j);
	glm::vec3 Calc_z(int x, int y, const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, const glm::vec3& val1, const glm::vec3& val2, const glm::vec3& val3);
	float Calc_area(int x1, int y1, float x2, float y2, float x3, float y3);
	bool IsInsidetheTrianle( int x, int y,int x1, int y1, int x2, int y2, int x3, int y3);
	glm::vec3 Get_GrayColor(float z);
	void filltheTriangles();
	void fillthewith_RandomColor(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3);
	
	void DrawLights(const Scene& scene, glm::mat4x4& inverse, glm::mat4x4& lookat, glm::mat4x4& projection, glm::mat4x4& st_view);
	glm::vec3 Ambient_color(glm::vec3& light, glm::vec3& model);
	glm::vec3 Diffuse_Color(glm::vec3 n, glm::vec3& i, glm::vec3& color, light& light);
	glm::vec3 Specular_Color(glm::vec3& n, glm::vec3& i, glm::vec3& v, glm::vec3& color, light& light);
	void filltheTriangle_Flat(const glm::vec3& v1, const glm::vec3& v2, const glm::vec3& v3, glm::vec3 color);
	void filltheTriangle_gouraud(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3& c1, glm::vec3& c2, glm::vec3& c3);
	void filltheTriangle_phong(const glm::vec3& p1, const glm::vec3& p2, const glm::vec3& p3, glm::vec3 n1, glm::vec3 n2, glm::vec3 n3,const Scene& scene, light& light, glm::vec3 position, int flag);


	
	
private:
	void PutPixel(const int i, const int j, const glm::vec3& color);
	void DrawLine(const glm::ivec2& p1, const glm::ivec2& p2, const glm::vec3& color);

	void CreateBuffers(int w, int h);
	void CreateOpenGLBuffer();
	void InitOpenGLRendering();

	float* color_buffer_;
	int viewport_width_;
	int viewport_height_;
	GLuint gl_screen_tex_;
	GLuint gl_screen_vtc_;
	float maxbufferZ = std::numeric_limits<float>::min(); //
	float minbufferZ = std::numeric_limits<float>::max();; //
	float* z_buffer_;

};
