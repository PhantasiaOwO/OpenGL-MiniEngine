#include "EngineMesh.h"

void EngineMesh::setupMesh()
{
    ///���ɲ��󶨶����������
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    ///���ɲ��󶨶��㻺�����
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    ///���ɲ��������������
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    ///���붥����ɫ����Ⱦ
    //����ͨ�ö���Attribute�����Index����
    glEnableVertexAttribArray(0);
    //����һΪ�����ĸ�Layout
    //������ȷ������ֵ�Ĵ�С
    //������ȷ����������
    //�����ľ��������Ƿ�����һ��
    //������Ϊ����ͬ�����ݵ������
    //������Ϊλ�������ڻ�������ʼλ�õ�ƫ����
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    ///���
    glBindVertexArray(0);
}

void EngineMesh::DrawMesh(const Shader& shader)
{
    uint32_t diffuseNr = 1;
    uint32_t specularNr = 1;
    uint32_t normalNr = 1;
    uint32_t heightNr = 1;
    for (uint32_t i = 0; i < textures.size(); ++i)
    {
        if (i > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS) break;
        glActiveTexture(GL_TEXTURE0 + i);
        string texture;
        if (textures[i].type == TextureType::DiffuseMap)
            texture = "texture_diffuse" + std::to_string(static_cast<long long>(diffuseNr++));
        else if (textures[i].type == TextureType::SpecularMap)
            texture = "texture_specular" + std::to_string(static_cast<long long>(specularNr++));
        else if (textures[i].type == TextureType::NormalMap)
            texture = "texture_normal" + std::to_string(static_cast<long long>(normalNr++));
        else if (textures[i].type == TextureType::HeightMap)
            texture = "texture_height" + std::to_string(static_cast<long long>(heightNr++));

        glUniform1i(glGetUniformLocation(shader.ID, (texture).c_str()), i);

        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }

    ///��VAO
    glBindVertexArray(VAO);
    ///���ݶ�����������������Ƭ
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    ///���VAO
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
