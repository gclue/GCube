precision mediump float;

uniform sampler2D u_diffuseTexture;
varying vec4 v_color;
varying vec2 v_texcoord;

void main()
{
//	gl_FragColor = vec4(texture2D(u_diffuseTexture, v_texcoord.st)) * v_color;
//	gl_FragColor = vec4(v_color.rgb, v_color.a * texture2D(u_diffuseTexture, v_texcoord.st).a);
//	gl_FragColor = vec4(1,0,0,1);
//	vec4 color = texture2D(u_diffuseTexture, v_texcoord.st);
//	gl_FragColor = vec4(v_color.rgb * color.a, v_color.a * color.a);
	gl_FragColor = v_color * texture2D(u_diffuseTexture, v_texcoord.st).a;
}

