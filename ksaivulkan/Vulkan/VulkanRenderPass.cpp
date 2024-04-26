#include "VulkanRenderPass.hpp"
#include "VulkanImage.hpp"
#include "vulkan/vulkan_enums.hpp"
using namespace ksai;
template <RenderPassContext T> using vr = VulkanRenderPass<T>;

// TODO Uniformize Parameters in these functions
template <>
VulkanRenderPass<RenderPassContext::Default>::VulkanRenderPass(
     const VulkanDevice& inDevice,
     const VulkanSurface& inSurface,
     const VulkanImage<ImageContext::DepthImage>& inDepthImage)
    : mDevice(inDevice.GetDeviceHandle()) {
    vk::Format SurfaceSwapChainFormat = inSurface.GetSurfaceImageFormat();
    vk::Format DepthImageFormat       = inDepthImage.GetImageFormat();
    vk::AttachmentDescription DepthAttachment =
         vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), DepthImageFormat)
              .setInitialLayout(vk::ImageLayout::eUndefined)
              .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
              .setLoadOp(vk::AttachmentLoadOp::eClear)
              .setStoreOp(vk::AttachmentStoreOp::eDontCare)
              .setSamples(vk::SampleCountFlagBits::e1);

    vk::AttachmentReference DepthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentDescription ColorAttachment =
         vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), SurfaceSwapChainFormat)
              .setInitialLayout(vk::ImageLayout::eUndefined)
              .setFinalLayout(vk::ImageLayout::ePresentSrcKHR)
              .setLoadOp(vk::AttachmentLoadOp::eClear)
              .setStoreOp(vk::AttachmentStoreOp::eStore)
              .setSamples(vk::SampleCountFlagBits::e1);
    vk::AttachmentReference ColorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

    vk::SubpassDescription SubPassDescription =
         vk::SubpassDescription(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics)
              .setColorAttachmentCount(1)
              .setPColorAttachments(&ColorAttachmentRef) // TODO Remove this Addressing
              .setPDepthStencilAttachment(&DepthAttachmentRef);

    std::array<vk::SubpassDescription, 1> SubpassDescriptions;
    SubpassDescriptions.fill(SubPassDescription);
    std::array<vk::AttachmentDescription, 2> AttachmentDescriptions;
    AttachmentDescriptions = {ColorAttachment, DepthAttachment};

    vk::SubpassDependency SubpassDependency =
         vk::SubpassDependency()
              .setSrcSubpass(VK_SUBPASS_EXTERNAL)
              .setDstSubpass(0)
              .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput |
                               vk::PipelineStageFlagBits::eEarlyFragmentTests)
              .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput |
                               vk::PipelineStageFlagBits::eEarlyFragmentTests)
              .setSrcAccessMask(vk::AccessFlagBits::eNone)
              .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                vk::AccessFlagBits::eDepthStencilAttachmentWrite);

    vk::SubpassDependency SelfDependency =
         vk::SubpassDependency()
              .setSrcSubpass(0)
              .setDstSubpass(0)
              .setSrcStageMask(vk::PipelineStageFlagBits::eLateFragmentTests)
              .setDstStageMask(vk::PipelineStageFlagBits::eLateFragmentTests)
              .setSrcAccessMask(vk::AccessFlagBits::eNone)
              .setDstAccessMask(vk::AccessFlagBits::eNone)
              .setDependencyFlags(vk::DependencyFlagBits::eByRegion);

    std::array<vk::SubpassDependency, 2> SubpassDependencies;
    SubpassDependencies = {SubpassDependency, SelfDependency};

    mRenderPass         = mDevice.createRenderPass(vk::RenderPassCreateInfo()
                                                .setAttachmentCount(AttachmentDescriptions.size())
                                                .setAttachments(AttachmentDescriptions)
                                                .setSubpassCount(SubpassDescriptions.size())
                                                .setSubpasses(SubpassDescriptions)
                                                .setDependencyCount(SubpassDependencies.size())
                                                .setDependencies(SubpassDependencies));
}

template <>
VulkanRenderPass<RenderPassContext::Shadow>::VulkanRenderPass(const VulkanDevice& inDevice,
                                                              const VulkanImageBase& inDepthImage)
    : mDevice(inDevice.GetDeviceHandle()) {
    vk::Format DepthImageFormat = inDepthImage.GetImageFormat();
    vk::AttachmentDescription DepthAttachment =
         vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), DepthImageFormat)
              .setInitialLayout(vk::ImageLayout::eUndefined)
              .setFinalLayout(vk::ImageLayout::eDepthStencilReadOnlyOptimal)
              .setLoadOp(vk::AttachmentLoadOp::eClear)
              .setStoreOp(vk::AttachmentStoreOp::eStore);
    vk::AttachmentReference DepthAttachmentRef(0, vk::ImageLayout::eDepthStencilAttachmentOptimal);
    vk::SubpassDescription SubPassDescription =
         vk::SubpassDescription(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics)
              .setColorAttachmentCount(0)
              .setPDepthStencilAttachment(&DepthAttachmentRef);

    std::vector<vk::SubpassDescription> SubpassDescriptions       = {SubPassDescription};
    std::vector<vk::AttachmentDescription> AttachmentDescriptions = {DepthAttachment};

    vk::SubpassDependency SubpassDependency1 =
         vk::SubpassDependency()
              .setSrcSubpass(VK_SUBPASS_EXTERNAL)
              .setDstSubpass(0)
              .setSrcStageMask(vk::PipelineStageFlagBits::eFragmentShader)
              .setDstStageMask(vk::PipelineStageFlagBits::eEarlyFragmentTests)
              .setSrcAccessMask(vk::AccessFlagBits::eShaderRead)
              .setDstAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
              .setDependencyFlags(vk::DependencyFlagBits::eByRegion);

    vk::SubpassDependency SubpassDependency2 =
         vk::SubpassDependency()
              .setSrcSubpass(0)
              .setDstSubpass(VK_SUBPASS_EXTERNAL)
              .setSrcStageMask(vk::PipelineStageFlagBits::eLateFragmentTests)
              .setDstStageMask(vk::PipelineStageFlagBits::eFragmentShader)
              .setSrcAccessMask(vk::AccessFlagBits::eDepthStencilAttachmentWrite)
              .setDstAccessMask(vk::AccessFlagBits::eShaderRead)
              .setDependencyFlags(vk::DependencyFlagBits::eByRegion);
    std::vector<vk::SubpassDependency> SubpassDependencies = {SubpassDependency1,
                                                              SubpassDependency2};
    mRenderPass = mDevice.createRenderPass(vk::RenderPassCreateInfo()
                                                .setAttachmentCount(AttachmentDescriptions.size())
                                                .setAttachments(AttachmentDescriptions)
                                                .setSubpasses(SubpassDescriptions)
                                                .setDependencies(SubpassDependencies));
}

template <>
VulkanRenderPass<RenderPassContext::MSAA>::VulkanRenderPass(
     const VulkanDevice& inDevice,
     const VulkanSurface& inSurface,
     const VulkanImage<ImageContext::ColorAttach>& inColorImageTarget,
     const VulkanImage<ImageContext::DepthImage>& inDepthImage,
     vk::SampleCountFlagBits inMSAASamples)
    : mDevice(inDevice.GetDeviceHandle()) {
    vk::Format SurfaceSwapChainFormat = inSurface.GetSurfaceImageFormat();
    vk::Format DepthImageFormat       = inDepthImage.GetImageFormat();

    vk::AttachmentDescription ColorAttachment =
         vk::AttachmentDescription(vk::AttachmentDescriptionFlags(),
                                   inColorImageTarget.GetImageFormat())
              .setInitialLayout(vk::ImageLayout::eUndefined)
              .setFinalLayout(
                   vk::ImageLayout::eColorAttachmentOptimal) // For multi sampled images cannot be
                                                             // presented directly
              .setLoadOp(vk::AttachmentLoadOp::eClear)
              .setStoreOp(vk::AttachmentStoreOp::eStore)
              .setSamples(inMSAASamples);
    vk::AttachmentReference ColorAttachmentRef(0, vk::ImageLayout::eColorAttachmentOptimal);

    vk::AttachmentDescription DepthAttachment =
         vk::AttachmentDescription(vk::AttachmentDescriptionFlags(), DepthImageFormat)
              .setInitialLayout(vk::ImageLayout::eUndefined)
              .setFinalLayout(vk::ImageLayout::eDepthStencilAttachmentOptimal)
              .setLoadOp(vk::AttachmentLoadOp::eClear)
              .setStoreOp(vk::AttachmentStoreOp::eDontCare)
              .setSamples(inMSAASamples);
    vk::AttachmentReference DepthAttachmentRef(1, vk::ImageLayout::eDepthStencilAttachmentOptimal);

    vk::AttachmentDescription ColorAttachmentResolve =
         vk::AttachmentDescription(vk::AttachmentDescriptionFlags(),
                                   inColorImageTarget.GetImageFormat())
              .setInitialLayout(vk::ImageLayout::eUndefined)
              .setFinalLayout(vk::ImageLayout::ePresentSrcKHR)
              .setLoadOp(vk::AttachmentLoadOp::eDontCare)
              .setStoreOp(vk::AttachmentStoreOp::eStore)
              .setStencilLoadOp(vk::AttachmentLoadOp::eDontCare)
              .setStencilStoreOp(vk::AttachmentStoreOp::eDontCare);
    vk::AttachmentReference ColorAttachmentResolveRef(
         2, vk::ImageLayout::eColorAttachmentOptimal); // Subpass ma hune layout expected

    vk::SubpassDescription SubPassDescription =
         vk::SubpassDescription(vk::SubpassDescriptionFlags(), vk::PipelineBindPoint::eGraphics)
              .setColorAttachmentCount(1)
              .setColorAttachments(ColorAttachmentRef)
              .setPDepthStencilAttachment(&DepthAttachmentRef)
              .setResolveAttachments(ColorAttachmentResolveRef);

    std::vector<vk::SubpassDescription> SubpassDescriptions       = {SubPassDescription};
    std::vector<vk::AttachmentDescription> AttachmentDescriptions = {
         ColorAttachment, DepthAttachment, ColorAttachmentResolve};

    vk::SubpassDependency SubpassDependency =
         vk::SubpassDependency()
              .setSrcSubpass(VK_SUBPASS_EXTERNAL)
              .setDstSubpass(0)
              .setSrcStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput |
                               vk::PipelineStageFlagBits::eEarlyFragmentTests |
                               vk::PipelineStageFlagBits::eBottomOfPipe) // TOO Naive TODO Fix this
              .setDstStageMask(vk::PipelineStageFlagBits::eColorAttachmentOutput |
                               vk::PipelineStageFlagBits::eEarlyFragmentTests |
                               vk::PipelineStageFlagBits::eBottomOfPipe)
              .setSrcAccessMask(vk::AccessFlagBits::eNone)
              .setDstAccessMask(vk::AccessFlagBits::eColorAttachmentWrite |
                                vk::AccessFlagBits::eDepthStencilAttachmentWrite);

    vk::SubpassDependency SelfDependency =
         vk::SubpassDependency()
              .setSrcSubpass(0)
              .setDstSubpass(0)
              .setSrcStageMask(vk::PipelineStageFlagBits::eLateFragmentTests)
              .setDstStageMask(vk::PipelineStageFlagBits::eLateFragmentTests)
              .setSrcAccessMask(vk::AccessFlagBits::eNone)
              .setDstAccessMask(vk::AccessFlagBits::eNone)
              .setDependencyFlags(vk::DependencyFlagBits::eByRegion);

    std::vector<vk::SubpassDependency> SubpassDependencies = {SubpassDependency, SelfDependency};

    mRenderPass = mDevice.createRenderPass(vk::RenderPassCreateInfo()
                                                .setAttachmentCount(AttachmentDescriptions.size())
                                                .setAttachments(AttachmentDescriptions)
                                                .setSubpassCount(SubpassDescriptions.size())
                                                .setSubpasses(SubpassDescriptions)
                                                .setDependencyCount(SubpassDependencies.size())
                                                .setDependencies(SubpassDependencies));
}
