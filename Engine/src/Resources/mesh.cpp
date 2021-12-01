#include "mesh.hpp"

#include <fstream>

#include "resources_manager.hpp"

namespace Resources
{
	Mesh::Mesh(const std::string& name, const std::string& parentMeshName)
		: Resource(name), parentMeshName(parentMeshName)
	{

	}

	Mesh::~Mesh()
	{
		// Destroy the VBO
		if (VAO)
			glDeleteVertexArrays(1, &VAO);

		// Destroy the VBO
		if (VBO)
			glDeleteBuffers(1, &VBO);
	}

	// Generate VAO, VBO and EBO from mesh
	void Mesh::generateVAO()
	{
		// VAO initialization and binding
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO initialization and binding
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		GLsizei stride = sizeof(Vertex);

		glBufferData(GL_ARRAY_BUFFER, vertices.size() * stride, vertices.data(), GL_STATIC_DRAW);

		// Set the attrib pointer to the positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(offsetof(Vertex, position)));
		glEnableVertexAttribArray(0);

		// Set the attrib pointer to the texture coordinates
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(offsetof(Vertex, texCoords)));
		glEnableVertexAttribArray(1);

		// Set the attrib pointer to the tangents
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(offsetof(Vertex, tangent)));
		glEnableVertexAttribArray(2);

		// Set the attrib pointer to the bitangents
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(offsetof(Vertex, bitangent)));
		glEnableVertexAttribArray(3);

		// Set the attrib pointer to the normals
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(offsetof(Vertex, normal)));
		glEnableVertexAttribArray(4);

		glEnableVertexAttribArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Mesh::compute(std::array<unsigned int, 3> offsets, std::vector<Core::Maths::vec3>& positions, std::vector<Core::Maths::vec3>& texCoords, std::vector<Core::Maths::vec3>& normals, std::vector<unsigned int>& indices)
	{
		// Create attributs vector from mesh values
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			Vertex vertex;
			vertex.position = positions[indices[i] - offsets[0]];

			if (!texCoords.empty())
				vertex.texCoords = texCoords[indices[i + 1] - offsets[1]];

			vertex.normal = normals[indices[i + 2] - offsets[2]];

			vertices.push_back(vertex);
		}

		if (!texCoords.empty())
		for (size_t i = 0; i < vertices.size(); i += 3)
		{
			Vertex& vert1 = vertices[i + 0];
			Vertex& vert2 = vertices[i + 1];
			Vertex& vert3 = vertices[i + 2];

			const Core::Maths::vec3& deltaPos1 = vert2.position - vert1.position;
			const Core::Maths::vec3& deltaPos2 = vert3.position - vert1.position;

			const Core::Maths::vec3& deltaUV1 = vert2.texCoords - vert1.texCoords;
			const Core::Maths::vec3& deltaUV2 = vert3.texCoords - vert1.texCoords;

			float f = 1.f / (deltaUV1.u * deltaUV2.v - deltaUV2.u * deltaUV1.v);

			Core::Maths::vec3 tangent = f * (deltaUV2.v * deltaPos1 - deltaUV1.v * deltaPos2);
			Core::Maths::vec3 bitangent = f * (deltaUV1.u * deltaPos2 - deltaUV2.u * deltaPos1);

			vert1.tangent = vert2.tangent = vert3.tangent = tangent;
			vert1.bitangent = vert2.bitangent = vert3.bitangent = bitangent;
		}

		// Tell to the RM that the initialization is finished
		ResourcesManager::addToMainThreadInitializerQueue(this);
	}

	void addData(std::vector<Core::Maths::vec3>& dataVector, const std::string& line)
	{
		std::istringstream iss(line);

		// Get a 3D Vector data form string stream
		Core::Maths::vec3 data = { 0.f };

		iss >> data.x;
		iss >> data.y;
		iss >> data.z;

		dataVector.push_back(data);
	}

	// Use to know if needed to triangulate faces
	int getNumFace(const std::string& line)
	{
		int numFace = 0;

		// Check how many spaces the line has
		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == ' ')
				numFace++;
		}

		return numFace;
	}

	// Give the number of vertices of a face from an .obj
	int numVerticesFace(const std::string& line)
	{
		int numV = 0;

		for (size_t i = 0; i < line.length(); i++)
		{
			if (line[i] == '/')
				numV++;
		}

		return numV > 6 ? 4 : 3;
	}

	// Allow to know if face is of type v//vn or v/uv/vn
	bool hasVU(const std::string& line)
	{
		for (size_t i = 3; i < line.length(); i++)
		{
			if (line[i - 1] == '/' && line[i] == '/')
				return true;
		}

		return false;
	}

	void addIndices(std::vector<unsigned int>& indices, const std::string& line)
	{
		std::istringstream iss(line);

		unsigned int indicesVertices[4];
		unsigned int indicesUV[4];
		unsigned int indicesNormals[4];

		// Number of vertices of the face 
		int numV = numVerticesFace(line);

		// Data type of the face
		bool doesHaveUV = hasVU(line);

		for (int i = 0; i < numV; i++)
		{
			// Face of type v/uv/vn
			if (!doesHaveUV)
			{
				iss >> indicesVertices[i];
				iss.ignore();
				iss >> indicesUV[i];
				iss.ignore();
				iss >> indicesNormals[i];
				iss.ignore();
			}
			// Face of type v//vn
			else
			{
				iss >> indicesVertices[i];
				iss.ignore();
				iss.ignore();
				iss >> indicesNormals[i];

				indicesUV[i] = 1;
			}

			int indexV2Strip = i;
			// Strip faces (triangulation)
			if (i > 2)
			{
				indices.push_back(indicesVertices[0] - 1);
				indices.push_back(indicesUV[0] - 1);
				indices.push_back(indicesNormals[0] - 1);
				indices.push_back(indicesVertices[i - 1] - 1);
				indices.push_back(indicesUV[i - 1] - 1);
				indices.push_back(indicesNormals[i - 1] - 1);
			}

			// Add vertex indices to the mesh indices
			indices.push_back(indicesVertices[i] - 1);
			indices.push_back(indicesUV[i] - 1);
			indices.push_back(indicesNormals[i] - 1);
		}
	}

	void Mesh::parse(const std::string& toParse, std::array<unsigned int, 3> offsets)
	{
		std::istringstream stringStream(toParse);

		std::vector<Core::Maths::vec3> positions;
		std::vector<Core::Maths::vec3> texCoords;
		std::vector<Core::Maths::vec3> normals;
		std::vector<unsigned int> indices;
		
		// Parse the attributs
		std::string line;
		while (std::getline(stringStream, line))
		{
			std::string_view view = line;

			if (view.starts_with("#") || view == "" || view.starts_with("\n"))
				continue;

			if (view.starts_with("v "))
				addData(positions, line.substr(2));
			else if (view.starts_with("vt "))
				addData(texCoords, line.substr(3));
			else if (view.starts_with("vn "))
				addData(normals, line.substr(3));
			else if (view.starts_with("f "))
				addIndices(indices, line.substr(2));
		}

		compute(offsets, positions, texCoords, normals, indices);
	}

	void Mesh::draw() const
	{
		if (!VAO)
			return;

		// Bind the mesh's VAO and draw it
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)vertices.size() * sizeof(Vertex));
		glBindVertexArray(0);
	}

	void Mesh::mainThreadInitialization()
	{
		// Generate the VAO buffer
		generateVAO();
	}
}