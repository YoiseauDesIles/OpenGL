#pragma once

class VertexBuffer
{
private:
	unsigned int m_RendererId;

public:
	VertexBuffer(const void* data, unsigned int size);
	VertexBuffer(const void* data, unsigned size, bool dynamicRender);
	~VertexBuffer();

	void bind() const;
	void unbind() const;

};

