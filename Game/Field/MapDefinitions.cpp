#pragma once
#include <string>

namespace Maps {
    static const std::string Village = "Field/village.json";
    static const std::string Dungeon = "Field/dungeon.json";
    static const std::string Forest  = "Field/forest.json";

    // Você também pode criar uma função para facilitar a troca futura
    inline std::string GetPathById(int id) {
        switch (id) {
        case 0: return Village;
        case 1: return Dungeon;
        default: return Village;
        }
    }
}