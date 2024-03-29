#type vertex
#version 330 core

layout (location = 0) in vec3 a_Position;
layout (location = 1) in vec4 a_Color;

out vec3 v_Position;
out vec4 v_Color;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

void main() {
	v_Position = a_Position;
	v_Color = a_Color;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1);
}

#type pixel
#version 330 core

layout (location = 0) out vec4 o_Color;

in vec3 v_Position;
in vec4 v_Color;

void main() {
	//color = vec4(v_Position * 0.5 + 0.5, 1.0);
	o_Color = v_Color*1.5;
}