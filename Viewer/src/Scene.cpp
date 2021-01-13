#include "Scene.h"
#include "MeshModel.h"
#include <string>

Scene::Scene() :
	active_camera_index_(0),
	active_model_index_(0)
{

}

void Scene::AddModel(const std::shared_ptr<MeshModel>& mesh_model)
{
	mesh_models_.push_back(mesh_model);
}

int Scene::GetModelCount() const
{
	return mesh_models_.size();
}

MeshModel& Scene::GetModel(int index) const
{
	return *mesh_models_[index];
}

MeshModel& Scene::GetActiveModel() const
{
	return *mesh_models_[active_model_index_];
}

void Scene::AddCamera(const std::shared_ptr<Camera>& camera)
{
	cameras_.push_back(camera);
}

int Scene::GetCameraCount() const
{
	return cameras_.size();
}

Camera& Scene::GetCamera(int index)
{
	return *cameras_[index];
}

Camera& Scene::GetActiveCamera()const
{
	return *cameras_[active_camera_index_];
}

void Scene::SetActiveCameraIndex(int index)
{
	active_camera_index_ = index;
}

int Scene::GetActiveCameraIndex() const
{
	return active_camera_index_;
}

void Scene::SetActiveModelIndex(int index)
{
	active_model_index_ = index;
}

int Scene::GetActiveModelIndex() const
{
	return active_model_index_;
}

void Scene::AddLight(const std::shared_ptr<light>& light)
{
	lights_.push_back(light);
	count_oflights = count_oflights + 1;
}

void Scene::SetActiveLightIndex(int index)
{
	active_light_index_ = index;
}


int Scene::GetActiveLightIndex() const
{
	return active_light_index_;
}

light& Scene::GetActivelight() const
{
	return *lights_[active_light_index_];
}

int Scene::Get_count_oflights() const
{
	return this->count_oflights;
}

light& Scene::GetLight(int index) const
{
	return *lights_[index];
}

int Scene::GetshadingLight() const
{
	return shading;
}
void Scene::Setshading(int index)
{
	shading = index;
}