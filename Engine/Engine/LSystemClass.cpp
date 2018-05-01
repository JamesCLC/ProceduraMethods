#include "LSystemClass.h"



LSystemClass::LSystemClass()
{
	// Default Axiom starter value.
	axiom = "F";
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
	m_ImprovedNoise = new improvednoise;
	double sample = 0.0;

	for (int i = 0; i < itterations; i++)
	{
		// Create a temporary string to store this itteration's results.
		temp.clear();

		// For each character in the axiom
		for (unsigned i = 0; i<axiom.length(); ++i)
		{
			// Sample Perlin noise for pseudo-random variation.
			//sample = m_ImprovedNoise->Sample((double)itterations * 2, (double)itterations * 2, (double)0);

			// generate a pseudo-random number between 1 and 3.
			sample = rand() % 3 + 1;

			// Apply a rule based on the sampled noise.
			if (axiom.at(i) == 'F')
			{
				temp.append("FF[+[<F-F-F]-[>F+F+F]");

				//if (sample >= 3)
				//{
				//	temp.append("F[+F]F[-F]F");
				//}
				//else if (sample >= 2)
				//{
				//	// Rule
				//}
				//else
				//{
				//	temp.append("FF[FF+[<F-F-F]-[>F+F+F]");
				//}

				// Primary Rule
				//temp.append("F[+F]F[-F]F");
				// Upward shennanigans?
				//temp.append("[+FF+FFFF]");
				//////// Tested Rules ////////
				//temp.append("F[+F][-F[-F]F]F[+F][-F]");
				//temp.append("F[-FF][+FF][FF");
				//temp.append("F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F");
				//temp.append("FF+[+F-F-F]-[-F+F+F]");
				// Palm Tree.
				//temp.append("[+F+F+F+F+F+F+F][-F-F-F-F-F-F-F]FFFFF");
				// Palm Tree, but with branches on branches.
				//temp.append("[+F+F+F+F+F[-F-F-F]+F+F][-F-F-F-F-F[+F+F+F]-F-F]FFFFF");
				// Example L-Systems from [http://www.inf.ed.ac.uk/teaching/courses/inf1/fp/2008/tutorials/Tutorial7.pdf]
				// 32-segment
				//temp.append("F+F-F-F+F+FF-F+F+FF+F-F-FF+FF-FF+F+F-FF-F-F+FF-F-F+F+F-F+");
				//temp.append("F-[[F]+F]+F[+FF]-F");
				//temp.append("F-F+F+FF-F-F+F");
				//temp.append("-F+FF++F+F--F-F");
				//////// Tested Rules ////////
			} // End "F"

			// Debug
			if (i == (axiom.length() - 1))
			{
				int foo = sample;
			}
		}
		// Update the axiom to be the product of this itteration
		axiom.swap(temp);
	} // End Itterations

	if (m_ImprovedNoise)
	{
		m_ImprovedNoise->Shutdown();
		delete m_ImprovedNoise;
		m_ImprovedNoise = 0;
	}
}

void LSystemClass::Parse(int j)
{
	// Moved to modelclass.cpp!

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

	// Delete the improved noise object.
	if (m_ImprovedNoise)
	{
		m_ImprovedNoise->Shutdown();
		delete m_ImprovedNoise;
		m_ImprovedNoise = 0;
	}
}

std::string LSystemClass::GetAxiom()
{
	return axiom;
}
