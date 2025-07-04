## Rules to follow for engine development

Src files (files in the `src` directory) should ONLY include headers from the `include/axr` directory.
NEVER from the `include/axr-hpp` directory.

The `include/axr-hpp` directory is ONLY for wrapping functions in `include/axr` into a c++ friendly style.
For example, wrapping functions into namespaces and classes.

Everything in `include/axr-hpp` is header only. No definitions in a `.cpp` file.

The `include/axr` directory MUST ONLY contain `.h` files. These files MUST be valid c code if they're exporting from the
axr library. They can contain c++ code if they are header only and just wrappers for libraries like spdlog.

The `include/axr-hpp` directory MUST ONLY contain `.hpp` files.

The `include` directory is for public facing header files only.
Any header file which isn't accessed by the application, must go inside the `src` directory.

- Avoid `dynamic_cast`
- Minimal polymorphism. Only for small things with minimal impact on the engine

Avoid putting `#ifdef` preprocessors around enum values. The available enums shouldn't change depending on platform or
graphics api or anything else really.

For the AXR API (include headers), use `const char*` for strings that don't need to be stored.
Use string arrays, like `char ApplicationName[AXR_MAX_APPLICATION_NAME_SIZE]` for strings
that will be stored.

### Definitions

`cleanup` functions are reserved for cleaning up the entire class.
These are typically only used in destructors, move and copy functions.

`reset` functions are for resetting another function back to before it was called.
For example, `resetSetup()` will destroy anything that was created in the `setup()` function and everything that
relies on the objects created in the `setup()`.