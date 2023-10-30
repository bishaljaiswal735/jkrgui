#pragma once
#include "Instance.hpp"
#include <glm/glm.hpp>

namespace Jkr {
	struct Vertex
	{
		glm::vec3 mPosition;
		glm::vec2 mTextureCorrdinate;
	};
	struct Quad
	{
		std::array<Vertex, 4> mVertices;
		std::array<uint32_t, 6> mIndex;
	};
}

namespace Jkr {
	class GUIPrimitive
	{
	public:
		GUIPrimitive(const Instance& inInstance);
		GUIPrimitive(const Instance& inInstance, const vk::ArrayProxyNoTemporaries<kstd::Vertex>& inVertices, const vk::ArrayProxyNoTemporaries<kstd::ui32>& inIndices);
		GUIPrimitive(const Instance& inInstance, const vk::ArrayProxyNoTemporaries<kstd::VertexEXT>& inVertices, const vk::ArrayProxyNoTemporaries<kstd::ui32>& inIndices, int inExtParameter_DoesNotDoAnything);
		GUIPrimitive(const Instance& inInstance, vk::DeviceSize inVertexBufferSizeInBytes, vk::DeviceSize inIndexBufferSizeInBytes);
		~GUIPrimitive() = default;
	public:
		GETTER& GetVertexBufferPtr() const { return mVertexBufferPtr; }
		GETTER& GetIndexBufferPtr() const { return mIndexBufferPtr; }
		GETTER GetIndexCount() const { return mIndexCount; }
		void SetIndexCount(uint32_t inIndex) {  mIndexCount = inIndex; }
	private:
		const Instance& mInstance;
		uint32_t mIndexCount = 0;
		Up<VulkanBufferVMA<BufferContext::Vertex, MemoryType::DeviceLocal>> mVertexBufferPtr = nullptr;
		Up<VulkanBufferVMA<BufferContext::Index, MemoryType::DeviceLocal>> mIndexBufferPtr = nullptr;
	};
}
