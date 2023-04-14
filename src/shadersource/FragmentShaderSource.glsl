// 片段着色器源码
#version 330 core

// 材料属性
struct Material {
	sampler2D	diffuse;
	sampler2D	specular;
	float		shininess;
};

// 光源属性
struct Light {
	vec3 position;
	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

uniform vec3 cameraPos;
uniform Material material;
uniform Light light;

in vec3 FragPos;
in vec3 FragNormal;
in vec2 TexCoords;

out vec4 FragColor;

void main()
{
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射
	vec3 norm = normalize(FragNormal);
	vec3 lightDir = normalize(light.position - FragPos);	// 顶点指向光源的方向
	float diffCos = max(dot(norm, lightDir), 0.0);	// 点积求cos值
	vec3 diffuse = light.diffuse * diffCos * vec3(texture(material.diffuse, TexCoords));

	// 镜面光照
	vec3 viewDir = normalize(cameraPos - FragPos);	// 视线方向
	vec3 reflectDir = reflect(-lightDir, norm);		// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specCos * vec3(texture(material.specular, TexCoords));

	// phong模型
	FragColor = vec4(ambient + diffuse + specular, 1);
}