#include "renderer.h"
#include <iostream>

Renderer::Renderer()
	:m_modelShader(nullptr), m_skyboxShader(nullptr)
{
	init();
}

void Renderer::setupShaders()
{
	m_modelShader = std::make_shared<Shader>(MODEL_VERTEX_SHADER_SRC, MODEL_FRAGMENT_SHADER_SRC);
	m_skyboxShader = std::make_shared<Shader>(SKYBOX_VERTEX_SHADER_SRC, SKYBOXL_FRAGMENT_SHADER_SRC);
	m_sunShader = std::make_shared<Shader>(SUN_VERTEX_SHADER_SRC, SUN_FRAGMENT_SHADER_SRC);
}

unsigned Renderer::init()
{
	RenderCommands::SetClearColor(glm::vec4(0.5f, 0.1f, 0.1f, 1.0f));
	setupShaders();
	RenderCommands::SetSolidMode();

	return 0;
}

unsigned Renderer::shutdown()
{
	m_modelShader->~Shader();
	return 0;
}

void Renderer::draw(glm::mat4 viewProjectionMat, glm::vec3 cameraPos, PointLight light, std::shared_ptr<VertexArray>  modelData, glm::mat4 modelMat, std::vector<Material> modelMaterials, std::vector<DrawCall> modelDrawCalls, std::shared_ptr<VertexArray>  skyboxData, glm::mat4 skyboxMat, std::shared_ptr<VertexArray>  sunData, glm::mat4 sunMat, bool useCelShading, int celSteps, bool useDebugNormals)
{
	RenderCommands::Clear();

	uploadCommonUniforms(viewProjectionMat, light, cameraPos, modelMat, skyboxMat, sunMat, useCelShading, celSteps, useDebugNormals);

	renderSkybox(skyboxData);

	if (modelData != nullptr)
	{
		m_modelShader->Bind();
		modelData->Bind();
		modelData->GetIndexBuffer()->Bind();
		for (const DrawCall& dc : modelDrawCalls)
		{
			Material& mat = modelMaterials[dc.materialID];

			m_modelShader->UploadUniformFloat3("u_MaterialDiffuse", mat.diffuse);
			m_modelShader->UploadUniformFloat3("u_MaterialSpecular", mat.specular);
			m_modelShader->UploadUniformFloat1("u_MaterialShininess", mat.shininess);
			m_modelShader->UploadUniformFloat3("u_MaterialAmbient", mat.ambient);

			if (mat.diffuseTex != 0)
			{
				glActiveTexture(GL_TEXTURE0);
				glBindTexture(GL_TEXTURE_2D, mat.diffuseTex);
				m_modelShader->UploadUniformInt1("u_DiffuseTex", 0);
				m_modelShader->UploadUniformInt1("u_HasTexture", true);
			}
			else
			{
				m_modelShader->UploadUniformInt1("u_HasTexture", false);
			}

			glDrawElements(
				GL_TRIANGLES,
				dc.indexCount,
				GL_UNSIGNED_INT,
				(void*)(dc.indexOffset * sizeof(unsigned int))
			);
		}
		m_modelShader->Unbind();
	}


	m_sunShader->Bind();
	sunData->Bind();
	sunData->GetIndexBuffer()->Bind();
	// Uploading unifrom values for the border

	RenderCommands::DrawIndex(sunData, GL_TRIANGLES);

	sunData->Unbind();

	m_sunShader->Unbind();
}

void Renderer::renderSkybox(std::shared_ptr<VertexArray> skyboxData)
{
	m_skyboxShader->Bind();
	skyboxData->Bind();
	skyboxData->GetIndexBuffer()->Bind();
	// Uploading unifrom values for the border

	RenderCommands::DrawIndex(skyboxData, GL_TRIANGLES);

	skyboxData->Unbind();

	m_skyboxShader->Unbind();
}

void Renderer::uploadCommonUniforms(glm::mat4 viewProjectionMatrix, PointLight light, glm::vec3 viewPos, glm::mat4 modelMat, glm::mat4 skyboxMat, glm::mat4 sunMat, bool useCelShading, int celSteps, bool useDebugNormals)
{
	m_modelShader->Bind();
	m_modelShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
	m_modelShader->UploadUniformFloat3("u_LightColor", light.Color);
	m_modelShader->UploadUniformFloat3("u_LightPos", light.Position);
	m_modelShader->UploadUniformFloat1("u_LightRange", light.Radius * light.LightRadius);
	m_modelShader->UploadUniformFloat3("u_ViewPosition", viewPos);
	m_modelShader->UploadUniformMat4("u_Model", modelMat);
	m_modelShader->UploadUniformInt1("u_UseDebug", useDebugNormals);
	m_modelShader->Unbind();

	m_skyboxShader->Bind();
	m_skyboxShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
	m_skyboxShader->UploadUniformMat4("u_Model", skyboxMat);

	m_sunShader->Bind();
	m_sunShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
	m_sunShader->UploadUniformMat4("u_Model", sunMat);
}

