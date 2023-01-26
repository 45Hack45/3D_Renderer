#include "VolumetricCloud.h"
#include "ModelManager.hpp"
#include "ShaderManager.hpp"

//Unit cube
float uCubeVertices[] = {
	//pos
	0,0,0,	//front-bottom-left		0
	1,0,0,	//front-bottom-right	1
	1,1,0,	//front-top-right		2
	0,1,0,	//front-top-left		3
	0,0,1,	//back-bottom-left		4
	1,0,1,	//back-bottom-right		5
	1,1,1,	//back-top-right		6
	0,1,1	//back-top-left			7
};

unsigned int uCubeTriangles[] = {
	0,2,3, //front-0
	0,1,2, //front-1
	4,0,7, //left-0
	0,3,7, //left-1
	1,6,2, //right-0
	1,5,6, //right-1
	5,7,6, //back-0
	5,4,7, //back-1
	4,1,0, //bottom-0
	4,5,1, //bottom-1
	3,6,7, //Top-0
	3,2,6  //Top-1
};

static float quadVertices[] = {
	//Position		//UV
	 1,  1, 0.0f,	1, 1,	// top right
	 1, -1, 0.0f,	1, 0,	// bottom right
	-1, -1, 0.0f,	0, 0,	// bottom left
	-1,  1, 0.0f,	0, 1	// top left 
};
static unsigned int çquadIndices[] = {  // note that we start from 0!
	2,0,3,
	2,1,0
};

//-------Unit Cube
static unsigned int cube_VAO, cube_VBO, cube_EBO;

namespace Engine {
	VolumetricCloud::VolumetricCloud(int Width, int Heigth, int Depth)
	{
		width = Width;
		heigth = Heigth;
		depth = Depth;
		samples = width * heigth * depth;

		shader = ShaderManager::Instance()->getShader("VolumetricCloudRender");
		cube_model = ModelManager::Instance()->getModel("Cube");
		cube_model->loadFile();

		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_3D, texture);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_REPEAT);
		glBindTexture(GL_TEXTURE_3D, 0);

		// create unit cube  buffers
#pragma region Unit Cube Render Buffers
		static bool cube_Exist = false;

		if (!cube_Exist) {
			glGenVertexArrays(1, &cube_VAO);
			glGenBuffers(1, &cube_VBO);
			glGenBuffers(1, &cube_EBO);

			glBindVertexArray(cube_VAO);

			// load data into vertex buffers
			glBindBuffer(GL_ARRAY_BUFFER, cube_VBO);

			glBufferData(GL_ARRAY_BUFFER, sizeof(uCubeVertices), uCubeVertices, GL_STATIC_DRAW);


			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cube_EBO);

			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uCubeTriangles), uCubeTriangles, GL_STATIC_DRAW);

			// set the vertex attribute pointers

			// vertex Positions
			glEnableVertexAttribArray(0);
			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);

			glBindVertexArray(0);//unbind VAO

			cube_Exist = true;
		}
#pragma endregion
	}

	void VolumetricCloud::GenerateClouds(bool useGPU)
	{

		if (volumePoints != nullptr) delete[] volumePoints;//Clear if it was already set

		volumePoints = new glm::vec4[samples];

		glEnable(GL_TEXTURE_3D);

		//Calculate values
		if (useGPU) {
			
		}
		else {
			int lPos = 0;//Position in array.
			float* minDistance = new float[4]{ float(samples),float(samples),float(samples),float(samples) };//Min distance of every chanel, Initilizing to a big number
			int voronoiPoints_count = voronoiChanelPoints.x + voronoiChanelPoints.y + voronoiChanelPoints.z + voronoiChanelPoints.w;

			float invMaxDistance = 1/ sqrtf(sqrtf(width * width + depth * depth) + heigth * heigth);

			for (int i = 0; i < width; i++)
			{
				for (int j = 0; j < heigth; j++)
				{
					for (int k = 0; k < depth; k++)
					{
						glm::vec3 position = glm::vec3(i, j, k);

						for (int c = 0; c < voronoiPoints_count; c++) {
							minDistance[voronoiPointsChanels[c]] =
								std::min(
									minDistance[voronoiPointsChanels[c]],
									glm::distance(position, voronoiPoints[c])
								);
						}

						volumePoints[lPos] = glm::vec4(minDistance[0], minDistance[1], minDistance[2], minDistance[3]) * invMaxDistance;
						volumePoints[lPos] = glm::vec4(1) - volumePoints[lPos];
						volumePoints[lPos] = glm::smoothstep(glm::vec4(cutOff), glm::vec4(cutOff + cutOffSmothnes), volumePoints[lPos]);
						
						float heightFade = (j / float(heigth) > heightCutoff) ? 1 : 0;

						volumePoints[lPos] = volumePoints[lPos] * heightFade;

						delete[] minDistance;
						minDistance = new float[4]{ float(samples),float(samples),float(samples),float(samples) };//Reset array

						lPos++;
					}
				}
			}

			delete[] minDistance;
			
			glBindTexture(GL_TEXTURE_3D, texture);//Bind texture to texture unit 0

			glTexImage3D(GL_TEXTURE_3D, 0, GL_RGB, width, heigth, depth, 0, GL_RGBA, GL_FLOAT, volumePoints);//Create texture
		}

		glBindTexture(GL_TEXTURE_3D, 0);

		glDisable(GL_TEXTURE_3D);
	}

	void VolumetricCloud::Update()
	{
		//glEnable(GL_TEXTURE_3D);



		//glDisable(GL_TEXTURE_3D);
	}

	void VolumetricCloud::Draw(Camera* cam)
	{
		glEnable(GL_TEXTURE_3D);

		shader->bind();
		shader->setMat4("viewProjection", cam->GetViewProjectionMatrix());
		shader->setMat4("model", transform.globalSpace());
		shader->setVector("cameraPosition", glm::vec4(cam->Position, 1.f) * glm::inverse(transform.globalSpace()));
		shader->setVector("vResolution", glm::vec3(width, heigth, depth));

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_3D, texture);//Bind texture to texture unit 0

		//cube_model->Draw();

		glEnable(GL_BLEND);

 		glBindVertexArray(cube_VAO);
								     //  #tringles
		glDrawElements(GL_TRIANGLES, sizeof(uCubeTriangles)/3, GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glDisable(GL_BLEND);

		glDisable(GL_TEXTURE_3D);
	}

	void VolumetricCloud::GenerateVoronoiPoints()
	{
		int vPoints = voronoiChanelPoints.x + voronoiChanelPoints.y + voronoiChanelPoints.z + voronoiChanelPoints.w;

		if (voronoiPoints != nullptr) delete[] voronoiPoints;// Clear if it was already set
		if (voronoiPointsChanels != nullptr) delete[] voronoiPointsChanels;

		if (vPoints == 0) {
			voronoiChanelPoints.r = 1;

			voronoiPoints = new	glm::vec3[1];
			voronoiPointsChanels = new int[1];

			voronoiPoints[0] = glm::vec3(1, 1, 1);

			//Seting point chanel
			voronoiPointsChanels[0] = 0;
			return;
		}

		voronoiPoints = new	glm::vec3[vPoints];
		voronoiPointsChanels = new int[vPoints];

		for (int i = 0; i < vPoints; i++)
		{
			voronoiPoints[i].x = random(0, width);
			voronoiPoints[i].y = random(0, heigth);
			voronoiPoints[i].z = random(0, depth);

			//Seting point chanel
			if (i < voronoiChanelPoints.x) {
				voronoiPointsChanels[i] = 0;
			}
			else if (i < voronoiChanelPoints.y) {
				voronoiPointsChanels[i] = 1;
			}
			else if (i < voronoiChanelPoints.z) {
				voronoiPointsChanels[i] = 2;
			}
			else{
				voronoiPointsChanels[i] = 3;
			}
		}

		
	}
}
