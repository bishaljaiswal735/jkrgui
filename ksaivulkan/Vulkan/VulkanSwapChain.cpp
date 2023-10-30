#include "VulkanSwapChain.hpp"
using namespace ksai;

ksai::VulkanSwapChainBase::VulkanSwapChainBase(const VulkanDevice& inDevice)
	: mDevice(inDevice.GetDeviceHandle())
{
}

ksai::VulkanSwapChainBase::~VulkanSwapChainBase()
{
	ExplicitlyDestroy();
}

void ksai::VulkanSwapChainBase::ExplicitlyDestroy()
{
    if (mSwapChain)
    {
        mDevice.waitIdle();
        for (auto& u : mSwapChainImageViews) { mDevice.destroyImageView(u); }
        mDevice.destroy(mSwapChain);
    }
    mSwapChainImageViews.clear();
    mSwapChainImages.clear();
    mSwapChain = nullptr;
}

std::pair<uint32_t, uint32_t> ksai::VulkanSwapChainBase::AcquireNextImage(const VulkanSemaphore& inSemapore)
{
	vk::ResultValue<uint32_t> Result = mDevice.acquireNextImageKHR(mSwapChain, std::numeric_limits<uint32_t>::max(), inSemapore.GetSemaphoreHandle());
	return std::pair<uint32_t, uint32_t>(static_cast<uint32_t>(Result.result), static_cast<uint32_t>(Result.value));
}
