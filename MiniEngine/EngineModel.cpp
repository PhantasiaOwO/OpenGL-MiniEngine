#include "EngineModel.h"
#include <stb_image.h>

void EngineModel::loadModel(const string& path)
{
    Assimp::Importer importer;
    //读取路径模型信息,Assimp库辅助建立数据结构
    cout << "Begin Load Model" << endl;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
    cout << "Finished" << endl;
    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    traverseNode(scene->mRootNode, scene);
}

void EngineModel::traverseNode(aiNode* node, const aiScene* scene)
{
    for (uint32_t i = 0; i < node->mNumMeshes; ++i)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        storeMesh(mesh,scene);
    }

    for (uint32_t i = 0; i < node->mNumChildren; ++i)
    {
        //递归读取子节点
        traverseNode(node->mChildren[i], scene);
    }
}

void EngineModel::storeMesh(aiMesh* mesh, const aiScene* scene)
{
    //顶点信息
    vector<Vertex> vertices;
    //链接索引
    vector<uint32_t> indices;
    //纹理信息
    vector<Texture> textures;

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
    {
        
        Vertex vertex;
        glm::vec3 vector;
        //位置
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;


        //法线
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        //纹理
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        //切线
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
        vertices.emplace_back(vertex);
    }
    
    //读取面索引
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    
    //读取材质信息
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    
    ///加载纹理
    //漫反射纹理
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DiffuseMap);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //高光反射纹理
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SpecularMap);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //深度纹理
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::HeightMap);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //法线纹理
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::NormalMap);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    meshes.push_back(EngineMesh(vertices, indices, textures));
}


vector<Texture> EngineModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType)
{
    vector<Texture> textures;
    //遍历当前种类的贴图数
    for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        //获得贴图路径,通过mtl文件确认的纹理位置
        mat->GetTexture(type, i, &str);
        //防止贴图重复读取
        bool skip = false;
        for (uint32_t j = 0; j < textures_loaded.size(); ++j)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                skip = true;
                break;
            }
        }

        if (!skip)
        {
            Texture texture;
            texture.id = loadTexture(str.C_Str());
            texture.type = textureType;
            texture.path = str.C_Str();
            textures.emplace_back(texture);
            textures_loaded.emplace_back(texture);
        }
    }
    return textures;
}

uint32_t EngineModel::loadTexture(const char* path)
{
    string filename = string(path);
    filename = this->directory + '/' + filename;

    GLuint textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;

    ///加载纹理数据
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        //确定颜色空间
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        //纹理绑定
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        //纹理环绕方式
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //设置MIPMAP采样算法
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
