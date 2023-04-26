// 片段着色器源码
#version 330 core

// 材料属性
struct Material {
	sampler2D	diffuse1;	// 漫反射
	sampler2D	specular1;	// 镜面反射
	sampler2D	emission;	// 自发光
	float		shininess;	// 光泽度
};

// 光照基础属性
struct LightBase{
	vec3 ambient;			// 环境光
	vec3 diffuse;			// 漫反射
	vec3 specular;			// 镜面反射
};

// 定向光
struct DirLight {
	bool is_set;			// 是否设置该光源	
	vec3 direction;			// 方向
	LightBase lb;			// 基础属性
};

// 衰减公式的系数
// 公式：1 / (constant + linear * distance + quadratic * distance^2)
struct Attenuation	{
	float constant;			// 常数
	float linear;			// 一次幂的系数
	float quadratic;		// 二次幂的系数
};

// 点光源
struct PointLight {
	bool is_set;			// 是否设置该光源	
	vec3 position;			// 位置
	LightBase lb;			// 基础属性
	Attenuation at;			// 衰减参数
};

// 聚光
struct SpotLight {
	bool is_set;			// 是否设置该光源
	vec3 position;			// 光源位置
	vec3 direction;			// 光方向
	float cutOffCos;		// 内圆锥切光角的余弦值
	float outerCutOffCos;	// 外圆锥切光角的余弦值
	LightBase lb;			// 基础属性
	Attenuation at;			// 衰减参数
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];	// 点光源对象
uniform DirLight dirLight;							// 定向光对象
uniform SpotLight spotLight;						// 聚光对象

uniform vec3 cameraPos;		// 相机位置
uniform Material material;	// 材料对象

in vec3 FragPos;			// 片段坐标
in vec3 FragNormal;			// 法线向量
in vec2 TexCoords;			// 纹理坐标

out vec4 FragColor;			// 片段最终的颜色

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float calcAttenuation(Attenuation at, float distance);

void main()
{		
	vec3 norm = normalize(FragNormal);				// 片段法线
	vec3 viewDir = normalize(cameraPos - FragPos);	// 视线方向

	// 定向光
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	// 点光源
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);		

	// 聚合光
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{	
	if(!light.is_set) return vec3(0.0f);

	// 环境光
	vec3 ambient = light.lb.ambient * vec3(texture(material.diffuse1, TexCoords));

	// 漫反射
	vec3 lightDir = normalize(-light.direction);
	float diffCos = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.lb.diffuse * diffCos * vec3(texture(material.diffuse1, TexCoords));

	// 镜面光照
	vec3 reflectDir = reflect(-lightDir, normal);		// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.lb.specular * specCos *  vec3(texture(material.specular1, TexCoords));

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	if(!light.is_set) return vec3(0.0f);

	// 环境光
	vec3 ambient = light.lb.ambient * vec3(texture(material.diffuse1, TexCoords));

	// 漫反射
	vec3 lightDir = normalize(light.position - fragPos);	// 片段指向光源位置的方向
	float diffCos = max(dot(normal, lightDir), 0.0);		// 点积求cos值
	vec3 diffuse = light.lb.diffuse * diffCos * vec3(texture(material.diffuse1, TexCoords));

	// 镜面光照
	vec3 reflectDir = reflect(-lightDir, normal);			// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.lb.specular * specCos *  vec3(texture(material.specular1, TexCoords));

	// 光线衰减系数
	float distance = length(light.position - FragPos);
	float attenuation = calcAttenuation(light.at, distance);

	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	return ambient + diffuse + specular;
}

vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	if(!light.is_set) return vec3(0.0f);

	// 环境光
	vec3 ambient = light.lb.ambient * vec3(texture(material.diffuse1, TexCoords));

	// 漫反射
	vec3 lightDir = normalize(light.position - FragPos);	// 片段指向光源位置的方向
	float diffCos = max(dot(normal, lightDir), 0.0);		// 点积求cos值
	vec3 diffuse = light.lb.diffuse * diffCos * vec3(texture(material.diffuse1, TexCoords));

	// 镜面光照
	vec3 reflectDir = reflect(-lightDir, normal);			// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.lb.specular * specCos *  vec3(texture(material.specular1, TexCoords));

	// 根据片段位置计算圆锥光照强度值
	float thetaCos = dot(lightDir, normalize(-light.direction));	// 片段与光源方向之间的夹角余弦值
	float epsilon = light.cutOffCos - light.outerCutOffCos;			// 内外圆锥切光角差值
	float interpolation = (thetaCos - light.outerCutOffCos) / epsilon;	// 插值
	float intensity = clamp(interpolation, 0.0, 1.0);				// 将插值约束在0.0到1.0之间

	// 光线衰减系数
	float distance = length(light.position - FragPos);
	float attenuation = calcAttenuation(light.at, distance);

	ambient *= attenuation * intensity;
	diffuse *= attenuation * intensity;
	specular *= attenuation * intensity;

	return ambient + diffuse + specular;
}

float calcAttenuation(Attenuation at, float distance)
{
	return 1.0 / (at.constant + at.linear * distance + at.quadratic * (distance * distance));
}
