#include <iostream>

#include "assets/Asset.h"
#include "assets/AssetManager.h"
#include "memory/Memory.h"
#include "memory/StackAllocator.h"
#include "memory/PoolAllocator.h"

int main() {
    const void* top = RootAllocator.GetTop();

    Assets = (AssetManager*)RootAllocator.Allocate(sizeof(AssetManager));
    new (Assets) AssetManager();

    PrintMemoryStats();

    // Print asset
    Asset* vsAsset = Assets->GetAsset(STRINGHASH("assets/shaders/SpriteVertexShader.glsl"));
    if (vsAsset == nullptr) {
        printf("ERROR: Couldn't find VS asset!\n");
    }
    else {
        printf("VS asset: '%.*s'\n", (int)vsAsset->GetDataLength(), (const char*)vsAsset->GetData());
    }

    RootAllocator.Pop(top);

    PrintMemoryStats();

    std::cout << "Hello World!";
    return 5;
}