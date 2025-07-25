// ----------------------------------------- //
// C/C++ Headers
// ----------------------------------------- //
#include <fstream>

// ----------------------------------------- //
// AXR Headers
// ----------------------------------------- //
#include "assetsUtils.hpp"
#include "axr/logger.h"
#include "axr/common/utils.h"

#ifdef AXR_USE_PLATFORM_WIN32
// ----------------------------------------- //
// Win32 Headers
// ----------------------------------------- //
#include <windows.h>
#endif

// ----------------------------------------- //
// STB Headers
// ----------------------------------------- //
#include <stb_image.h>

// ----------------------------------------- //
// GLM Headers
// ----------------------------------------- //
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>

// ----------------------------------------- //
// External Functions
// ----------------------------------------- //

AxrModelFileImageSamplerInfo axrModelFileImageSamplerInfoClone(
    const AxrModelFileImageSamplerInfo* modelFileImageSamplerInfo
) {
    if (modelFileImageSamplerInfo == nullptr) {
        axrLogErrorLocation("`modelFileImageSamplerInfo` is null.");
        return {};
    }

    AxrModelFileImageSamplerInfo info{
        .Name = {},
        .MinFilter = modelFileImageSamplerInfo->MinFilter,
        .MagFilter = modelFileImageSamplerInfo->MagFilter,
        .MipmapFilter = modelFileImageSamplerInfo->MipmapFilter,
        .WrapU = modelFileImageSamplerInfo->WrapU,
        .WrapV = modelFileImageSamplerInfo->WrapV,
    };
    strncpy_s(info.Name, modelFileImageSamplerInfo->Name, AXR_MAX_ASSET_NAME_SIZE);

    return info;
}

void axrModelFileImageSamplerInfoDestroy(AxrModelFileImageSamplerInfo* modelFileImageSamplerInfo) {
    if (modelFileImageSamplerInfo == nullptr) return;

    memset(modelFileImageSamplerInfo->Name, 0, sizeof(modelFileImageSamplerInfo->Name));
    modelFileImageSamplerInfo->MinFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    modelFileImageSamplerInfo->MagFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    modelFileImageSamplerInfo->MipmapFilter = AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
    modelFileImageSamplerInfo->WrapU = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
    modelFileImageSamplerInfo->WrapV = AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
}

AxrModelFileImageInfo axrModelFileImageInfoClone(const AxrModelFileImageInfo* modelFileImageInfo) {
    if (modelFileImageInfo == nullptr) {
        axrLogErrorLocation("`modelFileImageInfo` is null.");
        return {};
    }

    AxrModelFileImageInfo info{
        .Name = {},
        .FilePath = {},
    };
    strncpy_s(info.Name, modelFileImageInfo->Name, AXR_MAX_ASSET_NAME_SIZE);
    strncpy_s(info.FilePath, modelFileImageInfo->FilePath, AXR_MAX_FILE_PATH_SIZE);

    return info;
}

void axrModelFileImageInfoDestroy(AxrModelFileImageInfo* modelFileImageInfo) {
    if (modelFileImageInfo == nullptr) return;

    memset(modelFileImageInfo->Name, 0, sizeof(modelFileImageInfo->Name));
    memset(modelFileImageInfo->FilePath, 0, sizeof(modelFileImageInfo->FilePath));
}

AxrModelFileMaterialInfo axrModelFileMaterialInfoClone(
    const AxrModelFileMaterialInfo* modelFileMaterialInfo
) {
    if (modelFileMaterialInfo == nullptr) {
        axrLogErrorLocation("`modelFileMaterialInfo` is null.");
        return {};
    }

    AxrModelFileMaterialInfo info{
        .Name = {},
        .ColorImageIndex = modelFileMaterialInfo->ColorImageIndex,
        .ColorImageSamplerIndex = modelFileMaterialInfo->ColorImageSamplerIndex,
        .ColorFactor = modelFileMaterialInfo->ColorFactor,
    };
    strncpy_s(info.Name, modelFileMaterialInfo->Name, AXR_MAX_ASSET_NAME_SIZE);

    return info;
}

void axrModelFileMaterialInfoDestroy(AxrModelFileMaterialInfo* modelFileMaterialInfo) {
    if (modelFileMaterialInfo == nullptr) return;

    memset(modelFileMaterialInfo->Name, 0, sizeof(modelFileMaterialInfo->Name));
    modelFileMaterialInfo->ColorImageIndex = -1;
    modelFileMaterialInfo->ColorImageSamplerIndex = -1;
    modelFileMaterialInfo->ColorFactor = {};
}

AxrModelFileSubmeshInfo axrModelFileSubmeshInfoClone(
    const AxrModelFileSubmeshInfo* modelFileSubmeshInfo
) {
    if (modelFileSubmeshInfo == nullptr) {
        axrLogErrorLocation("`modelFileSubmeshInfo` is null.");
        return {};
    }

    return AxrModelFileSubmeshInfo{
        .MaterialIndex = modelFileSubmeshInfo->MaterialIndex,
    };
}

void axrModelFileSubmeshInfoDestroy(AxrModelFileSubmeshInfo* modelFileSubmeshInfo) {
    if (modelFileSubmeshInfo == nullptr) return;

    modelFileSubmeshInfo->MaterialIndex = -1;
}

AxrModelFileMeshInfo axrModelFileMeshInfoClone(
    const AxrModelFileMeshInfo* modelFileMeshInfo
) {
    if (modelFileMeshInfo == nullptr) {
        axrLogErrorLocation("`modelFileMeshInfo` is null.");
        return {};
    }

    AxrModelFileSubmeshInfo* submeshes = nullptr;

    if (modelFileMeshInfo->SubmeshCount != 0 && modelFileMeshInfo->Submeshes != nullptr) {
        submeshes = new AxrModelFileSubmeshInfo[modelFileMeshInfo->SubmeshCount]{};

        for (uint32_t i = 0; i < modelFileMeshInfo->SubmeshCount; ++i) {
            submeshes[i] = axrModelFileSubmeshInfoClone(&modelFileMeshInfo->Submeshes[i]);
        }
    }

    return AxrModelFileMeshInfo{
        .SubmeshCount = modelFileMeshInfo->SubmeshCount,
        .Submeshes = submeshes
    };
}

void axrModelFileMeshInfoDestroy(AxrModelFileMeshInfo* modelFileMeshInfo) {
    if (modelFileMeshInfo == nullptr) return;

    if (modelFileMeshInfo->Submeshes != nullptr) {
        for (uint32_t i = 0; i < modelFileMeshInfo->SubmeshCount; ++i) {
            axrModelFileSubmeshInfoDestroy(&modelFileMeshInfo->Submeshes[i]);
        }

        delete[] modelFileMeshInfo->Submeshes;
        modelFileMeshInfo->Submeshes = nullptr;
    }

    modelFileMeshInfo->SubmeshCount = 0;
}

AxrModelFileInfo axrModelFileInfoClone(const AxrModelFileInfo* modelFileInfo) {
    AxrModelFileImageSamplerInfo* imageSamplers = nullptr;
    AxrModelFileImageInfo* images = nullptr;
    AxrModelFileMaterialInfo* materials = nullptr;
    AxrModelFileMeshInfo* meshes = nullptr;

    if (modelFileInfo->ImageSamplerCount != 0 && modelFileInfo->ImageSamplers != nullptr) {
        imageSamplers = new AxrModelFileImageSamplerInfo[modelFileInfo->ImageSamplerCount]{};

        for (uint32_t i = 0; i < modelFileInfo->ImageSamplerCount; ++i) {
            imageSamplers[i] = axrModelFileImageSamplerInfoClone(&modelFileInfo->ImageSamplers[i]);
        }
    }

    if (modelFileInfo->ImageCount != 0 && modelFileInfo->Images != nullptr) {
        images = new AxrModelFileImageInfo[modelFileInfo->ImageCount]{};

        for (uint32_t i = 0; i < modelFileInfo->ImageCount; ++i) {
            images[i] = axrModelFileImageInfoClone(&modelFileInfo->Images[i]);
        }
    }

    if (modelFileInfo->MaterialCount != 0 && modelFileInfo->Materials != nullptr) {
        materials = new AxrModelFileMaterialInfo[modelFileInfo->MaterialCount]{};

        for (uint32_t i = 0; i < modelFileInfo->MaterialCount; ++i) {
            materials[i] = axrModelFileMaterialInfoClone(&modelFileInfo->Materials[i]);
        }
    }

    if (modelFileInfo->MeshCount != 0 && modelFileInfo->Meshes != nullptr) {
        meshes = new AxrModelFileMeshInfo[modelFileInfo->MeshCount]{};

        for (uint32_t i = 0; i < modelFileInfo->MeshCount; ++i) {
            meshes[i] = axrModelFileMeshInfoClone(&modelFileInfo->Meshes[i]);
        }
    }

    return AxrModelFileInfo{
        .ImageSamplerCount = modelFileInfo->ImageSamplerCount,
        .ImageSamplers = imageSamplers,
        .ImageCount = modelFileInfo->ImageCount,
        .Images = images,
        .MaterialCount = modelFileInfo->MaterialCount,
        .Materials = materials,
        .MeshCount = modelFileInfo->MeshCount,
        .Meshes = meshes,
    };
}

void axrModelFileInfoDestroy(AxrModelFileInfo* modelFileInfo) {
    if (modelFileInfo == nullptr) return;

    if (modelFileInfo->ImageSamplers != nullptr) {
        for (uint32_t i = 0; i < modelFileInfo->ImageSamplerCount; ++i) {
            axrModelFileImageSamplerInfoDestroy(&modelFileInfo->ImageSamplers[i]);
        }

        delete[] modelFileInfo->ImageSamplers;
        modelFileInfo->ImageSamplers = nullptr;
    }
    modelFileInfo->ImageSamplerCount = 0;

    if (modelFileInfo->Images != nullptr) {
        for (uint32_t i = 0; i < modelFileInfo->ImageCount; ++i) {
            axrModelFileImageInfoDestroy(&modelFileInfo->Images[i]);
        }

        delete[] modelFileInfo->Images;
        modelFileInfo->Images = nullptr;
    }
    modelFileInfo->ImageCount = 0;

    if (modelFileInfo->Materials != nullptr) {
        for (uint32_t i = 0; i < modelFileInfo->MaterialCount; ++i) {
            axrModelFileMaterialInfoDestroy(&modelFileInfo->Materials[i]);
        }

        delete[] modelFileInfo->Materials;
        modelFileInfo->Materials = nullptr;
    }
    modelFileInfo->MaterialCount = 0;

    if (modelFileInfo->Meshes != nullptr) {
        for (uint32_t i = 0; i < modelFileInfo->MeshCount; ++i) {
            axrModelFileMeshInfoDestroy(&modelFileInfo->Meshes[i]);
        }

        delete[] modelFileInfo->Meshes;
        modelFileInfo->Meshes = nullptr;
    }
    modelFileInfo->MeshCount = 0;
}

AxrResult axrGetModelFileInfo(
    const char* path,
    AxrModelFileInfo* modelFileInfo
) {
    const std::filesystem::path filePath = std::filesystem::path(path);
    const std::filesystem::path extension = filePath.extension();

    if (extension == ".gltf" || extension == ".glb") {
        return axrGetModelFileInfo_glTF(filePath, modelFileInfo);
    }

    axrLogErrorLocation("Unknown model file extension: {0}.", extension.string().c_str());
    return AXR_ERROR;
}

// ----------------------------------------- //
// Internal Functions
// ----------------------------------------- //

AxrImageColorChannelsEnum axrToImageColorChannels(const int colorChannels) {
    switch (colorChannels) {
        case STBI_grey: {
            return AXR_IMAGE_COLOR_CHANNELS_GRAY;
        }
        case STBI_grey_alpha: {
            return AXR_IMAGE_COLOR_CHANNELS_GRAY_ALPHA;
        }
        case STBI_rgb: {
            return AXR_IMAGE_COLOR_CHANNELS_RGB;
        }
        case STBI_rgb_alpha: {
            return AXR_IMAGE_COLOR_CHANNELS_RGB_ALPHA;
        }
        default: {
            axrLogErrorLocation("Unknown color channel: {0}.", colorChannels);
            return AXR_IMAGE_COLOR_CHANNELS_UNDEFINED;
        }
    }
}

AxrImageSamplerFilterEnum axrToImageSamplerFilter(const int samplerFilter, AxrImageSamplerFilterEnum& mipmapFilter) {
    switch (samplerFilter) {
        case TINYGLTF_TEXTURE_FILTER_NEAREST: {
            return AXR_IMAGE_SAMPLER_FILTER_NEAREST;
        }
        case TINYGLTF_TEXTURE_FILTER_LINEAR: {
            return AXR_IMAGE_SAMPLER_FILTER_LINEAR;
        }
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_NEAREST: {
            mipmapFilter = AXR_IMAGE_SAMPLER_FILTER_NEAREST;
            return AXR_IMAGE_SAMPLER_FILTER_NEAREST;
        }
        case TINYGLTF_TEXTURE_FILTER_NEAREST_MIPMAP_LINEAR: {
            mipmapFilter = AXR_IMAGE_SAMPLER_FILTER_LINEAR;
            return AXR_IMAGE_SAMPLER_FILTER_NEAREST;
        }
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_LINEAR: {
            mipmapFilter = AXR_IMAGE_SAMPLER_FILTER_LINEAR;
            return AXR_IMAGE_SAMPLER_FILTER_LINEAR;
        }
        case TINYGLTF_TEXTURE_FILTER_LINEAR_MIPMAP_NEAREST: {
            mipmapFilter = AXR_IMAGE_SAMPLER_FILTER_NEAREST;
            return AXR_IMAGE_SAMPLER_FILTER_LINEAR;
        }
        default: {
            axrLogErrorLocation("Unknown sampler filter: {0}.", samplerFilter);
            return AXR_IMAGE_SAMPLER_FILTER_UNDEFINED;
        }
    }
}

AxrImageSamplerWrapEnum axrToImageSamplerWrap(const int samplerWrap) {
    switch (samplerWrap) {
        case TINYGLTF_TEXTURE_WRAP_REPEAT: {
            return AXR_IMAGE_SAMPLER_WRAP_REPEAT;
        }
        case TINYGLTF_TEXTURE_WRAP_MIRRORED_REPEAT: {
            return AXR_IMAGE_SAMPLER_WRAP_MIRRORED_REPEAT;
        }
        case TINYGLTF_TEXTURE_WRAP_CLAMP_TO_EDGE: {
            return AXR_IMAGE_SAMPLER_WRAP_CLAMP_TO_EDGE;
        }
        default: {
            axrLogErrorLocation("Unknown sampler wrap: {0}.", samplerWrap);
            return AXR_IMAGE_SAMPLER_WRAP_UNDEFINED;
        }
    }
}

bool axrFileExists(const std::filesystem::path& path) {
    return std::filesystem::exists(axrGetAbsolutePathToAsset(path));;
}

std::filesystem::path axrGetAbsolutePathToAsset(const std::filesystem::path& path) {
    if (path.is_absolute()) {
        return path;
    }

    // We can assume any path that isn't absolute, is meant to point to the application 'assets' folder.
    // Engine defined asset paths are all absolute and will already point to the 'axr-assets' folder
    return axrGetAssetsDirectoryPath().append(path.c_str());
}

std::filesystem::path axrGetAssetsDirectoryPath() {
    return axrGetExecutableFileDirectoryPath().append("assets");
}

std::filesystem::path axrGetEngineAssetsDirectoryPath() {
    return axrGetExecutableFileDirectoryPath().append("axr-assets");
}

std::filesystem::path axrGetExecutableFileDirectoryPath() {
    std::wstring filePath;

#ifdef AXR_USE_PLATFORM_WIN32
    filePath.resize(MAX_PATH);
    GetModuleFileNameW(nullptr, filePath.data(), MAX_PATH);
    // Remove the executable file from the path
    filePath = filePath.substr(0, filePath.find_last_of(L"\\/") + 1);
#else
    axrLogErrorLocation("Unknown platform.")
    return {};
#endif

    return filePath;
}

AxrResult axrReadGltfFile(const std::filesystem::path& path, tinygltf::Model& model) {
    tinygltf::TinyGLTF loader;
    std::string error;
    std::string warning;
    bool result;

    const std::filesystem::path fullPath = axrGetAbsolutePathToAsset(path);
    const std::filesystem::path extension = fullPath.extension();

    if (extension == ".gltf") {
        result = loader.LoadASCIIFromFile(&model, &error, &warning, fullPath.string());
    } else if (extension == ".glb") {
        result = loader.LoadBinaryFromFile(&model, &error, &warning, fullPath.string());
    } else {
        axrLogErrorLocation("Unknown glTF model file extension: {0}.", extension.string().c_str());
        return AXR_ERROR;
    }

    if (!warning.empty()) {
        axrLogWarningLocation("Failed to load glTF model file: {0}.", warning.c_str());
        return AXR_ERROR;
    }

    if (!error.empty()) {
        axrLogErrorLocation("Failed to load glTF model file: {0}.", error.c_str());
        return AXR_ERROR;
    }

    if (!result) {
        axrLogErrorLocation("Failed to load glTF model file.");
        return AXR_ERROR;
    }

    return AXR_SUCCESS;
}

AxrResult axrLoadModel(const std::filesystem::path& path, std::vector<AxrMeshRAII>& meshes) {
    const std::filesystem::path extension = path.extension();

    if (extension == ".gltf" || extension == ".glb") {
        return axrLoadModel_glTF(path, meshes);
    }

    axrLogErrorLocation("Unknown model file extension: {0}.", extension.string().c_str());
    return AXR_ERROR;
}

AxrResult axrLoadModel_glTF(const std::filesystem::path& path, std::vector<AxrMeshRAII>& meshes) {
    AxrResult axrResult = AXR_SUCCESS;

    tinygltf::Model model;
    axrResult = axrReadGltfFile(path, model);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    // glTF Reference Guide https://www.khronos.org/files/gltf20-reference-guide.pdf

    meshes.resize(model.meshes.size());
    for (size_t meshIndex = 0; meshIndex < model.meshes.size(); ++meshIndex) {
        meshes[meshIndex].Submeshes.resize(model.meshes[meshIndex].primitives.size());
        glm::mat4 modelMatrix = glm::mat4(1.0f);

        // Apply node transformations
        for (const tinygltf::Node& node : model.nodes) {
            // TODO: We need to get the model matrix of every parent too for the global model matrix.
            //  But we need a more complex model to test it with.
            if (node.mesh == meshIndex) {
                if (!node.matrix.empty()) {
                    // If a matrix is defined, then the individual components won't be.
                    // Ref: https://registry.khronos.org/glTF/specs/2.0/glTF-2.0.html#transformations
                    modelMatrix = glm::mat4(
                        {node.matrix[0], node.matrix[1], node.matrix[2], node.matrix[3]},
                        {node.matrix[4], node.matrix[5], node.matrix[6], node.matrix[7]},
                        {node.matrix[8], node.matrix[9], node.matrix[10], node.matrix[11]},
                        {node.matrix[12], node.matrix[13], node.matrix[14], node.matrix[15]}
                    );
                } else {
                    glm::mat4 scaleMatrix = glm::mat4(1.0f);
                    glm::mat4 translateMatrix = glm::mat4(1.0f);
                    glm::mat4 rotationMatrix = glm::mat4(1.0f);

                    if (!node.scale.empty()) {
                        scaleMatrix = glm::scale(
                            glm::mat4(1.0f),
                            glm::vec3(node.scale[0], node.scale[1], node.scale[2])
                        );
                    }
                    if (!node.translation.empty()) {
                        translateMatrix = glm::translate(
                            glm::mat4(1.0f),
                            glm::vec3(node.translation[0], node.translation[1], node.translation[2])
                        );
                    }
                    if (!node.rotation.empty()) {
                        rotationMatrix = glm::toMat4(
                            glm::quat(
                                static_cast<float>(node.rotation[3]),
                                static_cast<float>(node.rotation[0]),
                                static_cast<float>(node.rotation[1]),
                                static_cast<float>(node.rotation[2])
                            )
                        );
                    }

                    modelMatrix = translateMatrix * rotationMatrix * scaleMatrix;
                }
            }
        }

        for (size_t submeshIndex = 0; submeshIndex < model.meshes[meshIndex].primitives.size(); ++submeshIndex) {
            AxrSubmeshRAII& submesh = meshes[meshIndex].Submeshes[submeshIndex];

            submesh.Indices = axrGetGltfBufferData<uint32_t>(
                model,
                model.meshes[meshIndex].primitives[submeshIndex].indices
            );

            std::vector<glm::vec3> vertexPositions = axrGetGltfBufferData<glm::vec3>(
                model,
                model.meshes[meshIndex].primitives[submeshIndex].attributes["POSITION"]
            );

            std::vector<glm::vec4> vertexColors;
            if (model.meshes[meshIndex].primitives[submeshIndex].attributes.contains("COLOR_0")) {
                vertexColors = axrGetGltfBufferData<glm::vec4>(
                    model,
                    model.meshes[meshIndex].primitives[submeshIndex].attributes["COLOR_0"]
                );
            }

            // We only go up to a max of 4 tex coord channels for now
            std::vector<std::vector<glm::vec2>> vertexTexCoords(4);
            for (size_t i = 0; i < vertexTexCoords.size(); ++i) {
                const std::string attributeName = "TEXCOORD_" + std::to_string(i);

                if (model.meshes[meshIndex].primitives[submeshIndex].attributes.contains(attributeName)) {
                    vertexTexCoords[i] = axrGetGltfBufferData<glm::vec2>(
                        model,
                        model.meshes[meshIndex].primitives[submeshIndex].attributes[attributeName]
                    );
                }
            }

            submesh.Vertices.resize(vertexPositions.size());
            for (size_t vertexIndex = 0; vertexIndex < vertexPositions.size(); ++vertexIndex) {
                submesh.Vertices[vertexIndex].Position = modelMatrix * glm::vec4(vertexPositions[vertexIndex], 1.0f);
                submesh.Vertices[vertexIndex].Color = !vertexColors.empty()
                                                          ? vertexColors[vertexIndex]
                                                          : glm::vec4(1.0f);
                submesh.Vertices[vertexIndex].TexCoord_0 = !vertexTexCoords[0].empty()
                                                               ? vertexTexCoords[0][vertexIndex]
                                                               : glm::vec2(0.0f);
                submesh.Vertices[vertexIndex].TexCoord_1 = !vertexTexCoords[1].empty()
                                                               ? vertexTexCoords[1][vertexIndex]
                                                               : glm::vec2(0.0f);
                submesh.Vertices[vertexIndex].TexCoord_2 = !vertexTexCoords[2].empty()
                                                               ? vertexTexCoords[2][vertexIndex]
                                                               : glm::vec2(0.0f);
                submesh.Vertices[vertexIndex].TexCoord_3 = !vertexTexCoords[3].empty()
                                                               ? vertexTexCoords[3][vertexIndex]
                                                               : glm::vec2(0.0f);
            }
        }
    }

    return AXR_SUCCESS;
}


AxrResult axrGetModelFileInfo_glTF(
    const std::filesystem::path& path,
    AxrModelFileInfo* modelFileInfo
) {
    // ----------------------------------------- //
    // Validation
    // ----------------------------------------- //

    if (modelFileInfo == nullptr) {
        axrLogErrorLocation("model file info is null.");
        return AXR_ERROR;
    }

    // ----------------------------------------- //
    // Process
    // ----------------------------------------- //

    AxrResult axrResult = AXR_SUCCESS;

    tinygltf::Model model;
    axrResult = axrReadGltfFile(path, model);
    if (AXR_FAILED(axrResult)) {
        return axrResult;
    }

    // glTF Reference Guide https://www.khronos.org/files/gltf20-reference-guide.pdf

    modelFileInfo->ImageSamplerCount = model.samplers.size();
    modelFileInfo->ImageSamplers = new AxrModelFileImageSamplerInfo[modelFileInfo->ImageSamplerCount]{};
    for (size_t i = 0; i < model.samplers.size(); ++i) {
        AxrImageSamplerFilterEnum filterPlaceholder{};

        strncpy_s(
            modelFileInfo->ImageSamplers[i].Name,
            model.samplers[i].name.c_str(),
            AXR_MAX_ASSET_NAME_SIZE
        );
        modelFileInfo->ImageSamplers[i].MinFilter = axrToImageSamplerFilter(
            model.samplers[i].minFilter,
            modelFileInfo->ImageSamplers[i].MipmapFilter
        );
        modelFileInfo->ImageSamplers[i].MagFilter = axrToImageSamplerFilter(
            model.samplers[i].magFilter,
            filterPlaceholder
        );
        modelFileInfo->ImageSamplers[i].WrapU = axrToImageSamplerWrap(model.samplers[i].wrapS);
        modelFileInfo->ImageSamplers[i].WrapV = axrToImageSamplerWrap(model.samplers[i].wrapT);
    }

    modelFileInfo->ImageCount = model.images.size();
    modelFileInfo->Images = new AxrModelFileImageInfo[modelFileInfo->ImageCount]{};
    for (size_t i = 0; i < model.images.size(); ++i) {
        strncpy_s(
            modelFileInfo->Images[i].Name,
            model.images[i].name.c_str(),
            AXR_MAX_ASSET_NAME_SIZE
        );
        strncpy_s(
            modelFileInfo->Images[i].FilePath,
            path.parent_path().append(model.images[i].uri).string().c_str(),
            AXR_MAX_FILE_PATH_SIZE
        );
    }

    modelFileInfo->MaterialCount = model.materials.size();
    modelFileInfo->Materials = new AxrModelFileMaterialInfo[modelFileInfo->MaterialCount]{};
    for (size_t i = 0; i < model.materials.size(); ++i) {
        strncpy_s(
            modelFileInfo->Materials[i].Name,
            model.materials[i].name.c_str(),
            AXR_MAX_ASSET_NAME_SIZE
        );

        const int colorTextureIndex = model.materials[i].pbrMetallicRoughness.baseColorTexture.index;
        // The index is -1 if there's no texture
        if (colorTextureIndex >= 0) {
            modelFileInfo->Materials[i].ColorImageIndex = model.textures[colorTextureIndex].source;
            modelFileInfo->Materials[i].ColorImageSamplerIndex = model.textures[colorTextureIndex].sampler;
        } else {
            modelFileInfo->Materials[i].ColorImageIndex = -1;
            modelFileInfo->Materials[i].ColorImageSamplerIndex = -1;
        }

        const std::vector<double>& colorFactorVector = model.materials[i].pbrMetallicRoughness.baseColorFactor;
        modelFileInfo->Materials[i].ColorFactor = glm::vec4(
            colorFactorVector[0],
            colorFactorVector[1],
            colorFactorVector[2],
            colorFactorVector[3]
        );
    }

    modelFileInfo->MeshCount = model.meshes.size();
    modelFileInfo->Meshes = new AxrModelFileMeshInfo[modelFileInfo->MeshCount]{};
    for (size_t i = 0; i < model.meshes.size(); ++i) {
        modelFileInfo->Meshes[i].SubmeshCount = model.meshes[i].primitives.size();
        modelFileInfo->Meshes[i].Submeshes = new AxrModelFileSubmeshInfo[modelFileInfo->Meshes[i].SubmeshCount]{};

        for (size_t j = 0; j < model.meshes[i].primitives.size(); ++j) {
            modelFileInfo->Meshes[i].Submeshes[j].MaterialIndex = model.meshes[i].primitives[j].material;
        }
    }

    return AXR_SUCCESS;
}

size_t axrGetGltfComponentByteSize(const int accessorComponentType) {
    switch (accessorComponentType) {
        case TINYGLTF_COMPONENT_TYPE_BYTE: {
            return sizeof(uint8_t);
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE: {
            return sizeof(uint8_t);
        }
        case TINYGLTF_COMPONENT_TYPE_SHORT: {
            return sizeof(int16_t);
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT: {
            return sizeof(uint16_t);
        }
        case TINYGLTF_COMPONENT_TYPE_INT: {
            return sizeof(int32_t);
        }
        case TINYGLTF_COMPONENT_TYPE_UNSIGNED_INT: {
            return sizeof(uint32_t);
        }
        case TINYGLTF_COMPONENT_TYPE_FLOAT: {
            return sizeof(float);
        }
        case TINYGLTF_COMPONENT_TYPE_DOUBLE: {
            return sizeof(double);
        }
        default: {
            axrLogErrorLocation("Unknown glTF accessor component type.");
            return 0;
        }
    }
}

size_t axrGetGltfComponentCount(const int accessorType) {
    switch (accessorType) {
        case TINYGLTF_TYPE_VEC2: {
            return 2;
        }
        case TINYGLTF_TYPE_VEC3: {
            return 3;
        }
        case TINYGLTF_TYPE_VEC4: {
            return 4;
        }
        case TINYGLTF_TYPE_MAT2: {
            return 2;
        }
        case TINYGLTF_TYPE_MAT3: {
            return 3;
        }
        case TINYGLTF_TYPE_MAT4: {
            return 4;
        }
        case TINYGLTF_TYPE_SCALAR: {
            return 1;
        }
        case TINYGLTF_TYPE_VECTOR: {
            return 4;
        }
        case TINYGLTF_TYPE_MATRIX: {
            return 16;
        }
        default: {
            axrLogErrorLocation("Unknown glTF accessor type.");
            return 0;
        }
    }
}
