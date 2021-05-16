/**********************************************************************************
* \file            Parser.h
*
* \brief        Definition of the Paser  class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			10 nov 2020
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef PARSER_H
#define PARSER_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <filereadstream.h>
#include <writer.h>
#include <document.h>
#include <istreamwrapper.h>
#include <ostreamwrapper.h>
#include <stringbuffer.h>
#include <../rapidjson/error/en.h>
#include <../rapidjson/document.h>
#include <../rapidjson/filewritestream.h>

class Parser
{
public:
	void ParseFile(std::string fileName);
	void GenerateFile(std::string outputFile);

	//Prints the contents of a given mesh file 
	//(Note: Attmpting to print the contents of an incompatible file using this function will result in an error)
	static void PrintMesh(std::string fileName);

	rapidjson::Document doc;
};

#endif /* PARSER_H */