// Ƭ����ɫ��Դ��
#version 330 core

// ��������
struct Material {
	sampler2D	diffuse1;	// ������
	sampler2D	specular1;	// ���淴��
	sampler2D	emission;	// �Է���
	float		shininess;	// �����
};

// ���ջ�������
struct LightBase{
	vec3 ambient;			// ������
	vec3 diffuse;			// ������
	vec3 specular;			// ���淴��
};

// �����
struct DirLight {
	bool is_set;			// �Ƿ����øù�Դ	
	vec3 direction;			// ����
	LightBase lb;			// ��������
};

// ˥����ʽ��ϵ��
// ��ʽ��1 / (constant + linear * distance + quadratic * distance^2)
struct Attenuation	{
	float constant;			// ����
	float linear;			// һ���ݵ�ϵ��
	float quadratic;		// �����ݵ�ϵ��
};

// ���Դ
struct PointLight {
	bool is_set;			// �Ƿ����øù�Դ	
	vec3 position;			// λ��
	LightBase lb;			// ��������
	Attenuation at;			// ˥������
};

// �۹�
struct SpotLight {
	bool is_set;			// �Ƿ����øù�Դ
	vec3 position;			// ��Դλ��
	vec3 direction;			// �ⷽ��
	float cutOffCos;		// ��Բ׶�й�ǵ�����ֵ
	float outerCutOffCos;	// ��Բ׶�й�ǵ�����ֵ
	LightBase lb;			// ��������
	Attenuation at;			// ˥������
};

#define NR_POINT_LIGHTS 4
uniform PointLight pointLights[NR_POINT_LIGHTS];	// ���Դ����
uniform DirLight dirLight;							// ��������
uniform SpotLight spotLight;						// �۹����

uniform vec3 cameraPos;		// ���λ��
uniform Material material;	// ���϶���

in vec3 FragPos;			// Ƭ������
in vec3 FragNormal;			// ��������
in vec2 TexCoords;			// ��������

out vec4 FragColor;			// Ƭ�����յ���ɫ

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
vec3 CalcSpotLight(SpotLight light, vec3 normal, vec3 fragPos, vec3 viewDir);
float calcAttenuation(Attenuation at, float distance);

void main()
{		
	vec3 norm = normalize(FragNormal);				// Ƭ�η���
	vec3 viewDir = normalize(cameraPos - FragPos);	// ���߷���

	// �����
	vec3 result = CalcDirLight(dirLight, norm, viewDir);

	// ���Դ
	for(int i = 0; i < NR_POINT_LIGHTS; ++i)
		result += CalcPointLight(pointLights[i], norm, FragPos, viewDir);		

	// �ۺϹ�
	result += CalcSpotLight(spotLight, norm, FragPos, viewDir);

	FragColor = vec4(result, 1.0);
}

vec3 CalcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{	
	if(!light.is_set) return vec3(0.0f);

	// ������
	vec3 ambient = light.lb.ambient * vec3(texture(material.diffuse1, TexCoords));

	// ������
	vec3 lightDir = normalize(-light.direction);
	float diffCos = max(dot(normal, lightDir), 0.0);
	vec3 diffuse = light.lb.diffuse * diffCos * vec3(texture(material.diffuse1, TexCoords));

	// �������
	vec3 reflectDir = reflect(-lightDir, normal);		// ����ⷽ��
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.lb.specular * specCos *  vec3(texture(material.specular1, TexCoords));

	return ambient + diffuse + specular;
}

vec3 CalcPointLight(PointLight light, vec3 normal, vec3 fragPos, vec3 viewDir)
{
	if(!light.is_set) return vec3(0.0f);

	// ������
	vec3 ambient = light.lb.ambient * vec3(texture(material.diffuse1, TexCoords));

	// ������
	vec3 lightDir = normalize(light.position - fragPos);	// Ƭ��ָ���Դλ�õķ���
	float diffCos = max(dot(normal, lightDir), 0.0);		// �����cosֵ
	vec3 diffuse = light.lb.diffuse * diffCos * vec3(texture(material.diffuse1, TexCoords));

	// �������
	vec3 reflectDir = reflect(-lightDir, normal);			// ����ⷽ��
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.lb.specular * specCos *  vec3(texture(material.specular1, TexCoords));

	// ����˥��ϵ��
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

	// ������
	vec3 ambient = light.lb.ambient * vec3(texture(material.diffuse1, TexCoords));

	// ������
	vec3 lightDir = normalize(light.position - FragPos);	// Ƭ��ָ���Դλ�õķ���
	float diffCos = max(dot(normal, lightDir), 0.0);		// �����cosֵ
	vec3 diffuse = light.lb.diffuse * diffCos * vec3(texture(material.diffuse1, TexCoords));

	// �������
	vec3 reflectDir = reflect(-lightDir, normal);			// ����ⷽ��
	float specCos = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);
	vec3 specular = light.lb.specular * specCos *  vec3(texture(material.specular1, TexCoords));

	// ����Ƭ��λ�ü���Բ׶����ǿ��ֵ
	float thetaCos = dot(lightDir, normalize(-light.direction));	// Ƭ�����Դ����֮��ļн�����ֵ
	float epsilon = light.cutOffCos - light.outerCutOffCos;			// ����Բ׶�й�ǲ�ֵ
	float interpolation = (thetaCos - light.outerCutOffCos) / epsilon;	// ��ֵ
	float intensity = clamp(interpolation, 0.0, 1.0);				// ����ֵԼ����0.0��1.0֮��

	// ����˥��ϵ��
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
