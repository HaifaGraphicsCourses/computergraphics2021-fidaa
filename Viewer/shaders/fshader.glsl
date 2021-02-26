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
uniform float type_l[5];
uniform int c;
uniform bool drawlight;
uniform int ts;

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
	 vec3 def_color,spec_color;
	 float in2;
	 vec3 diffuse2;
	if(drawlight==true)
		frag_color =vec4(0.f,0.f,0.f,1.f);
	else
	{
		for(int i=0; i<c;i++)
		{	
			vec3 I;
			if(type_l[i]==1.f) // parallel
				I=normalize(d_l[i]);
			else if(type_l[i]==2.f)
			{
				vec3 position=vec3(t_l[i]*vec4(p_l[i],1)); 
				I=normalize(fragPos - position);
			}
			in2=max(0.0, dot(I,-fragNormal));

			ambient_c+=vec3(ambient_l[i].x*material.ambient.x,ambient_l[i].y*material.ambient.y,ambient_l[i].z*material.ambient.z);

			def_color=vec3(diffuse_l[i].x*material.diffuse.x,diffuse_l[i].y*material.diffuse.y,diffuse_l[i].z*material.diffuse.z);
			float i_dot=dot(-fragNormal,I);
			diffuse_c+=def_color*i_dot;

			spec_color=vec3(specular_l[i].x*material.specular.x,specular_l[i].y*material.specular.y,specular_l[i].z*material.specular.z);
			vec3 r=(2.f * dot(-fragNormal,I)*fragNormal -I);
			float power=pow(max(0.f,dot(r,normalize(eye))),material.alpha);
			specular_c+=spec_color*power;
		}
			// Phong shading
			if(ts==2)
				frag_color=vec4(ambient_c+diffuse_c+specular_c,1.f);

			//Toon shading
			if(ts==1)
			{
				if (in2 > 0.95)      diffuse2 = vec3(1.0, 1.0, 1.0);
				else if (in2 > 0.85) diffuse2 = vec3(0.9, 0.9, 0.9);
				else if (in2 > 0.75) diffuse2 = vec3(0.8, 0.8, 0.8);
				else if (in2 > 0.65) diffuse2 = vec3(0.7, 0.7, 0.7);
				else if (in2 > 0.55) diffuse2 = vec3(0.6, 0.6, 0.6);
				else if (in2 > 0.45) diffuse2 = vec3(0.5, 0.5, 0.5);
				else if (in2 > 0.35) diffuse2 = vec3(0.4, 0.4, 0.4);
				else if (in2 > 0.25) diffuse2 = vec3(0.3, 0.3, 0.3);
				else if (in2 > 0.15) diffuse2 = vec3(0.2, 0.2, 0.2);
				else                       diffuse2 = vec3(0.1, 0.1, 0.1);
				frag_color=vec4(diffuse2*(ambient_c+diffuse_c+specular_c),1.f);
			}

			//texture model
			if(ts==0)
				frag_color=vec4(textureColor,1.f);

			//ambient color (without light source)
			if(c==0)
				frag_color=vec4(material.ambient,1.f);
	}

}
