# Architecture Notes

## Engine design pattern

### Public API example

```c++
// ---- General functions ----
AxrResult axrSetup(const Config& config);
void axrShutdown();

// ---- Application related functions ----
void axrApplicationProcessEvents();
float axrApplicationGetDeltaTime();
```

### Implementation example

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

### Possible Server Interaction Example

```c++
// NOTE (Ashe):
//  Because there will be 2 entt registries (1 on the server, 1 on the client), the default entity IDs are unlikely to be the
//  same. Espeically if there are multiple players and the server needs to keep track of more than the client. So we need
//  another ID for each entity. Each entity will get an immutable UUID and each registry will need a map to link between
//  the UUID and the entt ID (map<UUID, entt::entity>).
//  Any communication between the server and the client must use the UUID. Which should be stored within a component
//  (possibly a component just called metadata). And then convert the UUID to the entt::entity for the respected registry.

// NOTE: entity.isSynced should probably be part of the metadata component too

AxrScene.updateComponent(entity, component) {
    // Update the component in the registry
    entityRegistry.patch(entity, component);
    
    // Check if the component should send updates to the server.
    // We probably don't need an 'entity.isSynced' check. Instead, if an entity is marked as not synced, every component
    // added to it should also be set to not synced.
    if (entity.isSynced && component.isSynced) {
        // Add this new updated component to a queue on the server to be sent out on the next tick.
        //
        // Probably need some way to detect if the given component has already been queued for this entity
        // and to overwrite it. Only use the latest changes to a component to prevent sending more data than we need to.
        // Make sure that queueComponentUpdate has the if(IsServerConnected) guard inside.
        // Do the same for add and remove component functions.
        //
        // If the component already exists in the queueAddComponent list, then just change it's data with this component's
        // data inside the queueAddComponent list.
        //
        // If the component already exists in the queueRemoveComponent list, do nothing and log an error
        AxrServer.queueUpdateComponent(sceneID, entity, component);
    }
}

AxrScene.addComponent(entity, component) {
    // Add the component to the registry
    entityRegistry.add(entity, component);
    
    // TODO: Do we just have an isSynced option on each component?? i don't like it being part of the component itself.
    //  I don't want it to be editable after creation and the server obviously doesn't need to store that since it'll always be true.
    //  But I can't think of a different way to do it nicely.
    //  One option could be that the isSynced is only tied to entities. if an entity is synced, all it's data is.
    //  It just feels like there would be times where we don't care to sync certain components
    //  Another option: Maybe we can use 'concepts' to check for components with the 'isSynced' property. if it doesn't have it, it won't
    //  do any server interaction. if it has it, it will check if it's true. and if it is, it'll strip that data out before
    //  sending it to the server.
    if (component.isSynced) {
        // Add this new component to a queue on the server to be sent out on the next tick.
        //
        // If this component already exists in the queueRemoveComponent list, then remove it from the queueRemoveComponent
        // list, and add it to the queueUpdateComponent list.
        //
        // If this component already exists in the queueUpdateComponent list, then do nothing and log an error.
        //
        // And on the server side, if the component already exists for the given entity, then just update it to this new value.
        AxrServer.queueAddComponent(sceneID, entity, component);
    }
}

AxrScene.removeComponent(entity, component) {
    entityRegistry.remove(entity, component);
    
    if (component.isSynced) {
        // Add this component to a queue to be deleted on the server side on the next tick.
        //
        // If this component already exists in the queueAddComponent list, then remove it from the queueAddComponent
        // list.
        //
        // If this component already exists in the queueUpdateComponent list, then remove it from the queueUpdateComponent
        // list.
        //
        // Inside this function, make sure to check if the component is in the queueComponentAdd list. if the same component
        // we're scheduling to add is also scheduled to be removed, then just make a call to update it with the component
        // data within the 'add' list instead.
        AxrServer.queueRemoveComponent(sceneID, entity, component);
    }
}

/// Runs on a seperate thread about 30 times a second.
AxrServer.tick() {
    if (IsServerConnected) {
        // Send added components from the client side to the server side
        sendQueuedAddedComponents()
        
        // Send updated components from the client side to the server side
        sendQueuedUpdatedComponents()
        
        // Send removed components from the client side to the server side
        sendQueuedRemovedComponents()
        
        // Receive updated components from the server size
        // TODO: Need some way to send this to the scene.
        //  One option could be we just store the components in the AxrServer class, then after this functions is done, 
        //  the scene manager (probably just the AxrApplication) will grab the data from the AxrServer and send it to the
        //  respective AxrScene.
        updatedComponents = getUpdatedComponents()
        addedComponents = getAddedComponents()
        removedComponents = getRemovedComponents()
    }
}
```

## System Hierarchy

A module must NEVER access anything above or next to themselves in the following chart, only below.

```
|-------------------------------------------------------------------------|
|                   Public API (public include headers)                   |
|-------------------------------------------------------------------------|
|-------------------------------------------------------------------------|
|                              AxrApplication                             |
|-------------------------------------------------------------------------|
|-------------------------------------------------------------------------|
|                               AxrRenderer                               |
|-------------------------------------------------------------------------|
|-----------------------------------| |-----------------------------------|
|            AxrAssets              | |            AxrActions             |
|-----------------------------------| |-----------------------------------|
|-----------------------------------| |-----------------------------------|
|           AxrPlatform             | |           AxrXRSession            |
|-----------------------------------| |-----------------------------------|
|-------------------------------------------------------------------------|
|                                AxrServer                                |
|-------------------------------------------------------------------------|
|-------------------------------------------------------------------------|
|                               AxrAllocator                              |
|-------------------------------------------------------------------------|
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
- If you allocate an AxrVector_Stack with auto-deallocation disabled, and there's a non-zero chance for the function it'
  s in to be called again before the data is manually deallocated. You should cache the result to prevent duplicates
  existing if possible.

## Specific Notes

### UI

- Define a canvas with a size in world units (eg. 1 meter height by 1.5 meter width), along with a `pixels per meter`
  int value. The `pixels per meter` value defines the resolution.
    - UNSURE ON THIS BIT: Render the UI to an offscreen buffer, then use the output as an image on the UI canvas in
      world

### Scenes

- Use a double ended stack allocator for scene assets. The upper end can hold global resources while the lower end can
  hold scene specific resources.