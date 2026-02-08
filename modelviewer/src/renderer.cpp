#include "renderer.h"
#include <iostream>

Renderer::Renderer()
	:m_tunnelShader(nullptr), m_skyboxShader(nullptr)
{
	init();
}

void Renderer::setupShaders()
{
	m_tunnelShader = std::make_shared<Shader>(TUNNEL_VERTEX_SHADER_SRC, TUNNEL_FRAGMENT_SHADER_SRC);
	m_skyboxShader = std::make_shared<Shader>(SKYBOX_VERTEX_SHADER_SRC, SKYBOXL_FRAGMENT_SHADER_SRC);
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
	m_tunnelShader->~Shader();
	return 0;
}

void Renderer::draw(glm::mat4 viewProjectionMat, glm::vec3 cameraPos, glm::vec3 lightPos, float lightMix, std::shared_ptr<VertexArray>  modelData, glm::mat4 modelMat, std::shared_ptr<VertexArray>  skyboxData, glm::mat4 skyboxMat)
{
	RenderCommands::Clear();

	uploadCommonUniforms(viewProjectionMat, lightPos, cameraPos, lightMix, modelMat, skyboxMat);

	renderSkybox(skyboxData);
	renderModel(modelData);

}

void Renderer::renderModel(std::shared_ptr<VertexArray> tunnelData)
{
	m_tunnelShader->Bind();
	tunnelData->Bind();
	tunnelData->GetIndexBuffer()->Bind();
	// Uploading unifrom values for the border

	RenderCommands::DrawIndex(tunnelData, GL_TRIANGLES);

	tunnelData->Unbind();

	m_tunnelShader->Unbind();
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

void Renderer::uploadCommonUniforms(glm::mat4 viewProjectionMatrix, glm::vec3 lightPos, glm::vec3 viewPos, float lightMix, glm::mat4 modelMat, glm::mat4 skyboxMat)
{
	m_tunnelShader->Bind();
	m_tunnelShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
	m_tunnelShader->UploadUniformFloat3("u_LightColorDay", m_LIGHT_COLOR_DAY);
	m_tunnelShader->UploadUniformFloat3("u_LightColorNight", m_LIGHT_COLOR_NIGHT);
	m_tunnelShader->UploadUniformFloat3("u_LightPos", lightPos);
	m_tunnelShader->UploadUniformFloat1("u_LightMix", lightMix);
	m_tunnelShader->UploadUniformFloat1("u_AmbientStrength", m_AMBIENT_STRENGTH);
	m_tunnelShader->UploadUniformFloat1("u_SpecularStrength", m_SPECULAR_STRENGTH);
	m_tunnelShader->UploadUniformFloat3("u_ViewPosition", viewPos);
	m_tunnelShader->UploadUniformMat4("u_Model", modelMat);
	m_tunnelShader->Unbind();

	m_skyboxShader->Bind();
	m_skyboxShader->UploadUniformMat4("u_ViewProjection", viewProjectionMatrix);
	m_skyboxShader->UploadUniformMat4("u_Model", skyboxMat);
}

