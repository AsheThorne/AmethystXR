#pragma once

/// Shader buffer scope enum
enum AxrShaderBufferScopeEnum {
    AXR_SHADER_BUFFER_SCOPE_UNDEFINED = 0,
    AXR_SHADER_BUFFER_SCOPE_SCENE,
    AXR_SHADER_BUFFER_SCOPE_MATERIAL,
    AXR_SHADER_BUFFER_SCOPE_MODEL,
};

/// Engine asset shader properties
struct AxrShaderEngineAssetProperties {
    const char* Name;
    AxrShaderBufferScopeEnum Scope;
};

/// Get the scope of the named buffer engine asset
/// @param bufferName Buffer name
/// @returns Buffer scope
AxrShaderBufferScopeEnum axrGetBufferEngineAssetScope(const char* bufferName);