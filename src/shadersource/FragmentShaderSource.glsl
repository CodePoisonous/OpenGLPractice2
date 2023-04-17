// 片段着色器源码
#version 330 core

// 材料属性
struct Material {
	sampler2D	diffuse;
	sampler2D	specular;
	sampler2D	emission;
	float		shininess;
};

// 定向光
struct DirLight {
	vec3 direction;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;
};

// 点光源
struct PointLight {
	vec3 position;

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 衰减参数
	float constant;
	float linear;
	float quadratic;
};

// 聚光
struct SpotLight {
	vec3 position;			// 光源位置
	vec3 direction;			// 光方向
	float cutoffCos;		// 内圆锥切光角的余弦值
	float outerCutOffCos;	// 外圆锥切光角的遇险值

	vec3 ambient;
	vec3 diffuse;
	vec3 specular;

	// 衰减参数
	float constant;
	float linear;
	float quadratic;
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];
uniform DirLight dirlight;
uniform SpotLight spotLight;

uniform vec3 cameraPos;
uniform Material material;

in vec3 FragPos;
in vec3 FragNormal;
in vec2 TexCoords;

out vec4 FragColor;

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);

void main()
{		
	vec3 norm = normalize(FragNormal);				// 片段法线
	vec3 viewDir = normalize(cameraPos - FragPos);	// 视线方向

	// 定向光
	vec3 result = CalcDirLight(dirlight, norm, viewDir);

	// 点光源
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);		

	// 聚合光
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{	
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射
	vec3 lightDir = normalize(-light.direction);
	float diffCos = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.diffuse * diffCos * vec3(texture(material.diffuse, TexCoords));

	// 镜面光照
	vec3 reflectDir = reflect(-lightDir, normal);		// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specCos *  vec3(texture(material.specular, TexCoords));

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射
	vec3 lightDir = normalize(light.position - fragPos);	// 片段指向光源位置的方向
	float diffCos = max(dot(normal, lightDir), 0.0);	// 点积求cos值
	vec3 diffuse = light.diffuse * diffCos * vec3(texture(material.diffuse, TexCoords));

	// 镜面光照
	vec3 reflectDir = reflect(-lightDir, normal);		// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specCos *  vec3(texture(material.specular, TexCoords));

	// 光线衰减系数
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance 
					+ light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	// 环境光
	vec3 ambient = light.ambient * vec3(texture(material.diffuse, TexCoords));

	// 漫反射
	vec3 lightDir = normalize(light.position - FragPos);	// 片段指向光源位置的方向
	float diffCos = max(dot(normal, lightDir), 0.0);	// 点积求cos值
	vec3 diffuse = light.diffuse * diffCos * vec3(texture(material.diffuse, TexCoords));

	// 镜面光照
	vec3 reflectDir = reflect(-lightDir, normal);		// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specCos *  vec3(texture(material.specular, TexCoords));

	// 根据片段位置计算圆锥光照强度值
	float thetaCos = dot(lightDir, normalize(-light.direction));	// 片段与光源方向之间的夹角余弦值
	float epsilon = light.cutoffCos - light.outerCutOffCos;			// 内外圆锥切光角差值
	float interpolation = (thetaCos - light.outerCutOffCos) / epsilon;	// 插值
	float intensity = clamp(interpolation, 0.0, 1.0);				// 将插值约束在0.0到1.0之间

	// 光线衰减系数
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance 
					+ light.quadratic * (distance * distance));
	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}