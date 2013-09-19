
struct light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform mat4 u_mvMatrix;
uniform mat4 u_mvpMatrix;
uniform mat4 u_skinningMatrix[50];
uniform mat3 u_nMatrix;
uniform bool u_useSkinning;
uniform bool u_useLighting;
uniform light u_lightState;
uniform bool u_edge;
uniform float u_edgeSize;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
attribute vec4 a_joints; // j1,w1,j2,w2

varying vec2 v_texcoord;
varying vec3 v_color;

void main()
{
	vec3 pos = a_position;
	if (u_edge) {
		pos += a_normal * u_edgeSize;
	}
	
    v_texcoord = a_texcoord;
	
	// bone
	mat4 skmtx = mat4(1);
	if (u_useSkinning && a_joints[0] < 65535.0) {
		mat4 m1 = u_skinningMatrix[int(a_joints[0])] * a_joints[1];
		mat4 m2 = u_skinningMatrix[int(a_joints[2])] * a_joints[3];
		skmtx = m1 + m2;
		vec4 p1 = m1 * vec4(pos, 1.0);
		vec4 p2 = m2 * vec4(pos, 1.0);
		gl_Position = u_mvpMatrix * (p1 + p2);
	} else {
		gl_Position = u_mvpMatrix * vec4(pos, 1.0);
	}

    // light
	if (u_useLighting) {
		vec4 pos = u_mvMatrix * skmtx * vec4(a_position, 1.0);
		vec3 lightDir = normalize(u_lightState.position - vec3(pos));
		vec3 normal = normalize(u_nMatrix * mat3(skmtx) * a_normal);
		v_color = vec3(dot(lightDir, normal));
	} else {
		v_color = vec3(1.0);
	}
}
