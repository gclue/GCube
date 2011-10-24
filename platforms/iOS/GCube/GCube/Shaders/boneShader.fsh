precision mediump float;

uniform sampler2D u_diffuseTexture;
varying vec2 v_texcoord;
varying vec3 v_color;

void main()
{
//	gl_FragColor = vec4(texture2D(u_diffuseTexture, v_texcoord.st)) + vec4(v_color, 1.0);
	gl_FragColor = vec4(v_color, 1.0);
}
