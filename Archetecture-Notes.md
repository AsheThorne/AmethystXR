# Architecture Notes

## Engine design pattern

Public API example

```c++
// ---- General functions ----
AxrResult axrSetup(const Config& config);
void axrShutdown();

// ---- Application related functions ----
void axrApplicationProcessEvents();
float axrApplicationGetDeltaTime();
```

Implementation example

```c++
AxrResult axrSetup(const Config& config) {    
    if (AXR_FAILED(axr::Application::get().setup(
        Application::Config{...}
    )) {
        // Probably log a trace of some kind
        return AXR_ERROR;
    }
    if (AXR_FAILED(axr::PlatformLayer::get().setup(
        PlatformLayer::Config{...}
    )) {
        // Probably log a trace of some kind
        return AXR_ERROR;
    }
    if (AXR_FAILED(axr::Renderer::get().setup(
        Renderer::Config{...}
    )) {
        // Probably log a trace of some kind
        return AXR_ERROR;
    }
    if (AXR_FAILED(axr::XrSystem::get().setup(
        XrSystem::Config{...}
    )) {
        // Probably log a trace of some kind
        return AXR_ERROR;
    }
    ...
}

void axrShutdown() {
    axr::XrSystem::get().shutdown();
    axr::Renderer::get().shutdown();
    axr::PlatformLayer::get().shutdown();
    axr::Application::get().shutdown();
}

void axrApplicationProcessEvents() {
    axr::Application::get().processEvents();
}

float axrApplicationGetDeltaTime() {
    return axr::Application::get().getDeltaTime();
}

class axr::Application {
public:    
    struct Config {
        ...
    };
    
    static axr::Application& get() {
        return m_Singleton;
    }
    
    AxrResult setup(const Application::Config& config) {
        assert(!m_IsSetup)
        ...
    }
    
    void shutdown() {
        ...
    }
    
    void processEvents() {
        assert(m_IsSetup)
        ...
    }

    float getDeltaTime() {
        assert(m_IsSetup)
        ...
    }

private:
    static axr::Application m_Singleton;
    bool m_IsSetup;
    
    // Constructor
    axr::Application() {
        // Do nothing
    }
    // Destructor
    ~axr::Application() {
        // Do nothing
    }
};

class axr::PlatformLayer {
public:
    struct Config {
        ...
    };
    
    static axr::PlatformLayer& get() {
        return m_Singleton;
    }
    
    AxrResult setup(const Config& config) {
        assert(!m_IsSetup)
        ...
    }
    
    void shutDown() {
        ...
    }
    
private:
    static axr::PlatformLayer m_Singleton;
    bool m_IsSetup;
    
    // Constructor
    axr::PlatformLayer() {
        // Do nothing
    }
    // Destructor
    ~axr::PlatformLayer() {
        // Do nothing
    }
};
```

## System Hierarchy

A module must NEVER access anything above or next to themselves in the following chart, only below.
```
 -------------------------------------------------------------------------
|                   Public API (public include headers)                   |
 -------------------------------------------------------------------------
 -------------------------------------------------------------------------
|                               Application                               |
 -------------------------------------------------------------------------
 -------------------------------------------------------------------------
|                             Graphics System                             |
 -------------------------------------------------------------------------
 -----------------------------------   -----------------------------------
|         Resource System           | |          Action System            |
 -----------------------------------   -----------------------------------
 -----------------------------------   -----------------------------------
|         Platform System           | |            Xr System              |
 -----------------------------------   -----------------------------------
 -------------------------------------------------------------------------
|                              Memory System                              |
 -------------------------------------------------------------------------
```

## General Notes

- The game world object model, renderer, HID, player control system and animations are all effected by a multiplayer
  system. So we should design the engine with multiplayer in mind from the start.
- Create wrappers for third party libraries
    - The main reason is it makes it easier to swap things out if we decide to later.
- Build a custom memory management system
- Build the renderer decoupled from everything. Unlike V2 of AmethystXR. It should take in a surface to render too and
  assets to draw as inputs.
- Platform specific stuff should follow the pattern of: One header file defined and each platform gets its own file.
  Then the make file decides which file to compile based on the platform.
- Look into compressing animation data
- Make use of attributes. Especially these ones:
    - [[fallthrough]]
    - [[maybe_unused]]
    - [[nodiscard]]
    - [[likely]]/[[unlikely]]
- Use [`concepts`](https://en.cppreference.com/w/cpp/language/constraints.html) to limit template usage

## Specific Notes

### UI

- Define a canvas with a size in world units (eg. 1 meter height by 1.5 meter width), along with a `pixels per meter`
  int value. The `pixels per meter` value defines the resolution.
    - UNSURE ON THIS BIT: Render the UI to an offscreen buffer, then use the output as an image on the UI canvas in
      world