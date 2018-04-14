#include "LSystemClass.h"



LSystemClass::LSystemClass()
{
	// Default Axiom starter value.
	axiom = "F-F-F-F";

	//axiom = "F[+F][-F[-F]F]F[+F][-F]";
}

LSystemClass::LSystemClass(std::string n_Axiom)
{
	// Allow the Axiom to be specified when the class object is created.
	axiom = n_Axiom;
}


LSystemClass::~LSystemClass()
{
}

void LSystemClass::Generate(int itterations)
{
	for (int i = 0; i < itterations; i++)
	{
		temp.clear();
		// For each character in the axiom
		for (unsigned i = 0; i<axiom.length(); ++i)
		{
			// Apply a rule
			if (axiom.at(i) == 'F')
			{
				//temp.append("F[+F][-F[-F]F]F[+F][-F]");

				temp.append("F[-FF][+FF][FF");

				//temp.append("F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F");

				// Example L-Systems from [http://www.inf.ed.ac.uk/teaching/courses/inf1/fp/2008/tutorials/Tutorial7.pdf]

				// 32-segment
				//temp.append("F+F-F-F+F+FF-F+F+FF+F-F-FF+FF-FF+F+F-FF-F-F+FF-F-F+F+F-F+");

				//temp.append("F-[[F]+F]+F[+FF]-F");


				//temp.append("F-F+F+FF-F-F+F");


				//temp.append("-F+FF++F+F--F-F");
			}
		}
		axiom.swap(temp);
	}	
}

void LSystemClass::Parse(int j)
{
	for (unsigned i = 0; i<axiom.length(); ++i)
	{
		if (axiom.at(i) == 'F')
		{
			// Move the Matrix forward.
		}
		else if (axiom.at(i) == '+')
		{
			// Rotate the Matrix in the positive direction.
		}
		else if (axiom.at(i) == '-')
		{
			// Rotate the Matrix in the negative direction.
		}
		else if (axiom.at(i) == '[')
		{
			// Begin a branch.
			// Push the current matrix onto the stack.
		}
		else if (axiom.at(i) == ']')
		{
			// End a branch.
			// Pop the current matrix off the stack.
		}
	}
}

void LSystemClass::ShutDown()
{
	axiom.clear();
	temp.clear();
}

std::string LSystemClass::GetAxiom()
{
	return axiom;
}
