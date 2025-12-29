#include "Field.h"
#include <fstream>
#include <algorithm>
#include <Windows.h>
#include <vector>

bool Field::Initialize(ID3D11Device* device, std::string jsonPath) {
    std::ifstream file(jsonPath);
    if (!file.is_open()) {
        return false;
    }

    nlohmann::json data;
    try {
        data = nlohmann::json::parse(file);
    } catch (const std::exception& e) {
        return false;
    }

    for (auto& tex : data["texturas"].items()) {
        std::string path = tex.value();
        std::wstring wPath(path.begin(), path.end());

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
        DirectX::CreateWICTextureFromFile(device, wPath.c_str(), nullptr, &srv);
        m_textures[tex.key()] = srv;
    }

    for (auto& item : data["layout"]) {
        MapItem el;
        el.id = item["id"];
        el.pos = { (float)item["x"], (float)item["y"], (float)item["z"] };

        el.width = (float)item["w"];
        el.height = (float)item["h"];

        m_layout.push_back(el);
    }

    std::sort(m_layout.begin(), m_layout.end(), [](const MapItem& a, const MapItem& b) {
        return a.pos.z > b.pos.z;
        });

    return true;
}

void Field::Render(DirectX::SpriteBatch* batch) {
    for (auto& item : m_layout) {
        auto textureIt = m_textures.find(item.id);
        if (textureIt != m_textures.end()) {
            auto texture = textureIt->second.Get();
            if (texture) {
                RECT destRect = {
                    (long)item.pos.x,
                    (long)item.pos.y,
                    (long)(item.pos.x + item.width),
                    (long)(item.pos.y + item.height)
                };

                batch->Draw(texture, destRect, DirectX::Colors::White);
            } else {
                OutputDebugStringA(("Warning: Texture '" + item.id + "' is null\n").c_str());
            }
        } else {
            OutputDebugStringA(("Warning: Texture '" + item.id + "' not found in texture map\n").c_str());
        }
    }
}

void Field::SetupViewport(ID3D11DeviceContext* context) {

}

Field::Field()
{

}

Field::~Field()
{

}