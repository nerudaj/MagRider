#pragma once

#include <DGM/classes/Compatibility.hpp>

#ifdef ANDROID
#include <fmt/core.h>
#include <range/v3/all.hpp>
#else
#include <ranges>
#include <format>
#endif

#ifdef ANDROID
namespace uniranges = ranges;
#else
namespace uniranges = std::ranges;
#endif

namespace uni
{
#ifdef ANDROID
    using namespace fmt;
    namespace views {
        using namespace ranges::views;
    }

    namespace ranges {
        using namespace ::ranges;
    }
#else
    using namespace std;
#endif
}

template<class Container>
Container collect(auto&& range)
{
    return Container(range.begin(), range.end());
}
