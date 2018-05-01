#pragma once

#include <string>
#include "improvednoise.h"

class LSystemClass
{
public:
	LSystemClass();
	LSystemClass(std::string);
	~LSystemClass();

	void Generate(int);
	void ShutDown();

	std::string GetAxiom();

private:
	std::string axiom;
	std::string temp;

	improvednoise* m_ImprovedNoise;
};

