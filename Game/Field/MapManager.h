#pragma once
#include "field.h"
#include "MapDefinitions.h"
#include <memory>

class MapManager {
public:
    static void LoadLevel(Field* field, ID3D11Device* device, std::string mapPath) {
        if (field) {
            field->Initialize(device, mapPath);
        }
    }
};