// 片段着色器源码
#version 330 core

// 材料属性
struct Material {
	sampler2D	diffuse;
	sampler2D	specular;
	sampler2D	emission;
	float		shininess;
};

// 光源属性
struct Light {
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
	vec3 lightDir = normalize(light.position - FragPos);	// 片段指向光源位置的方向
	//vec3 lightDir = normalize(-light.direction);
	float diffCos = max(dot(norm, lightDir), 0.0);	// 点积求cos值
	vec3 diffuse = light.diffuse * diffCos * vec3(texture(material.diffuse, TexCoords));

	// 镜面光照
	vec3 viewDir = normalize(cameraPos - FragPos);	// 视线方向
	vec3 reflectDir = reflect(-lightDir, norm);		// 反射光方向
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.specular * specCos *  vec3(texture(material.specular, TexCoords));

	// 发光值
	//vec3 emit = vec3(texture(material.emission, TexCoords));

	// 根据片段位置计算圆锥光照强度值
	float thetaCos = dot(lightDir, normalize(-light.direction));	// 片段与光源方向之间的夹角余弦值
	float epsilon = light.cutoffCos - light.outerCutOffCos;			// 内外圆锥切光角差值
	float interpolation = (thetaCos - light.outerCutOffCos) / epsilon;	// 插值
	float intensity = clamp(interpolation, 0.0, 1.0);				// 将插值约束在0.0到1.0之间
	diffuse *= intensity;
	specular *= intensity;

	// 光线衰减系数
	float distance = length(light.position - FragPos);
	float attenuation = 1.0 / (light.constant + light.linear * distance 
					+ light.quadratic * (distance * distance));
	ambient *= attenuation;
	diffuse *= attenuation;
	specular *= attenuation;

	// phong模型
	FragColor = vec4(ambient + diffuse + specular/* + emit*/, 1.0);
}