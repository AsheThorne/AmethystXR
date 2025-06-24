#pragma once

// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <filesystem>
#include <vector>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "axr/assets.h"
#include "axr/common/enums.h"
#include "mesh.hpp"

// ----------------------------------------- //
//  Tiny glTF Headers
// ----------------------------------------- //
#include <tiny_gltf.h>

/// Convert an STB image color channel to an AxrImageColorChannelsEnum
/// @param colorChannels STB image color channels
/// @returns The converted color channels
AxrImageColorChannelsEnum axrToImageColorChannels(int colorChannels);

/// Convert a tiny_gltf image sampler filter to an AxrImageSamplerFilterEnum
/// @param samplerFilter tiny_gltf image sampler filter
/// @param mipmapFilter Output mip map filter component of the sampler filter
/// @returns The converted image sampler filter
AxrImageSamplerFilterEnum axrToImageSamplerFilter(int samplerFilter, AxrImageSamplerFilterEnum& mipmapFilter);
/// Convert a tiny_gltf image sampler wrap to an AxrImageSamplerWrapEnum
/// @param samplerWrap tiny_gltf image sampler wrap
/// @returns The converted image sampler wrap
AxrImageSamplerWrapEnum axrToImageSamplerWrap(int samplerWrap);

/// Read file bytes on disk
/// @param path Path to the file
/// @param data Output file data
/// @returns AXR_SUCCESS if the function succeeded
AxrResult axrReadFileBytes(const std::filesystem::path& path, std::vector<char>& data);

/// Get the absolute file path for the given path to the assets directory
/// @param path Path to use
/// @returns The absolute path
std::filesystem::path axrGetAbsolutePathToAsset(const std::filesystem::path& path);
/// Get the 'assets' directory path
/// @returns Path to the 'assets' directory
std::filesystem::path axrGetAssetsDirectoryPath();
/// Get the 'axr-assets' directory path
/// @returns Path to the 'axr-assets' directory
std::filesystem::path axrGetEngineAssetsDirectoryPath();
/// Get the directory where the executable file sits
/// @returns Path to the executable file directory
std::filesystem::path axrGetExecutableFileDirectoryPath();

/// Read a glTF file at the given path
/// @param path glTF model file path
/// @param model Output model data
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrReadGltfFile(const std::filesystem::path& path, tinygltf::Model& model);

/// Load a model's mesh data
/// @param path The model's file path
/// @param meshes Output model mesh data
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrLoadModel(const std::filesystem::path& path, std::vector<AxrMeshRAII>& meshes);
/// Load a glTF model's mesh data
/// @param path The model's file path
/// @param meshes Output model mesh data
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrLoadModel_glTF(const std::filesystem::path& path, std::vector<AxrMeshRAII>& meshes);

/// Get a glTF model's file info
/// @param path The model's file path
/// @param modelFileInfo Output model file info
/// @returns AXR_SUCCESS if the function succeeded
[[nodiscard]] AxrResult axrGetModelFileInfo_glTF(
    const std::filesystem::path& path,
    AxrModelFileInfo* modelFileInfo
);

/// Get the gltf accessor component size in bytes
/// @param accessorComponentType The accessor component type
/// @returns Size of component in bytes
[[nodiscard]] size_t axrGetGltfComponentByteSize(int accessorComponentType);
/// Get the number of components in the specified gltf accessor type
/// @param accessorType The accessor type
/// @returns The number of components
[[nodiscard]] size_t axrGetGltfComponentCount(int accessorType);

/// Get gltf model data for the given accessor index
/// @tparam Type Data Type
/// @param model Gltf model
/// @param accessorIndex Accessor Index
/// @returns Accessor data
template <typename Type>
[[nodiscard]] std::vector<Type> axrGetGltfBufferData(const tinygltf::Model& model, const int accessorIndex) {
    const tinygltf::Accessor& accessor = model.accessors[accessorIndex];
    const tinygltf::BufferView& bufferView = model.bufferViews[accessor.bufferView];
    const tinygltf::Buffer& buffer = model.buffers[bufferView.buffer];

    const size_t componentByteSize = axrGetGltfComponentByteSize(accessor.componentType);
    const size_t componentCount = axrGetGltfComponentCount(accessor.type);
    const size_t dataItemByteSize = componentByteSize * componentCount;

    const uint8_t* bufferBegin = buffer.data.data() + bufferView.byteOffset + accessor.byteOffset;
    std::vector<Type> data;

    // If the size of 'Type' is the same as the size of item, then we can safely cast and copy the buffer data directly.
    // Otherwise, we need to cast each element to the right size.
    if (sizeof(Type) == dataItemByteSize) {
        const uint8_t* bufferEnd = bufferBegin + accessor.count * dataItemByteSize;
        data.assign(
            reinterpret_cast<const Type*>(bufferBegin),
            reinterpret_cast<const Type*>(bufferEnd)
        );
    } else {
        data.resize(accessor.count);

        for (size_t i = 0; i < data.size(); ++i) {
            const uint8_t* dataItem = bufferBegin + i * dataItemByteSize;
            memcpy_s(
                &data[i],
                sizeof(Type),
                dataItem,
                dataItemByteSize
            );
        }
    }

    return data;
}
