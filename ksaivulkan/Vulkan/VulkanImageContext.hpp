#pragma once
#include "VulkanDevice.hpp"
#include "VulkanQueue.hpp"
#include "VulkanSurface.hpp"
#include <iostream>
#include <vulkan/vulkan.hpp>

namespace ksai {
enum class ImageContext {
    Default,
    Unknown,
    ExternalHandled,
    DepthImage,
    Storage,
    CubeCompatible,
    ColorImageMSAA,
    DepthImageMSAA
};

struct ImageProperties {
    vk::Format mImageFormat = vk::Format::eR16G16B16A16Sfloat;
    vk::FormatFeatureFlagBits mImageFormatFeature = vk::FormatFeatureFlagBits::eSampledImage;
    vk::ImageType mImageType = vk::ImageType::e2D;
    vk::ImageUsageFlags mImageUsage = vk::ImageUsageFlagBits::eSampled;
    vk::SampleCountFlagBits mSampleCountFlagBits = vk::SampleCountFlagBits::e1;
    ui mMipLevels = 1;
    ui mArrayLayers = 1;
    vk::ImageTiling mTiling = vk::ImageTiling::eOptimal;
    vk::ImageAspectFlagBits mImageAspect = vk::ImageAspectFlagBits::eColor;
    vk::MemoryPropertyFlagBits mMemoryProperty = vk::MemoryPropertyFlagBits::eDeviceLocal;
    vk::ImageViewType mImageViewType = vk::ImageViewType::e2D;
    vk::SharingMode mSharingMode = vk::SharingMode::eExclusive;
    vk::ImageLayout mInitialImageLayout = vk::ImageLayout::eGeneral;
    vk::Extent2D mExtent;
    vk::ImageCreateFlags mFlags = vk::ImageCreateFlags();
};
}
