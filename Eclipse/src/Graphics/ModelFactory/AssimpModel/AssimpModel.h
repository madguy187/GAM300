#ifndef ASSIMP_MODEL_CLASS_H
#define ASSIMP_MODEL_CLASS_H

#include "Importer.hpp"
#include "scene.h"
#include "postprocess.h"
#include <vector>
#include "mesh.h"

// fbx
#include "cimport.h"

namespace Eclipse
{
    // GTLF Reader
    // Render Multiple Have issues for now
    class AssimpModel
    {
    private:
        std::string NameOfModel;
        std::vector<Mesh> meshes;
        std::string directory;
        std::vector<Texture> textures_loaded;

        Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);
        std::vector<Texture> LoadTextures(aiMaterial* mat, aiTextureType type);
        void ProcessNode(aiNode* node, const aiScene* scene);
        void CheckUniformLoc(Shader& _shdrpgm, CameraComponent& _camera);
    public:
        bool noTex;
        unsigned int ID = 0;
        GLenum GlobalMode = GL_FILL;

        AssimpModel() { }
        AssimpModel(bool noTex = false);

        void LoadAssimpModel(std::string path);
        void Render(Shader& shader, GLenum mode);
        void Cleanup();
        void SetName(std::string name);
        std::string GetName();
        std::string GetDirectory();
        unsigned int GetNumberOfTextures();
        void GetTextureNames();

        // fbx
    public:
        void FBXLoadAssimpModel(const char* in);
    };

}
#endif // ASSIMP_MODEL_CLASS