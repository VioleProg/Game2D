#include "Field.h"
#include <fstream>
#include <algorithm>

bool Field::Initialize(ID3D11Device* device, std::string jsonPath) {
    std::ifstream file(jsonPath);
    nlohmann::json data = nlohmann::json::parse(file);

    for (auto& tex : data["texturas"].items()) {
        std::string path = tex.value();
        std::wstring wPath(path.begin(), path.end());

        Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> srv;
        DirectX::CreateWICTextureFromFile(device, wPath.c_str(), nullptr, &srv);
        m_textures[tex.key()] = srv;
    }

    for (auto& item : data["layout"]) {
        m_layout.push_back({ item["id"], { (float)item["x"], (float)item["y"], (float)item["z"] } });
    }

    std::sort(m_layout.begin(), m_layout.end(), [](const MapItem& a, const MapItem& b) {
        return a.pos.z > b.pos.z;
        });

    return true;
}

void Field::Render(DirectX::SpriteBatch* batch) {
    for (auto& item : m_layout) {
        auto texture = m_textures[item.id].Get();
        if (texture) {
            batch->Draw(texture, DirectX::SimpleMath::Vector2(item.pos.x, item.pos.y), DirectX::Colors::White);
        }
    }
}

void Field::SetupViewport(ID3D11DeviceContext* context) {
    D3D11_VIEWPORT vp = {};
    vp.Width = 800.0f;
    vp.Height = 600.0f;
    vp.MaxDepth = 1.0f;
    context->RSSetViewports(1, &vp);
}

Field::Field()
{

}

Field::~Field()
{

}