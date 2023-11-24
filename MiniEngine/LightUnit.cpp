#include "LightUnit.h"

LightUnit::LightUnit(int index) :
	_lightId(index)
{
	constexpr glm::vec4 defaultZero{0,0,0,1};
	
	SetPosition(defaultZero);
	SetAmbientColor(defaultZero);
	SetDiffuseColor(defaultZero);
	SetSpecularColor(defaultZero);
	
	SetSpotDirection({0,0,1});
	SetSpotExponent(0);
	SetSpotCutOff(180);
	SetAttenuationFactors(1,0,0);
	
	SetEnabled(false);
}

LightUnit::LightUnit(int index, glm::vec4 position) : LightUnit(index)
{
	SetPosition(position);
}

void LightUnit::SetEnabled(bool isEnabled)
{
	_isEnabled = isEnabled;
	if (isEnabled)
	{
		glEnable(_lightId);
	}
	else
	{
		glDisable(_lightId);
	}
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

bool LightUnit::SetSpotExponent(GLfloat spotExponent)
{
	if(spotExponent < 0 || spotExponent > 128) return false;
	_spotExponent = spotExponent;
	glLightf(_lightId, GL_SPOT_EXPONENT, _spotExponent);
	return true;
}

bool LightUnit::SetSpotCutOff(GLfloat spotCutOff)
{
	if((spotCutOff < 0 || spotCutOff > 90) && spotCutOff != 180) return false;
	_spotCutOff = spotCutOff;
	glLightf(_lightId, GL_SPOT_CUTOFF, _spotCutOff);
}

bool LightUnit::SetAttenuationFactors(glm::vec3 attenuationFactors)
{
	if(attenuationFactors.x < 0 || attenuationFactors.x > 1 ||
		attenuationFactors.y < 0 || attenuationFactors.y > 1 ||
		attenuationFactors.z < 0 || attenuationFactors.z > 1)
			return false;
	_attenuation = attenuationFactors;
	const GLfloat factors[] = {attenuationFactors.x, attenuationFactors.y, attenuationFactors.z};
	glLightfv(GL_LIGHT0,GL_LINEAR_ATTENUATION, factors);
	return true;
}

bool LightUnit::SetAttenuationFactors(GLfloat constantFactor, GLfloat linearFactor, GLfloat quadraticFactor)
{
	if(constantFactor < 0 || constantFactor > 1 ||
		linearFactor < 0 || linearFactor > 1 ||
		quadraticFactor < 0 || quadraticFactor > 1)
			return false;
	_attenuation = glm::vec3(constantFactor, linearFactor, quadraticFactor);
	const GLfloat factors[] = {constantFactor,  linearFactor, quadraticFactor};
	glLightfv(GL_LIGHT0,GL_LINEAR_ATTENUATION, factors);
	return true;
}

ELightType LightUnit::GetLightType() const
{
	if(_spotExponent != 0) return ELightType::SpotLight;
	if(_position.w == 0) return ELightType::PointLight;
	return ELightType::DirectionalLight;
}
