#pragma once
#include<glad/glad.h>
#include<sstream>
#include<fstream>
#include<string>
#include<iostream>
class Shader
{
	unsigned int ID;
public:
	Shader(const char* vsPath, const char* fsPath);
	void Use();
	void SetBool(const std::string& name, bool value)const;
	void SetInt(const std::string& name, int value)const;
	void SetFloat1(const std::string& name, float value)const;
	void SetFloat4(const std::string& name, float r, float g, float b, float a);
};

