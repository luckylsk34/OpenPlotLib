class VertexBuffer
{
private:
	unsigned int buffer_id;

public:
	VertexBuffer();
	VertexBuffer(const void *data, size_t size);
	~VertexBuffer();

	void bind() const;
	void unbind() const;
	void send_data(const void *data, size_t size) const;
};

// To be implemented later.
// class VariableVertexBuffer : public VertexBuffer
// {
// 	void **data;
// 	void *sizes;
// 	int num_data = 0;
// public:
// 	VariableVertexBuffer();
// 	~VariableVertexBuffer();

// 	void bind() const;
// 	void unbind() const;
// 	void add_data(const void* data, unsigned int size) const;
// 	void send_data() const;
// };
