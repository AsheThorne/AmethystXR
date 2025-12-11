# Coding Standards

- Functions for the axr library api (generally all functions in the include folder)
    - MUST be prefixed with axr.
    - SHOULD be prefixed with the class/category the function falls under. E.g. `axrLoggerSetup()`, logger is the
      class/category for this function.
- AxrResult enums
    - Errors MUST be prefixed with `AXR_ERROR_`
        - If any return code is not prefixed with `AXR_ERROR_`, then it's assumed to not cause much issue and probably
          won't need to be handled.
    - Usually, any success code (positive code) that isn't `AXR_SUCCESS`, is a warning.
    - if a function checks an AxrResult of another function, and it causes this function to return an error too, NEVER
      blindly return the same error. For example, `AxrResult axrResult = axrSetup()`, never do `return axrResult`. be
      explicit about returning an error such as `return AXR_ERROR`. This is to prevent unrelated/unexpected error
      messages on to other functions which may be checking this one.
- Logging messages
    - It's highly recommended to define a `AXR_FUNCTION_FAILED_STRING` macro at the start of a function, and undefine it
      at the end. It's to help the readability of errors/warnings and reduce repeating the same string for multiple
      errors. Create a human-readable message in the macro to point to what function failed. And in the log message, add
      a reason for the failure. Look at `axrLoggerCreate` in `logging.h` as an example.
- If you don't care about a function's return value, then cast the function to void like so `(void)f();`. This will
  prevent warnings about unused return values. like what we see in `axrLoggerSetup()` in `logging.h`.
- Make sure to keep memory alignment in mind when defining classes/structs
    - Probably would be a good idea to be explicit about padding too (example, `uint8_t _padding[n]`)
- Keep heap allocations to a minimum and NEVER from the heap within a tight loop.
- Avoid namespaces in the engine source code. This is because we can't use them in exported functions (including return
  types and parameters). so to keep things consistent, we'll just avoid using them.
