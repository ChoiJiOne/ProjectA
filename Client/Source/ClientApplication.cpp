#include "ClientApplication.h"

ClientApplication::~ClientApplication()
{
	Shutdown();
}

void ClientApplication::Setup()
{
	IGameFramework::Setup();
	clientPath_ = rootPath_ + L"Client/";

	auto clientResizeEvent = [&]() {
		int32_t width;
		int32_t height;
		window_->GetSize(width, height);
		glViewport(0, 0, width, height);
	};

	InputManager::Get().AddWindowEventAction("ClientResizeEvent",   EWindowEvent::Resize,       clientResizeEvent, true);
	InputManager::Get().AddWindowEventAction("ClientExitMinimize",  EWindowEvent::ExitMinimize, clientResizeEvent, true);
	InputManager::Get().AddWindowEventAction("ClientEnterMaximize", EWindowEvent::EnterMaximize,clientResizeEvent, true);
	InputManager::Get().AddWindowEventAction("ClientExitMaximize",  EWindowEvent::ExitMaximize, clientResizeEvent, true);
}

void ClientApplication::Shutdown()
{
}

void ClientApplication::Run()
{
	Vector3f cameraPosition = Vector3f(0.0f, 10.0f, 10.0f);

	Shader* shadowDepth = ResourceManager::Get().CreateResource<Shader>("shadowDepth");
	shadowDepth->Initialize(clientPath_ + L"Shader/ShadowDepth.vert", clientPath_ + L"Shader/ShadowDepth.frag");

	Shader* debugQuad = ResourceManager::Get().CreateResource<Shader>("debugQuad");
	debugQuad->Initialize(clientPath_ + L"Shader/DebugQuad.vert", clientPath_ + L"Shader/DebugQuad.frag");

	Shader* shadowMapping = ResourceManager::Get().CreateResource<Shader>("shadowMapping");
	shadowMapping->Initialize(clientPath_ + L"Shader/ShadowMapping.vert", clientPath_ + L"Shader/ShadowMapping.frag");

	std::vector<StaticMesh::Vertex> vertices = 
	{
		StaticMesh::Vertex(Vector3f(-1.0f, -1.0f, +0.0f), Vector3f(), Vector2f(0.0f, 0.0f)),
		StaticMesh::Vertex(Vector3f(+1.0f, -1.0f, +0.0f), Vector3f(), Vector2f(1.0f, 0.0f)),
		StaticMesh::Vertex(Vector3f(+1.0f, +1.0f, +0.0f), Vector3f(), Vector2f(1.0f, 1.0f)),
		StaticMesh::Vertex(Vector3f(-1.0f, +1.0f, +0.0f), Vector3f(), Vector2f(0.0f, 1.0f)),
	};
	std::vector<uint32_t> indices = 
	{
		0, 1, 2,
		0, 2, 3,
	};
	
	StaticMesh* quad = ResourceManager::Get().CreateResource<StaticMesh>("quad");
	quad->Initialize(vertices, indices);

	GeometryGenerator::CreateCube(Vector3f(10.0f, 1.0f, 10.0f), vertices, indices);
	StaticMesh* floor = ResourceManager::Get().CreateResource<StaticMesh>("floor");
	floor->Initialize(vertices, indices);

	GeometryGenerator::CreateCube(Vector3f(2.0f, 2.0f, 2.0f), vertices, indices);
	StaticMesh* cube = ResourceManager::Get().CreateResource<StaticMesh>("cube");
	cube->Initialize(vertices, indices);

	Material materia(Vector3f(0.329412f, 0.223529f, 0.027451f), Vector3f(0.780392f, 0.568627f, 0.113725f), Vector3f(0.992157f, 0.941176f, 0.807843f), 128.0f * 0.21794872f);

	const uint32_t shadowWidth = 1024;
	const uint32_t shadowHeight = 1024;
	
	uint32_t depthMapFBO;
	glGenFramebuffers(1, &depthMapFBO);

	uint32_t depthMap;
	glGenTextures(1, &depthMap);
	glBindTexture(GL_TEXTURE_2D, depthMap);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, shadowWidth, shadowHeight, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	Vector3f lightPosition = Vector3f(1.0f, 10.0f, 1.0f);
	Vector3f lightDirection = MathUtils::Normalize(-lightPosition);

	timer_.Reset();
	while (!bIsDoneLoop_)
	{
		timer_.Tick();
		InputManager::Get().Tick();

		float nearPlane = 1.0f;
		float farPlane = 100.0f;
		Matrix4x4f lightView = MathUtils::CreateLookAt(lightPosition, Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
		Matrix4x4f lightProjection = MathUtils::CreateOrtho(-10.0f, 10.0f, -1.0f, 10.0f, nearPlane, farPlane);

		shadowDepth->Bind();
		shadowDepth->SetUniform("lightView", lightView);
		shadowDepth->SetUniform("lightProjection", lightProjection);

		glViewport(0, 0, shadowWidth, shadowHeight);
		glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
		glClear(GL_DEPTH_BUFFER_BIT);

		shadowDepth->SetUniform("world", MathUtils::CreateTranslation(0.0f, -1.0f, 0.0f));
		glBindVertexArray(floor->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, floor->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		shadowDepth->SetUniform("world", MathUtils::CreateTranslation(-3.0f, +3.0f, 0.0));
		glBindVertexArray(cube->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, cube->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		shadowDepth->SetUniform("world", MathUtils::CreateTranslation(+3.0f, +3.0f, 1.0));
		glBindVertexArray(cube->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, cube->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		glBindFramebuffer(GL_FRAMEBUFFER, 0);
		shadowDepth->Unbind();

		RenderManager::Get().SetWindowViewport();
		RenderManager::Get().BeginFrame(0.0f, 0.0f, 0.0f, 1.0f);

		Matrix4x4f view = MathUtils::CreateLookAt(cameraPosition, Vector3f(0.0f, 0.0f, 0.0f), Vector3f(0.0f, 1.0f, 0.0f));
		Matrix4x4f projection = MathUtils::CreatePerspective(MathUtils::ToRadian(45.0f), window_->GetAspectSize(), 0.01f, 1000.0f);

		shadowMapping->Bind();
		shadowMapping->SetUniform("view", view);
		shadowMapping->SetUniform("projection", projection);
		shadowMapping->SetUniform("lightView", lightView);
		shadowMapping->SetUniform("lightProjection", lightProjection);
		shadowMapping->SetUniform("viewPosition", cameraPosition);
		shadowMapping->SetUniform("directionalLight.position", lightPosition);
		shadowMapping->SetUniform("directionalLight.direction", lightDirection);
		shadowMapping->SetUniform("directionalLight.ambientRGB", Vector3f(0.2f, 0.2f, 0.2f));
		shadowMapping->SetUniform("directionalLight.diffuseRGB", Vector3f(0.5f, 0.5f, 0.5f));
		shadowMapping->SetUniform("directionalLight.specularRGB", Vector3f(1.0f, 1.0f, 1.0f));
		shadowMapping->SetUniform("material.ambientRGB", materia.GetAmbientRGB());
		shadowMapping->SetUniform("material.diffuseRGB", materia.GetDiffuseRGB());
		shadowMapping->SetUniform("material.specularRGB", materia.GetSpecularRGB());
		shadowMapping->SetUniform("material.shininess", materia.GetShininess());

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, depthMap);

		shadowMapping->SetUniform("world", MathUtils::CreateTranslation(0.0f, -1.0f, 0.0f));
		glBindVertexArray(floor->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, floor->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		shadowMapping->SetUniform("world", MathUtils::CreateTranslation(-3.0f, +3.0f, 0.0));
		glBindVertexArray(cube->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, cube->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);
		
		shadowMapping->SetUniform("world", MathUtils::CreateTranslation(+3.0f, +3.0f, 1.0));
		glBindVertexArray(cube->GetVertexArrayObject());
		glDrawElements(GL_TRIANGLES, cube->GetIndexCount(), GL_UNSIGNED_INT, 0);
		glBindVertexArray(0);

		shadowMapping->Unbind();

		RenderManager::Get().RenderGrid3D(view, projection, -10.0f, 10.0f, 1.0f, -10.0f, 10.0f, 1.0f, Vector4f(1.0f, 1.0f, 1.0f, 1.0f));
		RenderManager::Get().EndFrame();
	}

	glDeleteTextures(1, &depthMap);
	glDeleteFramebuffers(1, &depthMapFBO);
}