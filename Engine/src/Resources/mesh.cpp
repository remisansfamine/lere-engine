#include "mesh.hpp"

#include <fstream>

#include "resources_manager.hpp"

namespace Resources
{
	void Mesh::draw() const
	{
		if (!this || !VAO)
			return;

		// Bind the mesh's VAO and draw it
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, (GLsizei)(attributs.size()));
		glBindVertexArray(0);
	}

	Mesh::Mesh(const std::string& name)
		: Resource(name)
	{

	}


	Mesh::~Mesh()
	{
		//glDeleteVertexArrays(1, &VAO);
	}

	// Generate VAO, VBO and EBO from mesh
	void Mesh::generateVAO()
	{
			// VAO initialization and binding
		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		// VBO initialization and binding
		GLuint VBO;
		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		glBufferData(GL_ARRAY_BUFFER, attributs.size() * sizeof(float), attributs.data(), GL_STATIC_DRAW);

		int stride = 3 * sizeof(Core::Maths::vec3);

		// Set the attrib pointer to the positions
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(0));
		glEnableVertexAttribArray(0);

		// Set the attrib pointer to the texture coordinates
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(sizeof(Core::Maths::vec3)));
		glEnableVertexAttribArray(1);

		// Set the attrib pointer to the normals
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, stride, (GLvoid*)(2 * sizeof(Core::Maths::vec3)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);

		// Destroy the VBO
		//glDeleteBuffers(1, &VBO);
	}

	void Mesh::compute(std::array<unsigned int, 3> offsets, std::vector<Core::Maths::vec3>& vertices, std::vector<Core::Maths::vec3>& texCoords, std::vector<Core::Maths::vec3>& normals, std::vector<unsigned int>& indices)
	{
		// Create attributs vector from mesh values
		for (size_t i = 0; i < indices.size(); i += 3)
		{
			Core::Maths::vec3& vertex = vertices[indices[i] - offsets[0]];
			attributs.push_back(vertex.x);
			attributs.push_back(vertex.y);
			attributs.push_back(vertex.z);

			if (!texCoords.empty())
			{
				Core::Maths::vec3& textureCoords = texCoords[indices[i + 1] - offsets[1]];
				attributs.push_back(textureCoords.x);
				attributs.push_back(textureCoords.y);
				attributs.push_back(textureCoords.z);
			}

			Core::Maths::vec3& normal = normals[indices[i + 2] - offsets[2]];
			attributs.push_back(normal.x);
			attributs.push_back(normal.y);
			attributs.push_back(normal.z);
		}

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

		std::vector<Core::Maths::vec3> vertices;
		std::vector<Core::Maths::vec3> texCoords;
		std::vector<Core::Maths::vec3> normals;
		std::vector<unsigned int> indices;

		std::string line;
		while (std::getline(stringStream, line))
		{
			std::string_view view = line;

			if (view.starts_with("#") || view == "" || view.starts_with("\n"))
				continue;

			if (view.starts_with("v "))
				addData(vertices, line.substr(2));
			else if (view.starts_with("vt "))
				addData(texCoords, line.substr(3));
			else if (view.starts_with("vn "))
				addData(normals, line.substr(3));
			else if (view.starts_with("f "))
				addIndices(indices, line.substr(2));
		}

		compute(offsets, vertices, texCoords, normals, indices);
	}

	void Mesh::mainThreadInitialization()
	{
		// Generate the VAO buffer
		generateVAO();
	}
}