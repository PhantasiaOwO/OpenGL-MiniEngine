#include "LightUnit.h"

LightUnit::LightUnit(int index):
	_lightId(index)
{
}

void LightUnit::SetPosition(glm::vec4 position)
{
	_position = position;
	const GLfloat pos[] = {_position.x,_position.y,_position.z,_position.w}; 
	glLightfv(_lightId, GL_POSITION, pos);
}

void LightUnit::SetAmbientColor(glm::vec4 ambientColor)
{
	_ambientColor = ambientColor;
	const GLfloat ambient[] = {_ambientColor.x,_ambientColor.y,_ambientColor.z,_ambientColor.w}; 
	glLightfv(_lightId, GL_AMBIENT, ambient);
}

void LightUnit::SetDiffuseColor(glm::vec4 diffuseColor)
{
	_diffuseColor = diffuseColor;
	const GLfloat diffuse[] = {_diffuseColor.x,_diffuseColor.y,_diffuseColor.z,_diffuseColor.w}; 
	glLightfv(_lightId, GL_DIFFUSE, diffuse);
}

void LightUnit::SetSpecularColor(glm::vec4 specularColor)
{
	_specularColor = specularColor;
	const GLfloat specular[] = {_specularColor.x,_specularColor.y,_specularColor.z,_specularColor.w}; 
	glLightfv(_lightId, GL_SPECULAR, specular);
}

void LightUnit::SetSpotDirection(glm::vec3 spotDirection)
{
	_spotDirection = spotDirection;
	const GLfloat Dir[] = {_spotDirection.x, _spotDirection.y, _spotDirection.z};
	glLightfv(_lightId, GL_SPOT_DIRECTION, Dir);
}

void LightUnit::SetSpotExponent(GLfloat spotExponent)
{
	_spotExponent = spotExponent;
	glLightf(_lightId, GL_SPOT_EXPONENT, _spotExponent);
}

void LightUnit::SetSpotCutOff(GLfloat spotCutOff)
{
	_spotCutOff = spotCutOff;
	glLightf(_lightId, GL_SPOT_CUTOFF, _spotCutOff);
}

void LightUnit::SetAttenuationFactors(glm::vec3 attenuationFactors)
{
	_attenuation = attenuationFactors;
	const GLfloat factors[] = {attenuationFactors.x, attenuationFactors.y, attenuationFactors.z};
	glLightfv(GL_LIGHT0,GL_LINEAR_ATTENUATION, factors);
}

void LightUnit::SetAttenuationFactors(GLfloat constantFactor, GLfloat linearFactor, GLfloat quadraticFactor)
{
	_attenuation = glm::vec3(constantFactor, linearFactor, quadraticFactor);
	const GLfloat factors[] = {constantFactor,  linearFactor, quadraticFactor};
	glLightfv(GL_LIGHT0,GL_LINEAR_ATTENUATION, factors);
}

ELightType LightUnit::GetLightType() const
{
	if(_spotExponent != 0) return ELightType::SpotLight;
	if(_position.w == 0) return ELightType::PointLight;
	return ELightType::DirectionalLight;
}
