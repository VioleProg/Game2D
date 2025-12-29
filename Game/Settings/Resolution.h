#pragma once
#include <SimpleMath.h>
#include <algorithm>

namespace Resolution {
    const float VirtualWidth = 1920.0f;
    const float VirtualHeight = 1080.0f;

    const float MinWidth = 800.0f;
    const float MinHeight = 600.0f;
    const float MaxWidth = 1920.0f;
    const float MaxHeight = 1080.0f;

    inline void GetEffectiveResolution(float windowWidth, float windowHeight, float& effectiveWidth, float& effectiveHeight) {
        effectiveWidth = (std::max)(MinWidth, (std::min)(MaxWidth, windowWidth));
        effectiveHeight = (std::max)(MinHeight, (std::min)(MaxHeight, windowHeight));
    }

    inline DirectX::SimpleMath::Matrix GetTransformMatrix(float windowWidth, float windowHeight) {
        return DirectX::SimpleMath::Matrix::Identity;
    }

}