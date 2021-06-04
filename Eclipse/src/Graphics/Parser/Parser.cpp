#include "pch.h"
#include "Parser.h"
#include <vector>

void Parser::ParseFile(std::string fileName)
{ 
    std::ifstream file(fileName.c_str());

    if (file)
    {
        rapidjson::IStreamWrapper ss(file);

        if (doc.ParseStream(ss).HasParseError())
        {
            std::cerr << "Error(offset " << static_cast<unsigned>(doc.GetErrorOffset())
                << "):" << GetParseError_En(doc.GetParseError()) << std::endl;

            throw std::invalid_argument("Parse error!");
        }
         
        file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open Json file!");
    }
}

void Parser::GenerateFile(std::string outputFile)
{
    /*DO NOT USE YET, IMPLEMENTATION INCOMPLETE*/
    std::ofstream output(outputFile.c_str());

    if (output)
    {
        rapidjson::Document  document;
    	
        std::string tempstring{ outputFile };
    	
        document.Parse(tempstring.c_str());

        FILE* fp = fopen("output.json", "wb"); // non-Windows use "w"
    	
        char writeBuffer[65536];
    	
        rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));

        rapidjson::Writer<rapidjson::FileWriteStream> writer(os);

        document.Accept(writer);

        //TEST CODE
        //std::stringstream ss_date;
        //ss_date << "30 Sep 2020";
        //
        //std::vector<std::string> m_vWriteFilePath;
        //m_vWriteFilePath.push_back("0.mp3");
        //m_vWriteFilePath.push_back("1.mp3");
        //m_vWriteFilePath.push_back("2.mp3");
        //m_vWriteFilePath.push_back("3.mp3");
        //m_vWriteFilePath.push_back("4.mp3");
        //
        //writer.StartObject();
        //writer.Key("version");
        //writer.String("1.0");
        //writer.Key("data");
        //writer.String(ss_date.str().c_str());
        //writer.Key("file");
        //writer.StartArray();
        //
        //for (const auto& path : m_vWriteFilePath) {
        //    writer.String(path.c_str());
        //}
        //writer.EndArray();
        //writer.EndObject();

        fclose(fp);
	    
    }
    else
    {
        throw std::runtime_error("Unable to save Json file!");
    }
}

void Parser::PrintMesh(std::string fileName)
{
    std::cout << "--------------------" << std::endl;
    std::cout << "Mesh File Contents" << std::endl;

    std::ifstream file(fileName.c_str());
    rapidjson::Document tempDoc;

    if (file)
    {
        rapidjson::IStreamWrapper ss(file);

        if (tempDoc.ParseStream(ss).HasParseError())
        {
            std::cerr << "Error(offset " << static_cast<unsigned>(tempDoc.GetErrorOffset())
                << "):" << GetParseError_En(tempDoc.GetParseError()) << std::endl;

            throw std::invalid_argument("Parse error!");
        }

        file.close();
    }
    else
    {
        throw std::runtime_error("Unable to open Json file!");
    }

    std::string name = tempDoc["name"].GetString();
    const rapidjson::Value& vertex = tempDoc["vertex"].GetArray();
    const rapidjson::Value& index = tempDoc["index"].GetArray();

    std::cout << "Name: " << name << std::endl;
    std::cout << "Vertices: " << std::endl;

    for (rapidjson::SizeType i = 0; i < vertex.Size(); i++)
    {
        const rapidjson::Value& coords = vertex[i];

        std::cout << "[ " << coords[rapidjson::SizeType(0)].GetDouble() << ", "
            << coords[rapidjson::SizeType(1)].GetDouble() << " ]" << std::endl;
    }

    std::cout << "Indices: " << std::endl;
    std::string circleMesh = "circle";

    if (name.compare(circleMesh) == 0)
    {
        for (auto& i : index.GetArray())
        {
            std::cout << i.GetUint() << std::endl;
        }
    }
    else
    {
        for (rapidjson::SizeType i = 0; i < index.Size(); i++)
        {
            const rapidjson::Value& coords = index[i];

            std::cout << "[ " << coords[rapidjson::SizeType(0)].GetUint() << ", "
                << coords[rapidjson::SizeType(1)].GetUint() << ", "
                << coords[rapidjson::SizeType(2)].GetUint() << " ]" << std::endl;
        }
    }

    std::cout << "--------------------" << std::endl;
}



