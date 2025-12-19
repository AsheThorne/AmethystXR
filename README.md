# AmethystXR

An OpenXR focused game engine written in c++

## Visual Studio Code setup

### Required Extensions

- [C/C++ by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cpptools)
- [Make Tools by Microsoft](https://marketplace.visualstudio.com/items?itemName=ms-vscode.cmake-tools)
- [Clang-Format by Xaver Hellauer](https://marketplace.visualstudio.com/items?itemName=xaver.clang-format)

### Troubleshooting

If you get a clang format error that looks like this:
`The 'clang-format' command is not available. Please check your clang-format.executable user setting and ensure it is installed.`
Make sure you have clang installed on your machine.

## Linux

### Troubleshooting

If console color isn't working, make sure you have the $TERM env variable set. You can set it globally, within
`/etc/environment` like so, `TERM=xterm-256color`.

## Duel booting platforms with Clion

If you're duel booting multiple platforms (like linux and windows) and using a JetBrains IDE like Clion, it's a good
idea to set up a symlink for the `.idea/workspace.xml` file. That way you can keep configurations separate between
platforms.

1. Create a copy of the `./idea/workspace.xml` file for each platform and rename it to `workspace-windows.xml`/
   `workspace-linux.xml`.
2. Anytime you switch platforms, run the `fix-jetbrains-workspace-<platform>` file for the platform you've switched to.
   This will create a symlink for the `workspace.xml` file to the platforms respective workspace file.