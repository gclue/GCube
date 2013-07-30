uniform mat4 u_mvpMatrix;

attribute vec4 a_position;
attribute vec2 a_texcoord;
attribute vec4 a_color;

varying vec2 v_texcoord;
varying vec4 v_color;

void main()
{
	gl_Position = u_mvpMatrix * a_position;
	v_texcoord = a_texcoord;
	v_color = a_color;
}
