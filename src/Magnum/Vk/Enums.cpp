/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
                2020, 2021 Vladimír Vondruš <mosra@centrum.cz>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "Enums.h"

#include <Corrade/Containers/ArrayView.h>

#include "Magnum/Mesh.h"
#include "Magnum/Sampler.h"

#ifdef MAGNUM_BUILD_DEPRECATED
#include "Magnum/Vk/PixelFormat.h"
#include "Magnum/Vk/VertexFormat.h"
#endif

namespace Magnum { namespace Vk {

namespace {

constexpr VkPrimitiveTopology PrimitiveTopologyMapping[]{
    VK_PRIMITIVE_TOPOLOGY_POINT_LIST,
    VK_PRIMITIVE_TOPOLOGY_LINE_LIST,
    VkPrimitiveTopology(~UnsignedInt{}),
    VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,
    VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,
    VkPrimitiveTopology(~UnsignedInt{}), /* Instances */
    VkPrimitiveTopology(~UnsignedInt{}), /* Faces */
    VkPrimitiveTopology(~UnsignedInt{})  /* Edges */
};

constexpr VkIndexType IndexTypeMapping[]{
    VK_INDEX_TYPE_UINT8_EXT,
    VK_INDEX_TYPE_UINT16,
    VK_INDEX_TYPE_UINT32
};

constexpr VkFilter FilterMapping[]{
    VK_FILTER_NEAREST,
    VK_FILTER_LINEAR
};

constexpr VkSamplerMipmapMode SamplerMipmapModeMapping[]{
    VK_SAMPLER_MIPMAP_MODE_NEAREST, /* See vkSamplerMipmapMode() for details */
    VK_SAMPLER_MIPMAP_MODE_NEAREST,
    VK_SAMPLER_MIPMAP_MODE_LINEAR
};

constexpr VkSamplerAddressMode SamplerAddressModeMapping[]{
    VK_SAMPLER_ADDRESS_MODE_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_MIRRORED_REPEAT,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_EDGE,
    VK_SAMPLER_ADDRESS_MODE_CLAMP_TO_BORDER,
    VK_SAMPLER_ADDRESS_MODE_MIRROR_CLAMP_TO_EDGE,
};

}

bool hasVkPrimitiveTopology(const Magnum::MeshPrimitive primitive) {
    if(isMeshPrimitiveImplementationSpecific(primitive))
        return true;

    CORRADE_ASSERT(UnsignedInt(primitive) - 1 < Containers::arraySize(PrimitiveTopologyMapping),
        "Vk::hasVkPrimitiveTopology(): invalid primitive" << primitive, {});
    return UnsignedInt(PrimitiveTopologyMapping[UnsignedInt(primitive) - 1]) != ~UnsignedInt{};
}

VkPrimitiveTopology vkPrimitiveTopology(const Magnum::MeshPrimitive primitive) {
    if(isMeshPrimitiveImplementationSpecific(primitive))
        return meshPrimitiveUnwrap<VkPrimitiveTopology>(primitive);

    CORRADE_ASSERT(UnsignedInt(primitive) - 1 < Containers::arraySize(PrimitiveTopologyMapping),
        "Vk::vkPrimitiveTopology(): invalid primitive" << primitive, {});
    const VkPrimitiveTopology out = PrimitiveTopologyMapping[UnsignedInt(primitive) - 1];
    CORRADE_ASSERT(out != VkPrimitiveTopology(~UnsignedInt{}),
        "Vk::vkPrimitiveTopology(): unsupported primitive" << primitive, {});
    return out;
}

bool hasVkIndexType(const Magnum::MeshIndexType type) {
    CORRADE_ASSERT(UnsignedInt(type) - 1 < Containers::arraySize(IndexTypeMapping),
        "Vk::hasVkIndexType(): invalid type" << type, {});
    return UnsignedInt(IndexTypeMapping[UnsignedInt(type) - 1]) != ~UnsignedInt{};
}

VkIndexType vkIndexType(const Magnum::MeshIndexType type) {
    CORRADE_ASSERT(UnsignedInt(type) - 1 < Containers::arraySize(IndexTypeMapping),
        "Vk::vkIndexType(): invalid type" << type, {});
    const VkIndexType out = IndexTypeMapping[UnsignedInt(type) - 1];
    CORRADE_ASSERT(out != VkIndexType(~UnsignedInt{}),
        "Vk::vkIndexType(): unsupported type" << type, {});
    return out;
}

#ifdef MAGNUM_BUILD_DEPRECATED
bool hasVkFormat(const Magnum::VertexFormat format) {
    return hasVertexFormat(format);
}

bool hasVkFormat(const Magnum::PixelFormat format) {
    return hasPixelFormat(format);
}

bool hasVkFormat(const Magnum::CompressedPixelFormat format) {
    return hasPixelFormat(format);
}
#endif

#ifdef MAGNUM_BUILD_DEPRECATED
VkFormat vkFormat(const Magnum::VertexFormat format) {
    return VkFormat(vertexFormat(format));
}

VkFormat vkFormat(const Magnum::PixelFormat format) {
    return VkFormat(pixelFormat(format));
}

VkFormat vkFormat(const Magnum::CompressedPixelFormat format) {
    return VkFormat(pixelFormat(format));
}
#endif

VkFilter vkFilter(const Magnum::SamplerFilter filter) {
    CORRADE_ASSERT(UnsignedInt(filter) < Containers::arraySize(FilterMapping),
        "Vk::vkFilter(): invalid filter" << filter, {});
    return FilterMapping[UnsignedInt(filter)];
}

VkSamplerMipmapMode vkSamplerMipmapMode(const Magnum::SamplerMipmap mipmap) {
    CORRADE_ASSERT(UnsignedInt(mipmap) < Containers::arraySize(SamplerMipmapModeMapping),
        "Vk::vkSamplerMipmapMode(): invalid mode" << mipmap, {});
    return SamplerMipmapModeMapping[UnsignedInt(mipmap)];
}

bool hasVkSamplerAddressMode(const Magnum::SamplerWrapping wrapping) {
    CORRADE_ASSERT(UnsignedInt(wrapping) < Containers::arraySize(SamplerAddressModeMapping),
        "Vk::hasVkSamplerAddressMode(): invalid wrapping" << wrapping, {});
    return UnsignedInt(SamplerAddressModeMapping[UnsignedInt(wrapping)]) != ~UnsignedInt{};
}

VkSamplerAddressMode vkSamplerAddressMode(const Magnum::SamplerWrapping wrapping) {
    CORRADE_ASSERT(UnsignedInt(wrapping) < Containers::arraySize(SamplerAddressModeMapping),
        "Vk::vkSamplerAddressMode(): invalid wrapping" << wrapping, {});
    const VkSamplerAddressMode out = SamplerAddressModeMapping[UnsignedInt(wrapping)];
    CORRADE_ASSERT(out != VkSamplerAddressMode(~UnsignedInt{}),
        "Vk::vkSamplerAddressMode(): unsupported wrapping" << wrapping, {});
    return out;
}

}}
