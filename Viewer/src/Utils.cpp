#include <memory>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>


#include "Utils.h"

glm::vec3 Utils::Vec3fFromStream(std::istream& issLine)
{
	float x, y, z;
	issLine >> x >> std::ws >> y >> std::ws >> z;
	return glm::vec3(x, y, z);
}

glm::vec2 Utils::Vec2fFromStream(std::istream& issLine)
{
	float x, y;
	issLine >> x >> std::ws >> y;
	return glm::vec2(x, y);
}

std::shared_ptr<MeshModel> Utils::LoadMeshModel(const std::string& filePath)
{
	std::vector<Face> faces;
	std::vector<glm::vec3> vertices;
	std::vector<glm::vec3> normals;
	std::ifstream ifile(filePath.c_str());
	glm::vec3 Vertex;
	float maxX = 0, minX = 0, maxY = 0, minY = 0, maxZ = 0, minZ = 0, MAX = 0;
	

	// while not end of file
	while (!ifile.eof())
	{
		// get line
		std::string curLine;
		std::getline(ifile, curLine);

		// read the type of the line
		std::istringstream issLine(curLine);
		std::string lineType;

		issLine >> std::ws >> lineType;

		// based on the type parse data
		if (lineType == "v")
		{
			Vertex = Utils::Vec3fFromStream(issLine);
			if(Vertex.x > maxX)
			{
				maxX = Vertex.x;
			}
			if (Vertex.x < minX)
			{
				minX = Vertex.x;
			}
			if (Vertex.y > maxY)
			{
				maxY = Vertex.y;
			}
			if (Vertex.y < minY)
			{
				minY = Vertex.y;
			}
			if (Vertex.z > maxZ)
			{
				maxZ = Vertex.z;
			}
			if (Vertex.z < minZ)
			{
				minZ = Vertex.z;
			}
			vertices.push_back(Vertex);
		}
		else if (lineType == "vn")
		{
			normals.push_back(Utils::Vec3fFromStream(issLine));
		}
		else if (lineType == "vt")
		{
			// TODO: Handle texture coordinates
		}
		else if (lineType == "f")
		{
			faces.push_back(Face(issLine));
		}
		else if (lineType == "#" || lineType == "")
		{
			// comment / empty line
		}
		else
		{
			std::cout << "Found unknown line Type \"" << lineType << "\"";
		}

	}
	if(maxX>= maxY && maxX >= maxZ)
	{
		MAX = maxX;
	}
	if (maxY >= maxX && maxY >= maxZ)
	{
		MAX = maxY;
	}
	if (maxZ >= maxY && maxZ >= maxX)
	{
		MAX = maxZ;
	}
	glm::mat4x4 ScalingMat = glm::scale(glm::vec3(350 / MAX, 350 / MAX, 350 / MAX));
	glm::mat4x4 TranMat = glm::translate(glm::vec3(abs(minX), abs(minY), abs(minZ)));
	return std::make_shared<MeshModel>(faces, vertices, normals, Utils::GetFileName(filePath),ScalingMat*TranMat);
}

std::string Utils::GetFileName(const std::string& filePath)
{
	if (filePath.empty()) {
		return {};
	}

	auto len = filePath.length();
	auto index = filePath.find_last_of("/\\");

	if (index == std::string::npos) {
		return filePath;
	}

	if (index + 1 >= len) {

		len--;
		index = filePath.substr(0, len).find_last_of("/\\");

		if (len == 0) {
			return filePath;
		}

		if (index == 0) {
			return filePath.substr(1, len - 1);
		}

		if (index == std::string::npos) {
			return filePath.substr(0, len);
		}

		return filePath.substr(index + 1, len - index - 1);
	}

	return filePath.substr(index + 1, len - index);
}