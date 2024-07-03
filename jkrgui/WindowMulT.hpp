﻿#pragma once
#include "MultiThreading.hpp"
#include "Offscreen.hpp"
#include "VulkanCommandBuffer.hpp"
#include <Vendor/Tracy/tracy/TracyVulkan.hpp>

namespace Jkr {
class WindowMulT : public Window {
    using VulkanCommandBufferArray = std::array<VulkanCommandBuffer, mMaxFramesInFlight>;
    using ThreadCommandBufferArray = ThreadCommandBuffer<mMaxFramesInFlight>;

    public:
    const VulkanCommandBufferArray& GetCommandBuffers(ParameterContext inParameter) const override;
    GETTER& GetShadowPass() { return *mShadowPass; }
    GETTER& GetDeferredPass() { return *mDeferredPass; }
    void BuildShadowPass(ui inWidth, ui inHeight);
    void BeginShadowPass(float ind);
    void EndShadowPass();

    void BuildDeferredPass(ui inWidth, ui inHeight);
    void BeginDeferredDraws(float r, float g, float b, float a, float d);
    void EndDeferredDraws();

    void BeginUpdates();
    void EndUpdates();
    void BeginDispatches();
    void EndDispatches();

    void BeginDraws(float r, float g, float b, float a, float d);
    void EndDraws();
    void Present();
    void Refresh();

    void BeginUIs();
    void EndUIs();
    void ExecuteUIs();
    void BeginThreadCommandBuffer(int inThreadId);
    void EndThreadCommandBuffer(int inThreadId);
    void ExecuteThreadCommandBuffer(int inThreadId);

    WindowMulT(const Instance& inInstance,
               const sv inTitle,
               ui inHeight,
               ui inWidth,
               ui inNumThreads,
               std::span<ui> inPerThreadBuffers,
               optref<ksai::ThreadPool> inPool = std::nullopt);
    ~WindowMulT() = default;

    private:
    optref<ksai::ThreadPool> mThreadPool;
    v<up<ThreadCommandBufferArray>> mThreadCommandBuffers;
    VulkanCommandPool mUICommandPool;
    VulkanCommandBufferArray mSecondaryCommandBuffersUI;
    Up<ShadowPass> mShadowPass;
    Up<DeferredPass> mDeferredPass;
    glm::uvec2 mFrameSize;
};

} // namespace Jkr
