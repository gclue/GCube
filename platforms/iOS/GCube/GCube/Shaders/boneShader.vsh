uniform mat4 u_mvMatrix;
uniform mat4 u_mvpMatrix;
uniform mat4 u_skinningMatrix[10];
uniform mat3 u_nMatrix;

attribute vec3 a_position;
attribute vec3 a_normal;
attribute vec2 a_texcoord;
attribute float a_joint_1;
attribute float a_joint_2;
attribute float a_weight_1;
attribute float a_weight_2;

varying vec2 v_texcoord;
varying vec3 v_color;

const vec3 lightPos = vec3(0.0, 4.0, 5.0);

void main()
{
	// bone
    v_texcoord = a_texcoord;
    vec4 p1 = u_skinningMatrix[int(a_joint_1)] * vec4(a_position, 1.0);
    vec4 p2 = u_skinningMatrix[int(a_joint_2)] * vec4(a_position, 1.0);
    gl_Position = u_mvpMatrix * (p1 * a_weight_1 + p2 * a_weight_2);

    // light
    vec4 pos = u_mvMatrix * vec4(a_position, 1.0);
    vec3 lightDir = normalize(lightPos - vec3(pos));
    vec3 normal = normalize(u_nMatrix * a_normal);
    v_color = vec3(dot(lightDir, normal));
}
