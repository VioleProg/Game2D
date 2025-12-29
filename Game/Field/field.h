#pragma once
#include <d3d11.h>
#include <SimpleMath.h>
#include <SpriteBatch.h>
#include <WICTextureLoader.h>
#include <vector>
#include <map>
#include <string>
#include <nlohmann/json.hpp>
#include <wrl.h>

struct MapItem {
    std::string id;
    DirectX::SimpleMath::Vector3 pos; // X, Y e Z (profundidade)
    float width, height;
};

class Field {
public:
    Field();
    ~Field();

    bool Initialize(ID3D11Device* device, std::string jsonPath);

    void Render(DirectX::SpriteBatch* batch);
    void SetupViewport(ID3D11DeviceContext* context);

private:
    std::map<std::string, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView>> m_textures;
    std::vector<MapItem> m_layout;
};