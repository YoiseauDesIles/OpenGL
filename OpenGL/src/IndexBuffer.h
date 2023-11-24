#pragma once

class IndexBuffer
{
private:
	unsigned int m_RendererId;
	unsigned int m_count;

public:
	IndexBuffer(const unsigned int* data, unsigned int count);
	IndexBuffer(const unsigned* data, unsigned count, bool dynamicMode);
	~IndexBuffer();

	void bind() const;
	void unbind() const;

	inline unsigned int GetCount() const { return m_count;  }
	inline void setCount(int count) { m_count = count;  }

};

