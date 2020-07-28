#include "Model.h"
#include "glad.h"
#include "glfw3.h"
namespace Engine
{
    Model::Model(const std::string& modelPath, const std::string& modelName,bool flipUVs):Asset(modelPath,modelName)
    {
        m_flipUvs = flipUVs;
        m_numMeshes = 0;
        m_meshes = nullptr;
    }

    Model::~Model()
    {
    }

    void Model::loadFile(float scaleFactor) {

        log_printf(log_level_e::LOG_INFO, "Loading Model: %s", fileNameAndExtension().c_str());

        float iniTime = glfwGetTime();

        if (importer)//delete importer if it already has one
            realeseLoadingResources();

        importer = new Assimp::Importer();

        if (m_flipUvs)
            m_scene = importer->ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_FlipUVs | aiProcess_GenNormals);
        else
            m_scene = importer->ReadFile(filePath, aiProcess_Triangulate | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

        if (!m_scene || m_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !m_scene->mRootNode) // if is Not Zero
        {
            log_printf(log_level_e::LOG_INFO, "ERROR::ASSIMP:: %s", importer->GetErrorString());
            return;
        }

        float finTime = glfwGetTime();
        log_printf(log_level_e::LOG_DEBUG, "    Assimp Load time: %f", (finTime - iniTime));

        m_numMeshes = m_scene->mNumMeshes;

        m_meshes = new Mesh[m_numMeshes];

        for (int j = 0; j < m_numMeshes; j++) {

            // data to fill
            std::vector<Vertex> vertices;
            std::vector<unsigned int> indices;

            aiMesh* mesh = m_scene->mMeshes[j];
            
            vertices.reserve(mesh->mNumVertices);//Allocate space for all the vertices to avoid reallocation
            indices.reserve(mesh->mNumFaces * 3u);

            m_totalVertices += mesh->mNumVertices;
            m_totalTriangles += mesh->mNumFaces;


            // walk through each of the mesh's vertices
            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Vertex vertex;
                glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
                
                // positions
                vector.x = mesh->mVertices[i].x;
                vector.y = mesh->mVertices[i].y;
                vector.z = mesh->mVertices[i].z;
                vertex.Position = vector * scaleFactor;

                //mesh bounding box
                m_meshes[j].bBox.min = glm::min(m_meshes[j].bBox.min, vector);
                m_meshes[j].bBox.max = glm::min(m_meshes[j].bBox.max, vector);

                //Model bounding box
                bBox.min = glm::min(bBox.min, vector);
                bBox.max = glm::min(bBox.max, vector);

                // normals
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                vertex.Normal = vector;

                // texture coordinates
                if (mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
                {
                    glm::vec2 vec;
                    // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                    // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                }
                else {
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);
                }

                // tangent
                if (mesh->mTangents) {
                    vector.x = mesh->mTangents[i].x;
                    vector.y = mesh->mTangents[i].y;
                    vector.z = mesh->mTangents[i].z;
                    vertex.Tangent = vector;
                }

                // bitangent
                if (mesh->mBitangents) {
                    vector.x = mesh->mBitangents[i].x;
                    vector.y = mesh->mBitangents[i].y;
                    vector.z = mesh->mBitangents[i].z;
                    vertex.Bitangent = vector;
                }

                vertices.push_back(vertex);
            }

            //Calculate bounding box center
            m_meshes[j].bBox.center = (m_meshes[j].bBox.min + m_meshes[j].bBox.max) / 2.f;

            if (!mesh->mTextureCoords[0])
                log_printf(log_level_e::LOG_INFO, "MESSAGE::MODEL:: TextureCoords not loaded:    Mesh %i     Model %s", j, fileName().c_str());

            //Triangles
            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                // retrieve all indices of the face and store them in the indices vector
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            //set Mesh and material index
            m_meshes[j] = Mesh(vertices.data(), vertices.size(), indices.data(), indices.size());
        }

        bBox.center = (bBox.min + bBox.max) / 2.f;

        finTime = glfwGetTime();
        log_printf(log_level_e::LOG_DEBUG, "    Mesh Load time: %f", finTime - iniTime);
        log_printf(log_level_e::LOG_DEBUG, "    Num Meshes: %i", m_numMeshes);
        log_printf(log_level_e::LOG_DEBUG, "    Total Vertices: %i   Total Triangles: %i\n", m_totalVertices, m_totalTriangles);
    
    }

}


