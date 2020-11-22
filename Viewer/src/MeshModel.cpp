#include "MeshModel.h"
#include <iostream>


MeshModel::MeshModel(std::vector<Face> faces, std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, const std::string& model_name, glm::mat4x4 T) :
	faces_(faces),
	vertices_(vertices),
	normals_(normals),
	STmat(T)
{

}

MeshModel::~MeshModel()
{
}

const Face& MeshModel::GetFace(int index) const
{
	return faces_[index];
}

int MeshModel::GetFacesCount() const
{
	return faces_.size();
}

const std::string& MeshModel::GetModelName() const
{
	return model_name_;
}
const glm::vec3 MeshModel::GetVertex(int index) const
{
	return vertices_[index - 1];
}

const glm::mat4x4 MeshModel::GetSTmat() const
{
	return STmat;
}
void MeshModel::PrintFaces() const
{
	int index;
	for (int i = 0; i <faces_.size(); i++)
	{
		std::cout << "the vartices of face i :";
		index = faces_[i].GetVertexIndex(0);
		std::cout <<'(' << vertices_[index-1].x << ','<< vertices_[index-1].y << ','<< vertices_[index-1].z << ')';
		index = faces_[i].GetVertexIndex(1);
		std::cout << '(' << vertices_[index-1].x << ',' << vertices_[index-1].y << ',' << vertices_[index-1].z << ')';
		index = faces_[i].GetVertexIndex(2);
		std::cout << '(' << vertices_[index-1].x << ',' << vertices_[index-1].y << ',' << vertices_[index-1].z << ')';
		std::cout << "\n";

	}
	
	return;
}
void MeshModel::PrintVertices()const
{
	glm::vec3 vtemp;
	std::cout << "all the  vertices:";
	for (int i = 1; i < vertices_.size(); i++)
	{
		std::cout << '(' << vertices_[i - 1].x << ',' << vertices_[i - 1].y << ',' << vertices_[i - 1].z << ')';
		std::cout << "\n";

	}
}
