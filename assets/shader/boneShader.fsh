precision mediump float;

uniform sampler2D u_diffuseTexture;
uniform float u_alpha;
uniform float u_useTexture;
varying vec2 v_texcoord;
varying vec3 v_color;

void main()
{
	gl_FragColor = (vec4(texture2D(u_diffuseTexture, v_texcoord.st)) + vec4(u_useTexture)) * vec4(v_color, 1.0) * vec4(vec3(1.0), u_alpha);
}
