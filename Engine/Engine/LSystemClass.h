#pragma once

#include <string>

class LSystemClass
{
public:
	LSystemClass();
	LSystemClass(std::string);
	~LSystemClass();

	void Generate(int);
	void Parse(int);
	void ShutDown();

	std::string GetAxiom();

private:
	std::string axiom;
	std::string temp;
};

