# Archetecture Notes

## General Notes
- The game world object model, renderer, HID, player control system and animations are all effected by a multiplayer system. So we should design the engine with multiplayer in mind from the start.
- Create wrappers for third party libraries
    - The main reason is it makes it easier to swap things out if we decide to later.
- Build a custom memory management stystem
- Build the renderer decoupled from everything. Unlike V2 of AmethystXR. It should take in a surface to render too and assets to draw as inputs.
- Platform specific stuff should follow the pattern of: One header file defined and each platform gets its own file. Then the make file decides which file to compile based on the platform.
- Look into compressing animation data
- Make use of attributes. Especially these ones:
    - [[fallthrough]]
    - [[maybe_unused]]
    - [[nodiscard]]
    - [[likely]]/[[unlikely]]
- Use [`concepts`](https://en.cppreference.com/w/cpp/language/constraints.html) to limit template usage
- Make sure to keep memory alignment in mind when defining classes/structs
    - Probaly would be a good idea to be explicit about padding too (example, `uint8_t _padding[n]`)


## Specific Notes

### UI
- Define a canvas with a size in world units (eg. 1 meter height by 1.5 meter width), along with a `pixels per meter` int value. The `pixels per meter` value defines the resolution.
  - UNSURE ON THIS BIT: Render the UI to an offscreen buffer, then use the output as an image on the UI canvas in world