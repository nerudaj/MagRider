#pragma once

#include <nlohmann/json.hpp>

struct [[nodiscard]] FeatureFlags final
{
    bool showInputSettings =
#ifdef _DEBUG
        true;
#else
        false;
#endif
};

NLOHMANN_DEFINE_TYPE_NON_INTRUSIVE(FeatureFlags, showInputSettings);
