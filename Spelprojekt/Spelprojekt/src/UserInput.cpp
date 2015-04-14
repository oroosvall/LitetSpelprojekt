#include "UserInput.h"

using namespace glm;

UserInput::~UserInput()
{
	delete viewMatrix;
}

void UserInput::KeyDown(char c)
{
	switch (c)
	{
	case('W') :
		W = true;
		break;
	case('S') :
		S = true;
		break;
	case('A') :
		A = true;
		break;
	case('D') :
		D = true;
		break;
	}
}

void UserInput::KeyUp(char c)
{
	switch (c)
	{
	case('W') :
		W = false;
		break;
	case('S') :
		S = false;
		break;
	case('A') :
		A = false;
		break;
	case('D') :
		D = false;
		break;
	}
}

void UserInput::Act(float deltaTime)
{
	float speedorig = speed;
	if (shift)
		speed *= 3;
	speed * deltaTime;

	if (W)
	{
		vec3 oldPos = pos;
		//pos += (target * speed);
		vec3 toTarget = (target - pos);
		pos += toTarget * (speed/2);
		target += (pos - oldPos);

	}
	if (S)
	{
		vec3 oldPos = pos;
		//pos -= (target * speed);
		vec3 toTarget = (target - pos);
		pos -= toTarget * (speed/2);
		target += (pos - oldPos);
	}
	if (A)
	{
		vec3 oldPos = pos;
		vec3 toTarget = (target - pos);
		vec3 left = cross(up, toTarget);
		left = normalize(left);
		pos += left*speed;
		target += (pos - oldPos);
	}
	if (D)
	{
		vec3 oldPos = pos;
		vec3 toTarget = (target - pos);
		vec3 left = cross(toTarget, up);
		left = normalize(left);
		pos += left*speed;
		target += (pos - oldPos);
	}
	if (space)
	{
		vec3 oldPos = pos;
		pos.y += speed;
		target += (pos - oldPos);
	}
	if (ctrl)
	{
		vec3 oldPos = pos;
		pos.y -= speed;
		target += (pos - oldPos);
	}

	*viewMatrix = lookAt(pos, target, up);

	speed = speedorig;
}

void UserInput::Shift(bool set)
{
	shift = set;
}

void UserInput::Space(bool set)
{
	space = set;
}

void UserInput::Ctrl(bool set)
{
	ctrl = set;
}

void UserInput::LMB(bool set)
{
	lmb = set;
}

void UserInput::RMB(bool set)
{
	rmb = set;
}

void UserInput::Mouse(float x, float y)
{
	angleH += x / 5.0f;
	angleV += y / 5.0f;
	if (angleV > 89)
		angleV = 89;
	if (angleV < -89)
		angleV = -89;

	//rotate vertically around x
	float rotateRad;
	vec3 view = vec3(1.0f, 0.0f, 0.0f);
	rotateRad = toRadian(angleV);
	rotV = mat3(cos(rotateRad), -sin(rotateRad), 0.0f,
		sin(rotateRad), cos(rotateRad), 0.0f,
		0.0f, 0.0f, 1.0f);
	view = rotV * view;
	view = normalize(view);

	//rotate horizontally around y
	rotateRad = toRadian(angleH);
	rotH = mat3(cos(rotateRad), 0.0f, -sin(rotateRad), 
				0.0f, 1.0f, 0.0f,
				sin(rotateRad), 0.0f, cos(rotateRad));
	view = rotH * view;
	view = normalize(view);

	//set new view pos
	target = pos - view;
}

void UserInput::Init(mat4* mat, vec3 posIn, vec3 targetIn, vec3 upIn)
{
	viewMatrix = mat;
	pos = posIn;
	target = targetIn;
	up = upIn;

	*viewMatrix = lookAt(pos, target, up);

	//Sets the initial view angels 
	vec3 hTarget(target.x, 0.0f, target.z);
	hTarget = normalize(hTarget);
	
	if (hTarget.z >= 0.0f)
	{
		if (hTarget.x > 0.0f)
			angleH = 180 - toDegree(asin(hTarget.z));
		else
			angleH = - toDegree(asin(hTarget.z));
	}
	else
	{
		if (hTarget.x >= 0.0f)
			angleH = toDegree(asin(-hTarget.z));
		else
			angleH = 90.0f + toDegree(asin(-hTarget.z));
	}
	angleV = -toDegree(asin(target.y));
}

float UserInput::toDegree(float val)
{
	return val * 57.2957795f;
}

float UserInput::toRadian(float val)
{
	return val * 0.0174532925f;
}

vec3* UserInput::GetPos()
{
	return &pos;
}

vec3 UserInput::getToTarget()
{
	return (target - pos);
}

bool UserInput::updateMouse()
{
	return shift;
}