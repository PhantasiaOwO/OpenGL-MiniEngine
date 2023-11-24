#include "EngineModel.h"
#include <stb_image.h>

void EngineModel::loadModel(const string& path)
{
    Assimp::Importer importer;
    //��ȡ·��ģ����Ϣ,Assimp�⸨���������ݽṹ
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
        //�ݹ��ȡ�ӽڵ�
        traverseNode(node->mChildren[i], scene);
    }
}

void EngineModel::storeMesh(aiMesh* mesh, const aiScene* scene)
{
    //������Ϣ
    vector<Vertex> vertices;
    //��������
    vector<uint32_t> indices;
    //������Ϣ
    vector<Texture> textures;

    for (uint32_t i = 0; i < mesh->mNumVertices; ++i)
    {
        
        Vertex vertex;
        glm::vec3 vector;
        //λ��
        vector.x = mesh->mVertices[i].x;
        vector.y = mesh->mVertices[i].y;
        vector.z = mesh->mVertices[i].z;
        vertex.Position = vector;


        //����
        vector.x = mesh->mNormals[i].x;
        vector.y = mesh->mNormals[i].y;
        vector.z = mesh->mNormals[i].z;
        vertex.Normal = vector;

        //����
        if (mesh->mTextureCoords[0])
        {
            glm::vec2 vec;
            vec.x = mesh->mTextureCoords[0][i].x;
            vec.y = mesh->mTextureCoords[0][i].y;
            vertex.TexCoords = vec;
        }
        else
            vertex.TexCoords = glm::vec2(0.0f, 0.0f);

        //����
        vector.x = mesh->mTangents[i].x;
        vector.y = mesh->mTangents[i].y;
        vector.z = mesh->mTangents[i].z;
        vertex.Tangent = vector;
        vertices.emplace_back(vertex);
    }
    
    //��ȡ������
    for (uint32_t i = 0; i < mesh->mNumFaces; ++i)
    {
        aiFace face = mesh->mFaces[i];
        for (uint32_t j = 0; j < face.mNumIndices; ++j)
        {
            indices.emplace_back(face.mIndices[j]);
        }
    }
    
    //��ȡ������Ϣ
    aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
    
    ///��������
    //����������
    vector<Texture> diffuseMaps = loadMaterialTextures(material, aiTextureType_DIFFUSE, TextureType::DiffuseMap);
    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
    //�߹ⷴ������
    vector<Texture> specularMaps = loadMaterialTextures(material, aiTextureType_SPECULAR, TextureType::SpecularMap);
    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
    //�������
    std::vector<Texture> normalMaps = loadMaterialTextures(material, aiTextureType_HEIGHT, TextureType::HeightMap);
    textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
    //��������
    std::vector<Texture> heightMaps = loadMaterialTextures(material, aiTextureType_AMBIENT, TextureType::NormalMap);
    textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

    meshes.push_back(EngineMesh(vertices, indices, textures));
}


vector<Texture> EngineModel::loadMaterialTextures(aiMaterial* mat, aiTextureType type, TextureType textureType)
{
    vector<Texture> textures;
    //������ǰ�������ͼ��
    for (uint32_t i = 0; i < mat->GetTextureCount(type); ++i)
    {
        aiString str;
        //�����ͼ·��,ͨ��mtl�ļ�ȷ�ϵ�����λ��
        mat->GetTexture(type, i, &str);
        //��ֹ��ͼ�ظ���ȡ
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

    ///������������
    unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        //ȷ����ɫ�ռ�
        GLenum format = 0;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;
        //�����
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        //�����Ʒ�ʽ
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        //����MIPMAP�����㷨
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
