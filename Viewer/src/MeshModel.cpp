#include "MeshModel.h"

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