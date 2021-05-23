
/**********************************************************************************
* \file         Particles.h
*
* \brief        Definition of the Particles class function(s).
*
* \author       Lin Yanping Rachel
*
* \email		l.yanpingrachel@digipen.edu
*
* \date			25 JFeb 2021
*
* \copyright    Copyright (c) 2020 DigiPen Institute of Technology. Reproduction
*                or disclosure of this file or its contents without the prior
*                written consent of DigiPen Institute of Technology is prohibited.
**********************************************************************************/
#ifndef PARTICLES_H
#define PARTICLES_H

#include "Parser.h"

class Particles
{
public:
    Particles();

    void InsertPosVtx();
    void InsertTextCoord();

    GLuint GetVaoID();
    GLuint GetVboID();
    GLuint GetIndexID();

    GLuint GetIndexVecSize();

    void GenerateIndices(unsigned int numQuads);
    void CreateBuffers();
private:
    GLuint vaoID;
    GLuint vboID;
    GLuint indexID;

    std::vector<glm::vec2> PosVec;
    std::vector<glm::vec2> TextVec;
    std::vector<unsigned int> IndexVec;
};

#endif /* PARTICLES_H */
