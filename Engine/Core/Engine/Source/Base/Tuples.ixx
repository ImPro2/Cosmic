module;
#include "cspch.hpp"
export module Cosmic.Base.Tuples;

import Cosmic.Base.Types;

namespace Cosmic
{

    export template<typename T, size_t N>
    struct Tuple
    {
    };

    export template<>
    struct Tuple<float32, 2>
    {
        union { float32 x, r, width;  };
        union { float32 y, g, height; };
    };

    export template<>
    struct Tuple<float32, 3>
    {
        union { float32 x, r, width;  };
        union { float32 y, g, height; };
        union { float32 z, b, depth;  };
    };

    export template<>
    struct Tuple<float32, 4>
    {
        union { float32 x, r; };
        union { float32 y, g; };
        union { float32 z, b; };
        union { float32 w, a; };
    };

    export template<>
    struct Tuple<int32, 2>
    {
        union { int32 x, r, width;  };
        union { int32 y, g, height; };
    };

    export template<>
    struct Tuple<int32, 3>
    {
        union { int32 x, r, width;  };
        union { int32 y, g, height; };
        union { int32 z, b, depth;  };
    };

    export template<>
    struct Tuple<int32, 4>
    {
        union { int32 x, r; };
        union { int32 y, g; };
        union { int32 z, b; };
        union { int32 w, a; };
    };

    export template<>
    struct Tuple<uint32, 2>
    {
        union { uint32 x, r, width; };
        union { uint32 y, g, height; };
    };

    export template<>
    struct Tuple<uint32, 3>
    {
        union { uint32 x, r, width; };
        union { uint32 y, g, height; };
        union { uint32 z, b, depth; };
    };

    export template<>
    struct Tuple<uint32, 4>
    {
        union { uint32 x, r; };
        union { uint32 y, g; };
        union { uint32 z, b; };
        union { uint32 w, a; };
    };

    export using float2 = Tuple<float32, 2>;
    export using float3 = Tuple<float32, 3>;
    export using float4 = Tuple<float32, 4>;
    export using int2   = Tuple<int32,   2>;
    export using int3   = Tuple<int32,   3>;
    export using int4   = Tuple<int32,   4>;
    export using uint2  = Tuple<uint32,  2>;
    export using uint3  = Tuple<uint32,  3>;
    export using uint4  = Tuple<uint32,  4>;
}