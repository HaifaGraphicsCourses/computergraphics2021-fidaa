#version 330 core

struct Material
{
	sampler2D textureMap;
	// You can add more fields here...
	// Such as:
	//		1. diffuse/specular relections constants
	//		2. specular alpha constant
	//		3. Anything else you find relevant

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
	float alpha;

};

// We set this field's properties from the C++ code
uniform Material material;
uniform vec3 eye;
uniform vec3 ambient_l[5];
uniform vec3 diffuse_l[5];
uniform vec3 specular_l[5];
uniform vec3 p_l[5];
uniform vec3 d_l[5];
uniform mat4 t_l[5];
uniform int type_l[5];
uniform int c;
uniform bool drawlight;




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
	 vec3 ambient_c = vec3(0.f,0.f,0.f) , diffuse_c= vec3(0.f,0.f,0.f), specular_c= vec3(0.f,0.f,0.f);
	 vec3 def_color=vec3(0.f,0.f,0.f),spec_color=vec3(0.f,0.f,0.f);
	if(drawlight==true)
		frag_color =vec4(1.f,1.f,1.f,1.f);
	else
	{
		for(int i=0; i<c;i++)
		{	
			vec3 I;
			if(type_l[i]==0) // parallel
			{
				I=normalize(d_l[i]);
			}
			else
			{
				vec3 position=vec3(t_l[i]*vec4(p_l[i],1)); 
				I=normalize(fragPos - position);
			}
			ambient_c+=vec3(ambient_l[i].x*material.ambient.x,ambient_l[i].y*material.ambient.y,ambient_l[i].z*material.ambient.z);

			diffuse_c=vec3(diffuse_l[i].x*material.diffuse.x,diffuse_l[i].y*material.diffuse.y,diffuse_l[i].z*material.diffuse.z);
			float i_dot=dot(-fragNormal,I);
			def_color+=diffuse_c*i_dot;

			specular_c=vec3(specular_l[i].x*material.specular.x,specular_l[i].y*material.specular.y,specular_l[i].z*material.specular.z);
			vec3 r=(2.f * dot(-fragNormal,I)*fragNormal -I);
			float power=pow(max(0.f,dot(r,normalize(eye))),material.alpha);
			spec_color=specular_c*power;
		}
		if(c > 0)
			frag_color=vec4(ambient_c+def_color+spec_color,1.f);
		else	
			frag_color=vec4(material.ambient,1.f);
	}
}
