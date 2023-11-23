#include "EngineMesh.h"

void EngineMesh::setupMesh()
{
    ///生成并绑定顶点数组对象
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);
    
    ///生成并绑定顶点缓冲对象
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    ///生成并绑定索引缓冲对象
    glGenBuffers(1, &EBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);


    ///传入顶点着色器渲染
    //启用通用顶点Attribute数组的Index索引
    glEnableVertexAttribArray(0);
    //参数一为传入哪个Layout
    //参数二确定传入值的大小
    //参数三确定数据类型
    //参数四决定数据是否做归一化
    //参数五为两个同类数据的相差间距
    //参数六为位置数据在缓冲中起始位置的偏移量
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

    ///解绑定
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

    ///绑定VAO
    glBindVertexArray(VAO);
    ///根据顶点索引绘制三角面片
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    ///解绑VAO
    glBindVertexArray(0);

    glActiveTexture(GL_TEXTURE0);
}
