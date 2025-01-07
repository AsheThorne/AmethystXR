## Rules to follow for engine development

Src files (files in the `src` directory) should ONLY include headers from the `include/axr` directory.
NEVER from the `include/axr-hpp` directory.

The `include/axr-hpp` directory is ONLY for wrapping functions in `include/axr` into a c++ friendly style.
For example, wrapping functions into namespaces and classes.

Everything in `include/axr-hpp` is header only. No definitions in a `.cpp` file.

The `include/axr` directory MUST ONLY contain `.h` files. These files MUST be valid c code.
No c++ specific code can be here.

The `include/axr-hpp` directory MUST ONLY contain `.hpp` files.

The `include` directory is for public facing header files only.
Any header file which isn't accessed by the application, must go inside the `src` directory. 