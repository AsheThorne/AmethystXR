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
- Try to avoid using std::string where possible