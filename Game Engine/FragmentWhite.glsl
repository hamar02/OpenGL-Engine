#version 440

in vec4 FragPos;  
in vec3 NormalF;
in vec2 uv_coordF;


out vec4 fragment_color;

uniform sampler2D ourTexture;
uniform vec3 lightPos;  

void main () {
	vec3 norm = normalize(NormalF);
	vec3 lightDir = normalize(lightPos - FragPos.xyz); 
	float diff = max(dot(norm, lightDir), 0.0);

	float ambientStrength = 1;
    vec3 ambient = ambientStrength * vec3(0.1,0.1,0.1);

	vec3 diffuseLight = diff * vec3(0.8,0.8,0.8);

    vec3 lightResult = ambient + diffuseLight;




	//fragment_color =  vec4(lightResult,1)*texture(ourTexture, uv_coordF);
	fragment_color = vec4(1,1,1, 1);

}