#include "Mesh.h"

#include "Assertion.h"
#include "GLAssertion.h"

#include <glad/glad.h>

Mesh::~Mesh()
{
	if (bIsInitialized_)
	{
		Release();
	}
}

void Mesh::Initialize(const std::vector<VertexPosition>& vertices, const std::vector<uint32_t>& indices)
{
	ASSERT(!bIsInitialized_, "already initialize mesh resource...");

	bHasVertexColor_ = false;
	bHasVertexTexture_ = false;
	bHasVertexNormal_ = false;

	indexCount_ = static_cast<uint32_t>(indices.size());

	const void* vertexBufferPtr = reinterpret_cast<const void*>(vertices.data());
	uint32_t vertexBufferSize = static_cast<uint32_t>(vertices.size()) * VertexPosition::GetStride();

	const void* indexBufferPtr = reinterpret_cast<const void*>(indices.data());
	uint32_t indexBufferSize = static_cast<uint32_t>(indices.size()) * sizeof(uint32_t);
	
	Initialize(vertexBufferPtr, vertexBufferSize, indexBufferPtr, indexBufferSize);

	GL_ASSERT(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexPosition::GetStride(), (void*)(offsetof(VertexPosition, position))), "failed to define an array of generic vertex attribute data");
	GL_ASSERT(glEnableVertexAttribArray(0), "failed to enable a generic vertex attribute array");

	GL_ASSERT(glBindVertexArray(0), "failed to unbind mesh vertex array object...");

	bIsInitialized_ = true;
}

void Mesh::Initialize(const std::vector<VertexPositionColor>& vertices, const std::vector<uint32_t>& indices)
{
	ASSERT(!bIsInitialized_, "already initialize mesh resource...");

	bHasVertexColor_ = false;
	bHasVertexTexture_ = false;
	bHasVertexNormal_ = false;

	indexCount_ = static_cast<uint32_t>(indices.size());

	const void* vertexBufferPtr = reinterpret_cast<const void*>(vertices.data());
	uint32_t vertexBufferSize = static_cast<uint32_t>(vertices.size()) * VertexPositionColor::GetStride();

	const void* indexBufferPtr = reinterpret_cast<const void*>(indices.data());
	uint32_t indexBufferSize = static_cast<uint32_t>(indices.size()) * sizeof(uint32_t);

	Initialize(vertexBufferPtr, vertexBufferSize, indexBufferPtr, indexBufferSize);

	GL_ASSERT(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, VertexPositionColor::GetStride(), (void*)(offsetof(VertexPositionColor, position))), "failed to define an array of generic vertex attribute data");
	GL_ASSERT(glEnableVertexAttribArray(0), "failed to enable a generic vertex attribute array");

	GL_ASSERT(glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, VertexPositionColor::GetStride(), (void*)(offsetof(VertexPositionColor, color))), "failed to define an array of generic vertex attribute data");
	GL_ASSERT(glEnableVertexAttribArray(1), "failed to enable a generic vertex attribute array");

	GL_ASSERT(glBindVertexArray(0), "failed to unbind mesh vertex array object...");

	bIsInitialized_ = true;
}

void Mesh::Release()
{
	ASSERT(bIsInitialized_, "not initialized before or has already been released...");

	GL_ASSERT(glDeleteBuffers(1, &indexBufferObject_), "failed to delete mesh index buffer...");
	GL_ASSERT(glDeleteBuffers(1, &vertexBufferObject_), "failed to delete mesh vertex buffer...");
	GL_ASSERT(glDeleteVertexArrays(1, &vertexArrayObject_), "failed to delete mesh vertex array object...");

	bIsInitialized_ = false;
}

void Mesh::Initialize(const void* vertexBufferPtr, uint32_t vertexBufferSize, const void* indexBufferPtr, uint32_t indexBufferSize)
{
	GL_ASSERT(glGenVertexArrays(1, &vertexArrayObject_), "failed to generate mesh vertex array object...");
	GL_ASSERT(glGenBuffers(1, &vertexBufferObject_), "failed to generate mesh vertex buffer...");
	GL_ASSERT(glGenBuffers(1, &indexBufferObject_), "failed to generate mesh index buffer...");

	GL_ASSERT(glBindVertexArray(vertexArrayObject_), "failed to bind mesh vertex array object...");
	GL_ASSERT(glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject_), "failed to bind mesh vertex buffer...");
	GL_ASSERT(glBufferData(GL_ARRAY_BUFFER, vertexBufferSize, vertexBufferPtr, GL_STATIC_DRAW), "failed to initialize mesh vertex buffer...");
	GL_ASSERT(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indexBufferObject_), "failed to bind mesh index buffer...");
	GL_ASSERT(glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexBufferSize, indexBufferPtr, GL_STATIC_DRAW), "failed to initialize mesh index buffer...");
}