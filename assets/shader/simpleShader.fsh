precision mediump float;

uniform sampler2D u_diffuseTexture;
uniform float u_bright;
uniform float u_alpha;
varying vec2 v_texcoord;

void main()
{
	gl_FragColor = vec4(texture2D(u_diffuseTexture, v_texcoord.st)) * vec4(vec3(1.0), u_alpha) + vec4(vec3(1.0-u_bright), 0);
}
