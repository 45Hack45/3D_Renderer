#include "Scene.h"

#include "ShaderManager.hpp"
#include "MaterialManager.hpp"

namespace Engine
{

	Entity* Scene::createEntity()
	{
		Entity* entity = m_scene.addChild();
		entities.push_back(entity);
		return entity;
	}
	Entity* Scene::createEntity(Entity* parent)
	{
		Entity* entity = createEntity();
		entity->setParent(parent);
		entity->transform.parent = &entity->m_parent->transform;
		return entity;
	}

	void Scene::loadModelMaterials(Model* model) {
		for (size_t i = 0; i < model->m_scene->mNumMaterials; i++)
		{
			bool usePhong = false;

			aiMaterial* aMat = model->m_scene->mMaterials[i];
			Material* mat;

			float shinines;
			aiColor3D col;
			aiString path;//diffuse texture relative path

			if (aMat->Get(AI_MATKEY_SHININESS, shinines) == aiReturn::aiReturn_SUCCESS)//Chek if it has specular exponent
			{
				if (shinines != 0) {
					usePhong = true;
					mat = new	Material(ShaderManager::Instance()->getShader("PhongShader"));
				}
				else
				{
					usePhong = true;
					mat = new	Material(ShaderManager::Instance()->getShader("PhongShader"));
				}
			}
			else {
				usePhong = true;
				mat = new	Material(ShaderManager::Instance()->getShader("PhongShader"));
			}


			/*if (usePhong)
			{
				mat->setFloat("shinines", shinines);
				if (aMat->Get(AI_MATKEY_SHININESS_STRENGTH, shinines) == aiReturn::aiReturn_SUCCESS)
					mat->setFloat("specularIntensity", shinines);
				else
				{
					log_printf_info("WARNING::ENGINE::LOAD_MODEL_MATERIAL: failed to get material specular shinines.	 Model: %s\n", model->assetName.c_str());
				}
			}

			if (aMat->Get(AI_MATKEY_COLOR_DIFFUSE, col) == aiReturn::aiReturn_SUCCESS)
				mat->setVec4("myColor", col.r, col.g, col.b, 1.0f);
			else
			{
				log_printf_info("WARNING::ENGINE::LOAD_MODEL_MATERIAL: failed to get material color.	 Model: %s\n", model->assetName.c_str());
			}*/

			mat->setFloat("shinines", 255);
			mat->setFloat("specularIntensity", 1);
			mat->setVec4("myColor", glm::vec4(1.f));


			if (aMat->GetTextureCount(aiTextureType::aiTextureType_DIFFUSE) > 0)
				if (aMat->GetTexture(aiTextureType::aiTextureType_DIFFUSE, 0, &path) == aiReturn::aiReturn_SUCCESS) {
					std::string textureName = path.C_Str();

					textureName = textureName.substr(0, textureName.find_last_of('.'));//Remove extension

					//log_printf_info("Material[%i] albedoPath: %s", i, textureName.c_str());
					mat->setTexture2D("albedoTexture", textureName);
				}
				else
				{
					log_printf_info("WARNING::ENGINE::LOAD_MODEL_MATERIAL: failed to get material texture.	 Model: %s\n", model->assetName.c_str());
				}

			int blend;
			if (aMat->Get(AI_MATKEY_BLEND_FUNC, blend) == aiReturn::aiReturn_SUCCESS)
				mat->isTransparent = true;

			std::string name = model->assetName + "_Material_";
			name += std::to_string(i);

			mat->name = name.c_str();

			MaterialManager::Instance()->AddMaterial(mat);
		}
	}

	Entity* Scene::createModelEntityMeshes(Model* model, aiNode* node, Entity* entity) {

		if (node->mNumMeshes <= 0)
			return NULL;

		Entity* t = createEntity(entity);

		//Set first mesh
		Mesh* mesh = &model->m_meshes[node->mMeshes[0]];

		aiMesh* amesh = model->m_scene->mMeshes[node->mMeshes[0]];
		std::string matName = model->assetName + "_Material_";
		matName += std::to_string(amesh->mMaterialIndex);


		Material* material = MaterialManager::Instance()->getMaterial(matName);

		std::string name = std::string(entity->name);
		if (node->mName.C_Str() == "") {
			name = std::string(entity->name);
			name += "_";
			name += std::to_string(0);
			t->name = name;
		}
		else {
			t->name = node->mName.C_Str();
		}

		if (material == nullptr)
			log_error("WARNING::ENGINE::FAILED_TO_CREATE_ENTITY_MESH: material not found");

		t->meshRenderer.Init(mesh, material);

		for (size_t i = 1; i < node->mNumMeshes; i++)//set other meshes
		{
			t = createEntity(entity);

			if (node->mName.C_Str() == "") {
				name = std::string(entity->name);
				name += "_";
				name += std::to_string(i);
				t->name = name;
			}
			else {
				name = node->mName.C_Str();
				name += "_";
				name += std::to_string(i);
				t->name = name.c_str();
			}

			Mesh* mesh = &model->m_meshes[node->mMeshes[i]];

			aiMesh* amesh = model->m_scene->mMeshes[node->mMeshes[i]];
			std::string matName = model->assetName + "_Material_";
			matName += std::to_string(amesh->mMaterialIndex);

			Material* material = MaterialManager::Instance()->getMaterial(matName);

			if (material == nullptr)
				log_error("WARNING::ENGINE::FAILED_TO_CREATE_ENTITY_MESH: material not found");

			t->meshRenderer.Init(mesh, material);
		}

		return t;
	}

	void Scene::createModelEntity(Model* model, aiNode* parentNode, Entity* parent) {
		for (size_t i = 0; i < parentNode->mNumChildren; i++)
		{
			aiNode* node = parentNode->mChildren[i];
			/*Entity* entity = createEntity(parent);

			std::string name =
			name += "_";
			name += std::to_string(i);

			entity->name = name.c_str();

			entity->transform.meshScaleFactor = .001f;*/

			Entity* entity = createModelEntityMeshes(model, node, parent);

			if (entity) {
				createModelEntity(model, node, entity);
			}
		}
	}

	Entity* Scene::loadModel2Scene(Model* model)
	{
		loadModelMaterials(model);

		aiNode* nRoot = model->m_scene->mRootNode;

		Entity* eRoot = createEntity();

		eRoot->name = nRoot->mName.C_Str();

		createModelEntityMeshes(model, nRoot, eRoot);//generate root meshes

		createModelEntity(model, nRoot, eRoot);//generate childs meshes

		return eRoot;
	}

}
