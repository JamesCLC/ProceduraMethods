#pragma once

#include <string>

class LSystemClass
{
public:
	LSystemClass();
	LSystemClass(std::string);
	~LSystemClass();

	void Generate();
	void ShutDown();

	std::string GetAxiom();


private:
	std::string axiom;
	std::string temp;
};

