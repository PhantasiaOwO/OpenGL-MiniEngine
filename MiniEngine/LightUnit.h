#pragma once
#include <gl/glut.h>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>


enum ELightType
{
	DirectionalLight,
	PointLight,
	SpotLight,
};

class LightUnit
{
private:
	friend class LightManager;
	LightUnit(int index);
	LightUnit(int index, glm::vec4 position);
	GLenum _lightId;				// 灯光序号

	// w分量为0时(默认)为平行光无衰减，为1时则为点光源有衰减
	glm::vec4 _position;			// 灯光坐标
	
	glm::vec4 _ambientColor;		// 环境光 (尽量只为0号灯光设置环境光)
	glm::vec4 _diffuseColor;		// 漫射光颜色
	glm::vec4 _specularColor;		// 反射光颜色

	// 聚光灯设置
	glm::vec3 _spotDirection;		// 发射角度
	GLfloat _spotExponent;			// 聚光强度 [0,128] 越大灯光越聚焦
	GLfloat _spotCutOff;			// 聚光角度 [0,90] 母线和中轴线切角 
	glm::vec3 _attenuation;			// 衰减因子，x,y,z 分别对应 常数衰减因子, 距离衰减因子, 二次函数衰减因子 默认为(0,0,1)

	bool _isEnabled = false;
public:
	void SetEnabled(bool isEnabled); 
	void SetPosition(glm::vec4 position);
	void SetAmbientColor(glm::vec4 ambientColor);
	void SetDiffuseColor(glm::vec4 diffuseColor);
	void SetSpecularColor(glm::vec4 specularColor);
	void SetSpotDirection(glm::vec3 spotDirection);
	bool SetSpotExponent(GLfloat spotExponent);
	bool SetSpotCutOff(GLfloat spotCutOff);
	bool SetAttenuationFactors(glm::vec3 attenuationFactors);
	bool SetAttenuationFactors(GLfloat constantFactor, GLfloat linearFactor, GLfloat quadraticFactor);

	bool IsEnabled() const { return _isEnabled; }
	glm::vec4 GetPosition() const { return _position; }
	glm::vec4 GetAmbientColor() const { return _ambientColor; }
	glm::vec4 GetDiffuseColor() const { return _diffuseColor; }
	glm::vec4 GetSpecularColor() const { return _specularColor; }
	glm::vec3 GetSpotDirection() const { return _spotDirection; }
	GLfloat GetSpotExponent() const { return _spotExponent; }
	GLfloat GetSpotCutOff() const {return _spotCutOff;}
	glm::vec3 GetAttenuationFactors() const { return _attenuation;}
	
	ELightType GetLightType() const;
};

