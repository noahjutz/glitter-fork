# Glitter Fork

For more information about Glitter, see https://github.com/Polytonic/Glitter.

## Getting Started

```sh
# Build
cmake -B Build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DASSIMP_BUILD_ZLIB=ON .

# Compile
cmake --build Build -j$(nproc) &&

# Run
./Build/Glitter/Glitter
```

Flags:
- `CMAKE_EXPORT_COMPILE_COMMANDS`: Creates compile_commands.json for LSP.
- `ASSIMP_BUILD_ZLIB`: Fixes a compilation error ([issue](https://github.com/assimp/assimp/issues/6542)).
