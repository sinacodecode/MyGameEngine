#ifndef MODEL_H
#define MODEL_H

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include "assimp/postprocess.h"
#include <string>
#include "stb_image.h"
#include <vector>
#include <map>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Mesh.h"
#include "shader_s.h"

using namespace std;

//this function doesnt belong here :(
inline unsigned int TextureFromFile(const char* path, const string& directory, bool gamma = false)
{
    string filename = string(path);
    filename = directory + '/' + filename;
    std::cout << filename << '\n';
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width{};
    int height{};
    int nrComponents{};
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (nullptr != data)
    {
        GLenum format{};
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        if (nrComponents == 4)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        }
        else
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        }
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << '\n';
        stbi_image_free(data);
    }

    return textureID;
}

class Model
{
public:
    // model data 
    vector<Texture> m_textures_loaded;	// stores all the textures loaded so far, optimization to make sure textures aren't loaded more than once.
    vector<Mesh>    m_meshes;
    int m_shaderID{};
    string m_directory;
    bool m_gammaCorrection;

    // constructor, expects a filepath to a 3D model.
    Model(string const& path, int shaderID, bool gamma = false) : m_shaderID{shaderID} ,m_gammaCorrection(gamma)
    {
        loadModel(path);
    }

    // draws the model, and thus all its meshes
    void Draw(Shader& shader) const;

private:
    // loads a model with supported ASSIMP extensions from file and stores the resulting meshes in the meshes vector.
    void loadModel(const string& path);

    // processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    void processNode(aiNode* node, const aiScene* scene);

    Mesh processMesh(aiMesh* mesh, const aiScene* scene);

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType type, string typeName);
};

#endif
