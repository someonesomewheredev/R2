#pragma once
#include <stdint.h>

#define VK_DEFINE_HANDLE(object) typedef struct object##_T* object;
VK_DEFINE_HANDLE(VkBuffer)
VK_DEFINE_HANDLE(VkCommandBuffer)
VK_DEFINE_HANDLE(VmaAllocation)
#undef VK_DEFINE_HANDLE

namespace R2::VK
{
    struct Handles;
    class Core;
    class CommandBuffer;

    enum class BufferUsage : uint32_t
    {
        Storage = 1,
        Uniform = 2,
        Index = 4,
        Vertex = 8,
        Indirect = 16
    };

    inline BufferUsage operator|(const BufferUsage& a, const BufferUsage& b)
    {
        return (BufferUsage)((uint32_t)a | (uint32_t)b);
    }

    enum class AccessFlags : uint64_t;
    enum class PipelineStageFlags : uint64_t;

    struct BufferCreateInfo
    {
        BufferUsage Usage;
        uint64_t Size;
        bool Mappable;
    };

    class Buffer
    {
    public:
        Buffer(Core* renderer, const BufferCreateInfo& createInfo);
        VkBuffer GetNativeHandle();
        void SetDebugName(const char* dbgName);

        uint64_t GetSize();
        BufferUsage GetUsage();
        void* Map();
        void Unmap();
        void CopyTo(VkCommandBuffer cb, Buffer* other, uint64_t numBytes, uint64_t srcOffset, uint64_t dstOffset);
        void Acquire(CommandBuffer cb, AccessFlags access);
        void Acquire(CommandBuffer cb, AccessFlags access, PipelineStageFlags stage);

        ~Buffer();
    private:
        Core* renderer;
        VkBuffer buffer;
        VmaAllocation allocation;

        uint64_t size;
        BufferUsage usage;
        
        AccessFlags lastAccess;
        PipelineStageFlags lastPipelineStage;
    };
}