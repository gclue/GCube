precision mediump float;

uniform sampler2D u_diffuseTexture;
uniform sampler2D u_diffuseTexture_mlt;
uniform float u_alpha;
uniform float u_useTexture;
uniform float u_useTexture_mlt;
uniform vec4 u_edgeColor;

varying vec2 v_texcoord;
varying vec2 v_texcoord_mlt;
varying vec3 v_color;

void main()
{
	if (u_edgeColor.a > 0.0) {
		gl_FragColor = u_edgeColor;
	} else {
		vec4 color = vec4(texture2D(u_diffuseTexture, v_texcoord.st)) + vec4(u_useTexture);
		color *= (vec4(texture2D(u_diffuseTexture_mlt, v_texcoord_mlt.st)) + vec4(u_useTexture_mlt));
		gl_FragColor = color * vec4(v_color, 1.0) * vec4(vec3(1.0), u_alpha);
	}
}
