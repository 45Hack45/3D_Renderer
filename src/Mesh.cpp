#include "Mesh.h"

namespace Engine
{
	Mesh::Mesh(Vertex* vertices, int numVertices, unsigned int* triangles, int numTriangles)
	{
		m_vertices = vertices;
		m_numVertices = numVertices;

		m_triangles = triangles;
		m_numTriangles = numTriangles;

		// create buffers/arrays
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        // load data into vertex buffers
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, m_numVertices * sizeof(Vertex), m_vertices, GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_numTriangles * sizeof(unsigned int), m_triangles, GL_STATIC_DRAW);


        // set the vertex attribute pointers
        
        // vertex Positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

        // vertex normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));

        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Tangent));

        // vertex bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Bitangent));

        glBindVertexArray(0);//unbind VAO
	}

	Mesh::~Mesh()
	{

        //log_printf(log_level_e::LOG_INFO, "%i\n", m_vertices[100]);
        
      //  if(m_vertices)
		    //delete[] m_vertices;
		
      //  if(m_triangles)
      //      delete[] m_triangles;
	}

    void Mesh::Draw() {
        // draw mesh
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, m_numTriangles, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
}


