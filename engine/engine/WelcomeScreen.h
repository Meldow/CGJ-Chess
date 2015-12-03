#pragma once
#include <iostream>
#include <fstream>
#include <string>

class WelcomeScreen {
public:
	WelcomeScreen() {
		std::ifstream Reader("welcome.txt");             //Open file
		std::string Art = getFileContents(Reader);       //Get file
		std::cout << Art << std::endl;               //Print it to the screen
		Reader.close();                           //Close file
	};
	~WelcomeScreen() {};

	//std::string getFileContents(std::ifstream&);            //Gets file contents
	std::string getFileContents(std::ifstream& File) {
		std::string Lines = "";        //All lines

		if (File)                      //Check if everything is good
		{
			while (File.good()) {
				std::string TempLine;                  //Temp line
				std::getline(File, TempLine);        //Get temp line
				TempLine += "\n";                      //Add newline character

				Lines += TempLine;                     //Add newline
			}
			return Lines;
		} else                           //Return error
		{
			return "ERROR File does not exist.";
		}
	}
};
