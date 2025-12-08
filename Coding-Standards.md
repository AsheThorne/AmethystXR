# Coding Standards

- Functions for the axr library api (generally all functions in the include folder)
    - MUST be prefixed with axr.
    - SHOULD be prefixed with the class/category the function falls under. E.g. `axrLoggerSetup()`, logger is the
      class/category for this function.
- AxrResult enums
    - Errors MUST be prefixed with `AXR_ERROR_`
        - If any return code is not prefixed with `AXR_ERROR_`, then it's assumed to not cause much issue and probably
          won't need to be handled.