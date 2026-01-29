#pragma once

// ----------------------------------------- //
// Headers
// ----------------------------------------- //
#include "axr/logging.h"
#include "types.h"
#include "vulkan/vulkanRenderer.h"

#include <utility>

// ----------------------------------------- //
// Public Functions
// ----------------------------------------- //

// NOTE (Ashe): If we need to expand this to accept more than 1 api callback function, do it like this.
//  Create InvocableWithNewApiContext concept for the new api context class
//  Add 'typename NewApiCallback_T' to the template
//  Add '&& InvocableWithNewApiContext<NewApiCallback_T, Args...>' at the end of 'requires'
//  Add 'NewApiCallback_T&& newApiCallback' parameter
//  Uncomment 'using R2_T = std::invoke_result_t<NewApiCallback_T, AxrNewApiRenderer::Context&, Args...>;'
//  Uncomment 'static_assert(std::is_same_v<R1_T, R2_T>, ...)'
//  Create new case statement for the new api

// Don't wrap in an AXR_VULKAN_SUPPORTED preprocessor.
// An empty AxrVulkanRenderer::Context struct is declared if AXR_VULKAN_SUPPORTED isn't defined
/// Check that the given function is invocable with the vulkan renderer context
template<typename Func_T, typename... Args>
concept InvocableWithVulkanContext = std::invocable<Func_T, AxrVulkanRenderer::Context&, Args...>;

#define AXR_FUNCTION_FAILED_STRING "Failed to call renderer context function. "
/// Generic function to call different callbacks depending on the rendering api type
/// @tparam VulkanCallback_T Vulkan callback function type
/// @tparam Args Function arguments
/// @param context Rendering context
/// @param vulkanCallback Vulkan callback function
/// @param args Callback function arguments
template<typename VulkanCallback_T, typename... Args>
    requires InvocableWithVulkanContext<VulkanCallback_T, Args...>
decltype(auto) axrRendererContextExecute(AxrRendererContext& context,
                                         VulkanCallback_T&& vulkanCallback,
                                         Args&&... args) {
    using R1_T = std::invoke_result_t<VulkanCallback_T, AxrVulkanRenderer::Context&, Args...>;
    // using R2_T = std::invoke_result_t<NewApiCallback_T, AxrNewApiRenderer::Context&, Args...>;

    // static_assert(std::is_same_v<R1_T, R2_T>, "All functions must have the same return type.");

    using Return_T = R1_T;

    switch (context.ApiType) {
        case AXR_RENDERER_CONTEXT_API_TYPE_VULKAN: {
            if constexpr (std::is_void_v<Return_T>) {
                std::invoke(std::forward<VulkanCallback_T>(vulkanCallback),
                            context.Vulkan,
                            std::forward<Args>(args)...);
                return;
            } else {
                return std::invoke(std::forward<VulkanCallback_T>(vulkanCallback),
                                   context.Vulkan,
                                   std::forward<Args>(args)...);
            }
        }
        case AXR_RENDERER_CONTEXT_API_TYPE_UNDEFINED:
        default: {
            axrLogError(AXR_FUNCTION_FAILED_STRING "Unknown render api type.");
            if constexpr (std::is_void_v<Return_T>) {
                return;
            } else {
                return Return_T{};
            }
        }
    }
}
#undef AXR_FUNCTION_FAILED_STRING
