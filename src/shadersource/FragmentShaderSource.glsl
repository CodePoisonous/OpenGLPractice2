// 片段着色器源码
#version 330 core

uniform vec3 lightPos;
uniform vec3 cameraPos;
uniform vec3 objectColor;
uniform vec3 lightColor;

in vec3 FragPos;
in vec3 FragNormal;

out vec4 FragColor;

void main()
{
	// 环境光
	float ambientStrength  = 0.1;
	vec3 ambient = ambientStrength  * lightColor;

	// 漫反射
	vec3 norm = normalize(FragNormal);
	vec3 lightDir = normalize(lightPos - FragPos);	// 顶点指向光源的方向
	float diffCos = max(dot(norm, lightDir), 0.0);	// 点积求cos值
	vec3 diffuse = diffCos * lightColor;

	// 镜面光照
	vec3 viewDir = normalize(cameraPos - FragPos);	// 视线方向
	vec3 reflectDir = reflect(-lightDir, norm);		// 反射光方向
	int Shininess = 32;								// 反光度
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), Shininess);
	float specularStrength = 0.5;					// 镜面强度
	vec3 specular = specularStrength * specCos * lightColor;

	// phong模型
	vec3 result = (ambient + diffuse + specular) * objectColor;
	FragColor = vec4(result, 1);
}