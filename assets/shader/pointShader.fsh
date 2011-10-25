precision mediump float;

uniform sampler2D u_diffuseTexture;
uniform float u_bright;
uniform float u_alpha;

void main() {
	vec4 baseColor = texture2D(u_diffuseTexture, gl_PointCoord);
	gl_FragColor = baseColor * vec4(vec3(1.0), u_alpha) + vec4(vec3(1.0-u_bright), 0);
}
