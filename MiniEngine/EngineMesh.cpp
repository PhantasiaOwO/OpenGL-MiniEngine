#include "EngineMesh.h"

void EngineMesh::DrawMesh()
{
	glEnable(GL_TEXTURE_2D);//激活二维纹理贴图
	for (uint32_t i = 0; i < textures.size(); ++i)
	{
		//暂时只支持漫反射贴图
		if(i == 0)
		glBindTexture(GL_TEXTURE_2D, textures[i].id);
	}
	for (size_t i = 0; i < indices.size(); i+=3) 
	{
		glBegin(GL_TRIANGLES);
		for (int j = 0; j < 3; ++j) 
		{
			//传入顶点纹理坐标
			glTexCoord2f(vertices[indices[i + j]].TexCoords.x, vertices[indices[i + j]].TexCoords.y);
			//传入顶点法线
			glNormal3fv(vertices[indices[i+j]].Normal.DataPtr());
			//传入顶点坐标
			glVertex3fv(vertices[indices[i+j]].Position.DataPtr());
		}
		glEnd();
	}
	glDisable(GL_TEXTURE_2D);
}
