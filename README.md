# AmethystXR
An OpenXR focused game engine written in c++

## Project Setup

### Build OpenXR

Open external/OpenXR-SDK-Source in cmd

```
mkdir build
cd build
cmake -G "Visual Studio 17" -A x64 ../
```

Open OpenXR.sln and build the solution in release mode.

### Build AmethystXR

Open AmethystXR root folder in cmd

```
mkdir cmake-build-sln
cd cmake-build-sln
cmake ../
```

### Using git in Rider

Go to `Settings -> Version Control -> Directory Mappings` And
add the root `AmethystXR` directory to it. You should then have
access to the git repo within rider.

# Sandbox

The sandbox application is for using and testing the AmethystXR dll.