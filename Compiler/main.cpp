#include "pch.h"
#include "Compilers.h"
#include "CompilerSerializer.h"
#include <iostream>
#include <fstream>
#include <bitset>

using namespace EclipseCompiler;
using namespace std;

std::unique_ptr<Compilers> AllCompilers;

void convertToBinary(unsigned int n)
{
    if (n / 2 != 0) {
        convertToBinary(n / 2);
    }
    printf("%d", n % 2);
}

static std::string base64_encode(const std::string& in) {

    std::string out;

    int val = 0, valb = -6;
    for (char c : in) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) out.push_back("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

static std::string base64_decode(const std::string& in) {

    std::string out;

    std::vector<int> T(256, -1);
    for (int i = 0; i < 64; i++) T["ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"[i]] = i;

    int val = 0, valb = -8;
    for (char c : in) {
        if (T[c] == -1) break;
        val = (val << 6) + T[c];
        valb += 6;
        if (valb >= 0) {
            out.push_back(char((val >> valb) & 0xFF));
            valb -= 8;
        }
    }
    return out;
}

int main()
{
    AllCompilers = std::make_unique<Compilers>();
    AllCompilers->LoadModels("..//Eclipse//src//Assets//ASSModels");

    ofstream file1;

    file1.exceptions(ifstream::failbit | ifstream::badbit);
    try
    {
        file1.open("CSC2134.txt", fstream::in | fstream::out | fstream::binary | ios::trunc);
    }
    catch (ifstream::failure e)
    {
        cout << "Exception opening/reading file" << e.what();
    }

    for (auto& i : AllCompilers->Geometry)
    {
        // Name , NoTextures , Diffuse , Specular , Ambient
        file1 << i.first << std::endl;
        file1 << (i.second->NoTex ? 1 : 0) << std::endl;
        file1 << i.second->Diffuse.x << " " << i.second->Diffuse.y << " " << i.second->Diffuse.z << std::endl;
        file1 << i.second->Specular.x << " " << i.second->Specular.y << " " << i.second->Specular.z << std::endl;
        file1 << i.second->Ambient.x << " " << i.second->Ambient.y << " " << i.second->Ambient.z << std::endl;
        file1 << "-" << std::endl;

        for (auto& Vertices : i.second->Vertices)
        {
            file1 << Vertices.Position.x << " " << Vertices.Position.y << " " << Vertices.Position.z << std::endl;
            file1 << Vertices.Normal.x << " " << Vertices.Normal.y << " " << Vertices.Normal.z << std::endl;
            file1 << Vertices.TextureCoodinates.x << " " << Vertices.TextureCoodinates.y << std::endl;
        }
        file1 << "--" << std::endl;

        for (auto& Indices : i.second->Indices)
        {
            file1 << Indices << std::endl; 
        }
        file1 << "---" << std::endl;

    }

    file1.close();

    return 0;
}

