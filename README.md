# FF Panel — Android NDK Mod

Free Fire mod panel — Sketchware Pro / Android NDK source.

## Stack
- **Language:** C++ (NDK), Java
- **Rendering:** ImGui (OpenGL3 + Android backend)
- **Hooking:** Dobby, KittyMemory, And64InlineHook, Substrate
- **IL2Cpp:** XINOX / IL2CppSDKGenerator
- **Networking:** libcurl + OpenSSL

## Structure
```
app/
└── src/main/
    ├── AndroidManifest.xml
    ├── jni/
    │   ├── NEXO.cpp          ← main entry
    │   ├── NEXO.h
    │   ├── NEXO MODS/        ← hack features
    │   ├── XINOX/            ← IL2Cpp + tools
    │   ├── Unity/            ← Unity SDK structs
    │   ├── KittyMemory/      ← memory patching
    │   ├── imgui/            ← GUI rendering
    │   ├── Dobby/            ← hook library
    │   ├── include/          ← Substrate, And64InlineHook
    │   └── xdl/              ← dynamic linker utils
    ├── jniLibs/              ← pre-built libNEXO.so
    └── res/                  ← app resources
```

## Build
Open in Sketchware Pro → Build.
For NDK: `ndk-build` inside `app/src/main/jni/`.

---
*Rename package, app name, and branding before release.*
