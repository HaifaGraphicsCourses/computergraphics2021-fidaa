#version 330 core

layout(location = 0) in vec3 pos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 texCoords;

// The model/view/projection matrices
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform bool draw;
uniform mat4 light_trans;
uniform int light_type;

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
	gl_PointSize =12.f;

	// Pass the vertex texture coordinates property as it is. Its interpolated value
	// will be avilable for us in the fragment shader
	fragTexCoords = texCoords;

	//float r=sqrt(pow(orig_fragPos.x,2)+pow(orig_fragPos.y,2)+pow(orig_fragPos.z,2));
    //fragTexCoords.x =atan((sqrt(pow(orig_fragPos.x,2)+pow(orig_fragPos.y,2)))/orig_fragPos.z);			
    //fragTexCoords.y = atan(orig_fragPos.y/orig_fragPos.x);

	float theta =(atan(orig_fragPos.z,orig_fragPos.x)) + 3.14; 
	fragTexCoords.x=theta;
	fragTexCoords.y=orig_fragPos.y;

	// This is an internal OpenGL variable, we must set a value to this variable
	gl_Position = projection * view *  model * vec4(pos, 1.0f);

	if(draw ==  true)
	{
	if(light_type == 1) //parallel
		gl_Position = projection * view * vec4(pos,1.f);
	else
		gl_Position = projection * view *  light_trans * vec4(pos, 1.0f);


	}
	
}