#include "LSystemClass.h"



LSystemClass::LSystemClass()
{
}

LSystemClass::LSystemClass(std::string n_Axiom)
{
	axiom = n_Axiom;
}


LSystemClass::~LSystemClass()
{
}

void LSystemClass::Generate()
{
	temp.clear();

	// For each character in the axiom
	for (unsigned i = 0; i<axiom.length(); ++i)
	{
		// Apply a rule
		if (axiom.at(i) == 'F')
		{
			temp.append("F-F+F+FF-F-F+F");
		}
	}
	axiom.swap(temp);
}
