#include "VertexArray.h"
#include <Shader.cpp>

VertexArray::VertexArray()
{
	glGenVertexArrays(1, &m_vertexArrayID);
}

VertexArray::~VertexArray()
{
	glDeleteVertexArrays(1, &m_vertexArrayID);
}

void VertexArray::Bind() const
{
	glBindVertexArray(m_vertexArrayID);
}

void VertexArray::Unbind() const
{
	glBindVertexArray(0);
}

// Adds new VertexBuffer and sets the attribute pointer for the BufferAttributes
void VertexArray::AddNormalVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer)
{
	VertexBuffers.push_back(vertexBuffer);
	vertexBuffer->Bind();
	SetVertexAttributePointer(VertexBuffers.size() - 1);
}

// Adds new VertexBuffer and sets the attribute pointer for the BufferAttributes
void VertexArray::AddPerInstanceVertexBuffer(const std::shared_ptr<VertexBuffer> vertexBuffer, GLuint everyXInstance)
{
	VertexBuffers.push_back(vertexBuffer);
	vertexBuffer->Bind();
	SetVertexAttributePointer(VertexBuffers.size() - 1);
	SetVertexAttributeDivisor(VertexBuffers.size() - 1, everyXInstance);
}



void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer> &indexBuffer)
{
	IdxBuffer = indexBuffer;
}

// Check if the index is for a valid buffer and enables it
void VertexArray::EnableVertexArray(GLuint i)
{
	int numberOfBuffers = 0;

	for (auto var : VertexBuffers)
	{
		for (auto atr : var->GetLayout().GetAttributes())
		{
			if (atr.Type == ShaderDataType::Mat4)
			{
				numberOfBuffers += 4;
			}
			else
			{
				numberOfBuffers++;
			}
		}
	}
	if (i >= 0 && i < numberOfBuffers)
	{
		glEnableVertexAttribArray(i);
	}
}

int VertexArray::GetNumVBOs()
{
	return VertexBuffers.size();
}


void VertexArray::SetupInstanceBuffer(const std::vector<float>& instanceData, int maxCapacity)
{
	Bind();

	// Calculate instance count (19 floats per instance: 3 color + 16 matrix)
	m_instanceCount = instanceData.size() / 19;

	// Set capacity (if not specified, use current count * 2 for growth)
	m_maxInstanceCapacity = (maxCapacity > 0) ? maxCapacity : m_instanceCount * 2;

	// Create the instance buffer
	// Note: We allocate maxCapacity size but only upload instanceData.size()
	size_t bufferSize = m_maxInstanceCapacity * 19 * sizeof(float);

	// Create VertexBuffer with max capacity
	m_instanceBuffer = std::make_shared<VertexBuffer>(nullptr, bufferSize, GL_DYNAMIC_DRAW);
	m_instanceBuffer->Bind();

	// Upload initial data if any
	if (!instanceData.empty()) {
		m_instanceBuffer->BufferSubData(0, instanceData.size() * sizeof(float), instanceData.data());
	}

	// Setup attribute 3: Color (vec3)
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float), (void*)0);
	glVertexAttribDivisor(3, 1);

	// Setup attributes 4-7: Model Matrix (mat4 = 4 x vec4)
	for (unsigned int i = 0; i < 4; i++) {
		GLuint attribIndex = 4 + i;
		glEnableVertexAttribArray(attribIndex);
		glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
			(void*)((3 + i * 4) * sizeof(float)));
		glVertexAttribDivisor(attribIndex, 1);
	}

	Unbind();
}

void VertexArray::AddInstances(const std::vector<glm::vec3>& colors, const std::vector<glm::mat4>& modelMatrices)
{
	if (!m_instanceBuffer) {
		std::cerr << "Instance buffer not initialized! Call SetupInstanceBuffer first." << std::endl;
		return;
	}

	if (colors.size() != modelMatrices.size()) {
		std::cerr << "Color and matrix count mismatch!" << std::endl;
		return;
	}

	size_t newInstanceCount = m_instanceCount + colors.size();

	// Check if we need to grow the buffer
	if (newInstanceCount > m_maxInstanceCapacity) {
		// Need to reallocate - double the capacity
		m_maxInstanceCapacity = newInstanceCount * 2;

		// Read back existing data
		std::vector<float> existingData(m_instanceCount * 19);
		m_instanceBuffer->Bind();
		glGetBufferSubData(GL_ARRAY_BUFFER, 0,
			m_instanceCount * 19 * sizeof(float),
			existingData.data());

		// Create new buffer with larger capacity
		size_t newBufferSize = m_maxInstanceCapacity * 19 * sizeof(float);
		GLuint newBufferID;
		glGenBuffers(1, &newBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, newBufferID);
		glBufferData(GL_ARRAY_BUFFER, newBufferSize, nullptr, GL_DYNAMIC_DRAW);

		// Copy old data
		glBufferSubData(GL_ARRAY_BUFFER, 0, existingData.size() * sizeof(float),
			existingData.data());

		// Delete old buffer and update
		GLuint oldID = m_instanceBuffer->GetID();
		glDeleteBuffers(1, &oldID);

		// Update the instance buffer with new ID
		m_instanceBuffer = std::make_shared<VertexBuffer>(nullptr, newBufferSize, GL_DYNAMIC_DRAW);

		// Re-setup attribute pointers with new buffer
		Bind();
		glBindBuffer(GL_ARRAY_BUFFER, newBufferID);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, 19 * sizeof(float), (void*)0);
		glVertexAttribDivisor(3, 1);

		for (unsigned int i = 0; i < 4; i++) {
			GLuint attribIndex = 4 + i;
			glEnableVertexAttribArray(attribIndex);
			glVertexAttribPointer(attribIndex, 4, GL_FLOAT, GL_FALSE, 19 * sizeof(float),
				(void*)((3 + i * 4) * sizeof(float)));
			glVertexAttribDivisor(attribIndex, 1);
		}

		Unbind();
	}

	// Prepare new instance data
	std::vector<float> newData;
	newData.reserve(colors.size() * 19);

	for (size_t i = 0; i < colors.size(); i++) {
		// Add color
		newData.push_back(colors[i].r);
		newData.push_back(colors[i].g);
		newData.push_back(colors[i].b);

		// Add matrix (column-major)
		const float* matPtr = glm::value_ptr(modelMatrices[i]);
		for (int j = 0; j < 16; j++) {
			newData.push_back(matPtr[j]);
		}
	}

	// Upload new data at the end of existing data
	m_instanceBuffer->Bind();
	m_instanceBuffer->BufferSubData(m_instanceCount * 19 * sizeof(float),
		newData.size() * sizeof(float),
		newData.data());

	m_instanceCount = newInstanceCount;
}

void VertexArray::UpdateInstanceData(const std::vector<float>& instanceData)
{
	if (!m_instanceBuffer) {
		std::cerr << "Instance buffer not initialized!" << std::endl;
		return;
	}

	m_instanceCount = instanceData.size() / 19;

	if (m_instanceCount > m_maxInstanceCapacity) {
		// Need to reallocate
		m_maxInstanceCapacity = m_instanceCount * 2;

		Bind();
		m_instanceBuffer->Bind();

		// Orphan old buffer and create new one
		glBufferData(GL_ARRAY_BUFFER,
			m_maxInstanceCapacity * 19 * sizeof(float),
			instanceData.data(),
			GL_DYNAMIC_DRAW);

		Unbind();
	}
	else {
		// Just update existing buffer
		m_instanceBuffer->Bind();
		m_instanceBuffer->BufferSubData(0, instanceData.size() * sizeof(float),
			instanceData.data());
	}
}

void VertexArray::SetVertexAttributePointer(GLuint bufferIndex)
{
	ASSERT(bufferIndex >= 0 && bufferIndex < VertexBuffers.size(), "Index to set Vertex Attribute Data is out of range.");

	BufferLayout layout = VertexBuffers[bufferIndex]->GetLayout();

	std::vector<BufferAttribute> attributes = layout.GetAttributes();
	// Sets the vertex attribute pointer for each BufferAttribute
	for (GLuint j = 0; j < attributes.size(); j++)
	{
		BufferAttribute attribute = attributes[j];

		if (attribute.Type == ShaderDataType::Mat4)
		{
			for (int i = 0; i < 4; i++)
			{
				GLuint index = m_indexOfAttributes;

				glEnableVertexAttribArray(index);
				glVertexAttribPointer(index, ShaderDataTypeComponentCount(ShaderDataType::Float4), ShaderDataTypeToOpenGLBaseType(ShaderDataType::Float4), attribute.Normalized, layout.GetStride(), reinterpret_cast<void*>(static_cast<uintptr_t>(attribute.Offset) * i));
			
				//Increase index of the whole VAO
				m_indexOfAttributes++;
			}
		}
		else
		{
			GLuint index = m_indexOfAttributes;

			glEnableVertexAttribArray(index);
			glVertexAttribPointer(index, ShaderDataTypeComponentCount(attribute.Type), ShaderDataTypeToOpenGLBaseType(attribute.Type), attribute.Normalized, layout.GetStride(), reinterpret_cast<void*>(static_cast<uintptr_t>(attribute.Offset)));
			GLint enabled;
			glGetVertexAttribiv(index, GL_VERTEX_ATTRIB_ARRAY_ENABLED, &enabled);
			
			//Increase index of the whole VAO
			m_indexOfAttributes++;
		}
	}
}

void VertexArray::SetVertexAttributeDivisor(GLuint bufferIndex, GLuint everyInstance)
{	
	ASSERT(bufferIndex >= 0 && bufferIndex < VertexBuffers.size(), "Index to set Vertex Attribute Data is out of range.");

	BufferLayout layout = VertexBuffers[bufferIndex]->GetLayout();

	std::vector<BufferAttribute> attributes = layout.GetAttributes();

	GLuint numAttributes = 0;

	for (auto attribute : attributes)
	{
		if (attribute.Type == ShaderDataType::Mat4)
		{
			numAttributes += 4;
		}
		else
		{
			numAttributes++;
		}
	}

	GLuint startValue = m_indexOfAttributes - numAttributes;


	// Sets the vertex attribute pointer for each BufferAttribute
	for (GLuint j = startValue; j < m_indexOfAttributes; j++)
	{
		glVertexAttribDivisor(j, everyInstance);
			// Check divisor
			GLint divisor;
		glGetVertexAttribiv(j, GL_VERTEX_ATTRIB_ARRAY_DIVISOR, &divisor);
	}
}