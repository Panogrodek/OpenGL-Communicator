#type vertex
#version 450
layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec4 a_Color;
layout(location = 2) in vec2 a_TexCoords;

out vec3 v_FragPos;
out vec4 v_Color;	

void main()
{
	v_FragPos = a_Position;
	v_Color = a_Color;
	gl_Position = vec4(a_Position,1.0);
}

#type fragment
#version 450
layout(location = 0) out vec4 color;

in vec3 v_FragPos;
in vec4 v_Color;

void main(){
	color = v_Color;
}