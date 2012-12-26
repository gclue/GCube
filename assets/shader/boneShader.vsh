
struct light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform mat4 u_mvMatrix;
uniform mat4 u_mvpMatrix;
uniform mat4 u_skinningMatrix[30];
uniform mat3 u_nMatrix;
uniform bool u_useSkinning;
uniform bool u_useLighting;
uniform light u_lightState;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
attribute vec4 a_joints; // j1,w1,j2,w2

varying vec2 v_texcoord;
varying vec3 v_color;

void main()
{
    v_texcoord = a_texcoord;
	
	// bone
	if (u_useSkinning) {
		vec4 p1 = u_skinningMatrix[int(a_joints[0])] * vec4(a_position, 1.0);
		vec4 p2 = u_skinningMatrix[int(a_joints[2])] * vec4(a_position, 1.0);
		gl_Position = u_mvpMatrix * (p1 * a_joints[1] + p2 * a_joints[3]);
	} else {
		gl_Position = u_mvpMatrix * vec4(a_position, 1.0);
	}

    // light
	if (u_useLighting) {
		vec4 pos = u_mvMatrix * vec4(a_position, 1.0);
		vec3 lightDir = normalize(u_lightState.position - vec3(pos));
		vec3 normal = normalize(u_nMatrix * a_normal);
		v_color = vec3(dot(lightDir, normal));
	} else {
		v_color = vec3(1.0);
	}
}
