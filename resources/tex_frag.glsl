#version 120

varying vec2 vTex;
uniform sampler2D texture0;

void main()
{
	vec3 tex = texture2D(texture0, vTex.st).rgb;
	gl_FragColor = vec4(tex, 1.0);
}
