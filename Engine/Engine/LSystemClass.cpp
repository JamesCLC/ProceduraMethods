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
			sample = rand() % 10 + 1;

			// Apply a rule based on the sampled noise.
			if (axiom.at(i) == 'F')
			{
				if (sample >= 4)
				{
					temp.append("FF[-F+F]");
				}
				else if (sample >= 3)
				{
					temp.append("FF[+F-F]");
				}
				else
				{
					temp.append("F");
				}

				//temp.append("FF[+[<F-F-F]-[>F+F+F]");
				//temp.append("F[+F-FFF][-F+FFF]FF");

				//////// Tested Rules ////////
				//temp.append("F[+F]F[-F]F");
				//temp.append("[+FF+FFFF]");
				//temp.append("F[+F][-F[-F]F]F[+F][-F]");
				//temp.append("F[-FF][+FF][FF");
				//temp.append("F+FF-FF-F-F+F+FF-F-F+F+FF+FF-F");
				//temp.append("FF+[+F-F-F]-[-F+F+F]");
			
				//temp.append("[+F+F+F+F+F+F+F][-F-F-F-F-F-F-F]FFFFF");						// Palm Tree.
				//temp.append("[+F+F+F+F+F[-F-F-F]+F+F][-F-F-F-F-F[+F+F+F]-F-F]FFFFF");		// Palm Tree, but with branches on branches.
					
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
