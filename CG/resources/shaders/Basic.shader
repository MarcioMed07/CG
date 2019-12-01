#shader vertex
#version 330 core

layout(location = 0) in vec4 position;
layout(location = 1) in vec2 texC;

out vec2 v_TexC;

uniform mat4 u_MVP;

void main(){
	gl_Position = u_MVP *  position;
	v_TexC = texC;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexC;

uniform sampler2D u_Texture;
uniform sampler2D u_DisplaceMap;
uniform float u_Maximum;
uniform float u_Time;

void main(){
	vec2 displaceMov = vec2(v_TexC.x + u_Time, v_TexC.y + u_Time);
	vec4 displaceTex = texture2D(u_DisplaceMap, displaceMov);
	float displaceConst = displaceTex.b * u_Maximum;
	vec2 textureMov = vec2(v_TexC.x + displaceConst + u_Time, v_TexC.y + displaceConst - u_Time);

	gl_FragColor = texture2D(u_Texture, textureMov);
}

