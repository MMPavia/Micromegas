#include <string>
#include <iostream>
#include <sstream>

int main(int argc, char *argv[])
{	
	std::stringstream str;
	str << argv[1];	
	int dec;
	str >> dec;

	if (dec>6)
	{
		std::cout << "Massimo 6 decimali" << std::endl;
		dec = 6;
	}

	std::string mis = "+1.234567E+10";
	std::string dec_cut = mis.substr(0,dec+3);
	std::string pwr_cut = mis.substr(9,4);
	std::string mis_cut = dec_cut + pwr_cut; 
	std::cout << mis_cut << std::endl;
}
