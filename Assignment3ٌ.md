# Assignment3:

## Renderer::InitOpenGLRendering():
Before this part,We used this function to render the obj, and now we render the obj using OpenGL, 
in this function we prepare the information we need for drawing (the VAO for example),to help us draw we create two triangles that stretch over the screen, The texture is drawn over them.so we used to draw using the color buffer,and The SwapBuffer() function loads color buffer into a texture on the GPU, in other words we the function maps the pixels in the image to the texture coordinates (the triangles), and as we mentions before those triangles build the screen.


## vertex shader:
~~~    				
#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// These outputs will be available in the fragment shader as inputs
out vec3 orig_fragPos;
out vec3 fragPos;
out vec3 fragNormal;
out vec2 fragTexCoords;

void main()
{
	// Apply the model transformation to the 'position' and 'normal' properties of the vertex,
	// so the interpolated values of these properties will be available for usi n the fragment shader
	orig_fragPos = vec3(vec4(pos, 1.0f));
	fragPos = vec3(model * vec4(pos, 1.0f));
	fragNormal = mat3(model) * normal;

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	fragTexCoords = texCoords;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);
}
~~~

##  fragment shade:
~~~
#version 330 core

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant
};

// We set this field's properties from the C++ code
uniform Material material;

// Inputs from vertex shader (after interpolation was applied)
in vec3 fragPos;
in vec3 fragNormal;
in vec2 fragTexCoords;
in vec3 orig_fragPos;
// The final color of the fragment (pixel)
out vec4 frag_color;

void main()
{
	// Sample the texture-map at the UV coordinates given by 'fragTexCoords'
	vec3 textureColor = vec3(texture(material.textureMap, fragTexCoords));

	frag_color = vec4(0.5,0.5,0.5,1);
}
~~~

## render function:
~~~

void Renderer::Render( Scene& scene)
{
	int cameraCount = scene.GetCameraCount();
	if (cameraCount > 0)
	{
		int modelCount = scene.GetModelCount();
		const Camera& camera = scene.GetActiveCamera();

		for (int currentModelIndex = 0; currentModelIndex < modelCount; currentModelIndex++)
		{

			
			std::shared_ptr<MeshModel> currentModel = scene.GetModel(currentModelIndex);

			// Activate the 'colorShader' program (vertex and fragment shaders)
			colorShader.use();

			// Set the uniform variables
			colorShader.setUniform("model", currentModel->Get_transmatrix());
			colorShader.setUniform("view", camera.Get_Lookat() * camera.Get_Invtransmatrix());
			colorShader.setUniform("projection", camera.GetProjectionTransformation());
			colorShader.setUniform("material.textureMap", 0);

			// Set 'texture1' as the active texture at slot #0
			//texture1.bind(0);

			// Drag our model's faces (triangles) in fill mode
			glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);

			// Unset 'texture1' as the active texture at slot #0
			//texture1.unbind(0);

			colorShader.setUniform("color", glm::vec3(0, 0, 0));

			// Drag our model's faces (triangles) in line mode (wireframe)
			glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
			glBindVertexArray(currentModel->GetVAO());
			glDrawArrays(GL_TRIANGLES, 0, currentModel->GetModelVertices().size());
			glBindVertexArray(0);
		}
	}
}
~~~

### Implement Phong shading in the fragment shade:
#### this is an example of parallel light,lighting the car obj:

![parallel_light_white](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/parallel_light_white.png)

#### the car after changing colors:
![parallel_diff_colors1](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/parallel_diff_colors1.png)

#### after transformation:
![1withTransformation](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/1withTransformation.png)

#### point light:
![armadilo](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/armadilo.JPG)

#### example for more than one light:
![beethoven](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/beethoven.JPG)
#### more examples: 
![anotherex](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/anotherex.png)
![horse](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/horse.JPG)

### texture mapping:
#### plane texture:

![teapot](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/teapot.JPG)
#### cylinder texture:
![Cylindrical](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/Cylindrical.png)

##### We can recognize the artifacts that are displayed under the pouring hose of the teapot.

### Toon shading:
##### toon shading with point light on the right of the model and closer than the model:
![toonemapping1](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/toonemapping1.jpeg)

##### toon shading with point light on the edge of the model with 10 different layers of intensity.
![toonemapping2](https://github.com/HaifaGraphicsCourses/computergraphics2021-fidaa/blob/master/toonemapping2.jpeg)