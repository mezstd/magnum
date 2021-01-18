#ifndef Magnum_Vk_MeshLayout_h
#define Magnum_Vk_MeshLayout_h
/*
    This file is part of Magnum.

    Copyright © 2010, 2011, 2012, 2013, 2014, 2015, 2016, 2017, 2018, 2019,
                2020 Vladimír Vondruš <mosra@centrum.cz>

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

/** @file
 * @brief Class @ref Magnum::Vk::MeshLayout, enum @ref Magnum::Vk::MeshPrimitive, function @ref Magnum::Vk::hasMeshPrimitive(), @ref Magnum::Vk::meshPrimitive()
 * @m_since_latest
 */

#include <Corrade/Containers/Pointer.h>

#include "Magnum/Tags.h"
#include "Magnum/Vk/Vk.h"
#include "Magnum/Vk/Vulkan.h"
#include "Magnum/Vk/visibility.h"

namespace Magnum { namespace Vk {

/* About naming -- I wonder why Vulkan tries *so hard* to avoid naming anything
   a "mesh". It would so nicely group things togehter BUT NO, there's primitive
   topology, and vertex input state, and input assembly and ugh. */

/**
@brief Mesh primitive
@m_since_latest

Wraps a @type_vk_keyword{PrimitiveTopology}.
*/
enum class MeshPrimitive: Int {
    /* The _LIST seems too verbose and looks like Vulkan naming got inspired by
       D3D here. I'm omitting those since it's unnecessary verbosity, Metal
       doesn't have those either. GL had the naming right. */

    /** Single points. */
    Points = VK_PRIMITIVE_TOPOLOGY_POINT_LIST,

    /**
     * Each pair of vertices defines a single line, lines aren't
     * connected together.
     */
    Lines = VK_PRIMITIVE_TOPOLOGY_LINE_LIST,

    /**
     * First two vertices define first line segment, each following
     * vertex defines another segment.
     */
    LineStrip = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP,

    /** Each three vertices define one triangle. */
    Triangles = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,

    /**
     * First three vertices define first triangle, each following
     * vertex defines another triangle.
     */
    TriangleStrip = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP,

    /**
     * First vertex is center, each following vertex is connected to
     * previous and center vertex.
     * @requires_vk_feature @ref DeviceFeature::TriangleFans if the
     *      @vk_extension{KHR,portability_subset} extension is present
     */
    TriangleFan = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_FAN,

    /**
     * Lines with adjacency information.
     * @requires_vk_feature @ref DeviceFeature::GeometryShader
     */
    LinesAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_LIST_WITH_ADJACENCY,

    /**
     * Line strip with adjacency information.
     * @requires_vk_feature @ref DeviceFeature::GeometryShader
     */
    LineStripAdjacency = VK_PRIMITIVE_TOPOLOGY_LINE_STRIP_WITH_ADJACENCY,

    /**
     * Triangles with adjacency information.
     * @requires_vk_feature @ref DeviceFeature::GeometryShader
     */
    TrianglesAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST_WITH_ADJACENCY,

    /**
     * Triangle strip with adjacency information.
     * @requires_vk_feature @ref DeviceFeature::GeometryShader
     */
    TriangleStripAdjacency = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_STRIP_WITH_ADJACENCY,

    /**
     * Patches.
     * @requires_vk_feature @ref DeviceFeature::TessellationShader
     */
    Patches = VK_PRIMITIVE_TOPOLOGY_PATCH_LIST
};

/** @debugoperatorenum{MeshPrimitive} */
MAGNUM_VK_EXPORT Debug& operator<<(Debug& debug, MeshPrimitive value);

/**
@brief Check availability of a generic mesh primitive
@m_since_latest

In particular, Vulkan doesn't support the @ref MeshPrimitive::LineLoop
primitive. Returns @cpp false @ce if Vulkan doesn't support such primitive,
@cpp true @ce otherwise. Moreover, returns @cpp true @ce also for all types
that are @ref isMeshPrimitiveImplementationSpecific(). The @p primitive value
is expected to be valid.

@note Support of some types depends on presence of a particular Vulkan
    extension. Such check is outside of the scope of this function and you are
    expected to verify extension availability before using such type.

@see @ref meshPrimitive()
*/
MAGNUM_VK_EXPORT bool hasMeshPrimitive(Magnum::MeshPrimitive primitive);

/**
@brief Convert generic mesh primitive to Vulkan mesh primitive
@m_since_latest

In case @ref isMeshPrimitiveImplementationSpecific() returns @cpp false @ce for
@p primitive, maps it to a corresponding Vulkan primitive topology. In case
@ref isMeshPrimitiveImplementationSpecific() returns @cpp true @ce, assumes
@p primitive stores a Vulkan-specific primitive topology and returns
@ref meshPrimitiveUnwrap() cast to @type_vk{PrimitiveTopology}.

Not all generic mesh primitives have a Vulkan equivalent and this function
expects that given primitive is available. Use @ref hasMeshPrimitive() to query
availability of given primitive.
@see @ref vkIndexType()
*/
MAGNUM_VK_EXPORT MeshPrimitive meshPrimitive(Magnum::MeshPrimitive primitive);

/**
@brief Mesh layout
@m_since_latest

Wraps the @type_vk_keyword{VertexInputBindingDescription},
@type_vk_keyword{VertexInputAttributeDescription},
@type_vk_keyword{PipelineVertexInputStateCreateInfo},
@type_vk_keyword{PipelineInputAssemblyStateCreateInfo},
@type_vk_keyword{VertexInputBindingDivisorDescriptionEXT} and
@type_vk_keyword{PipelineVertexInputDivisorStateCreateInfoEXT} structures.
*/
class MAGNUM_VK_EXPORT MeshLayout {
    public:
        /**
         * @brief Constructor
         * @param primitive     Mesh primitive
         *
         * The following @type_vk{PipelineVertexInputStateCreateInfo} fields
         * are pre-filled in addition to `sType`, everything else is
         * zero-filled:
         *
         * -    <em>(none)</em>
         *
         * The following @type_vk{PipelineInputAssemblyStateCreateInfo} fields
         * are pre-filled in addition to `sType`, everything else is
         * zero-filled:
         *
         * -    `topology` to @p primitive
         *
         * @see @ref vkPipelineVertexInputStateCreateInfo(),
         *      @ref vkPipelineInputAssemblyStateCreateInfo()
         */
        explicit MeshLayout(MeshPrimitive primitive);
        /** @overload */
        explicit MeshLayout(Magnum::MeshPrimitive primitive);

        /**
         * @brief Construct without initializing the contents
         *
         * Note that not even the `sType` fields are set --- the structures
         * have to be fully initialized afterwards in order to be usable.
         */
        explicit MeshLayout(NoInitT) noexcept;

        /**
         * @brief Construct from existing data
         *
         * Copies the existing values verbatim, pointers are kept unchanged
         * without taking over the ownership. Modifying the newly created
         * instance will not modify the original data nor the pointed-to data.
         */
        explicit MeshLayout(const VkPipelineVertexInputStateCreateInfo& vertexInfo, const VkPipelineInputAssemblyStateCreateInfo& assemblyInfo);

        /** @brief Copying is not allowed */
        MeshLayout(const MeshLayout&) = delete;

        /** @brief Move constructor */
        MeshLayout(MeshLayout&& other) noexcept;

        ~MeshLayout();

        /** @brief Copying is not allowed */
        MeshLayout& operator=(const MeshLayout&) = delete;

        /** @brief Move assignment */
        MeshLayout& operator=(MeshLayout&& other) noexcept;

        /**
         * @brief Add a buffer binding
         * @param binding   Binding index, to which a buffer subrange will be
         *      bound when drawing the mesh. Has to be unique among all
         *      @ref addBinding() and @ref addInstancedBinding() calls.
         * @param stride    Binding stride, in bytes
         *
         * Adds a new @type_vk{VertexInputBindingDescription} structure to
         * @p vkPipelineVertexInputStateCreateInfo() with the following fields
         * set:
         *
         * -    `binding`
         * -    `stride`
         * -    `inputRate` to @val_vk{VERTEX_INPUT_RATE_VERTEX,VertexInputRate}
         *
         * @see @ref addInstancedBinding()
         */
        MeshLayout& addBinding(UnsignedInt binding, UnsignedInt stride);

        /**
         * @brief Add an instanced buffer binding
         * @param binding   Binding index, to which a buffer subrange will be
         *      bound when drawing the mesh. Has to be unique among all
         *      @ref addBinding() and @ref addInstancedBinding() calls.
         * @param stride    Binding stride, in bytes
         * @param divisor   Attribute divisor. @cpp 1 @ce means the attribute
         *      will be advanced for each instance, larger values will mean
         *      `n` instances will be drawn using the same attribute,
         *      @cpp 0 @ce will make all instances use a single attribute
         *      (which is effectively the same as setting @p divisor to
         *      instance count).
         *
         * Compared to @ref addBinding(), sets `inputRate` to
         * @val_vk{VERTEX_INPUT_RATE_INSTANCE,VertexInputRate}. If @p divisor
         * is not @cpp 1 @ce, a new @type_vk{VertexInputBindingDivisorDescriptionEXT} structure with
         * is added to @type_vk{PipelineVertexInputDivisorStateCreateInfoEXT}
         * which is then referenced from the `pNext` chain of
         * @ref vkPipelineVertexInputStateCreateInfo(), with the following
         * fields set:
         *
         * -    `binding`
         * -    `divisor`
         *
         * <b></b>
         *
         * @requires_vk_feature @ref DeviceFeature::VertexAttributeInstanceRateDivisor
         *      if @p divisor isn't `1`
         * @requires_vk_feature @ref DeviceFeature::VertexAttributeInstanceRateZeroDivisor
         *      if @p divisor is `0`
         */
        MeshLayout& addInstancedBinding(UnsignedInt binding, UnsignedInt stride, UnsignedInt divisor = 1);

        /**
         * @brief Add an attribute
         * @param location      Attribute location, matching a shader input
         * @param binding       Binding index, corresponding to the @p binding
         *      parameter of one of the @ref addBinding() /
         *      @ref addInstancedBinding() calls.
         * @param format        Vertex format
         * @param offset        Attribute offset in bytes inside @p stride of
         *      given @p binding
         *
         * Adds a new @type_vk{VertexInputAttributeDescription} structure to
         * @ref vkPipelineVertexInputStateCreateInfo() with the following
         * fields set:
         *
         * -    `location`
         * -    `binding`
         * -    `format`
         * -    `offset`
         */
        MeshLayout& addAttribute(UnsignedInt location, UnsignedInt binding, VertexFormat format, UnsignedInt offset);

        /** @brief Underlying @type_vk{PipelineVertexInputStateCreateInfo} structure */
        VkPipelineVertexInputStateCreateInfo& vkPipelineVertexInputStateCreateInfo() {
            return _vertexInfo;
        }
        /** @overload */
        const VkPipelineVertexInputStateCreateInfo& vkPipelineVertexInputStateCreateInfo() const {
            return _vertexInfo;
        }
        /** @overload */
        operator const VkPipelineVertexInputStateCreateInfo*() const {
            return &_vertexInfo;
        }

        /**
         * @brief Underlying @type_vk{PipelineInputAssemblyStateCreateInfo} structure
         *
         * If @ref addInstancedBinding() was called with @p divisor different
         * than @cpp 0 @ce, the `pNext` chain contains the
         * @type_vk{PipelineVertexInputDivisorStateCreateInfoEXT} structure.
         */
        VkPipelineInputAssemblyStateCreateInfo& vkPipelineInputAssemblyStateCreateInfo() {
            return _assemblyInfo;
        }
        /** @overload */
        const VkPipelineInputAssemblyStateCreateInfo& vkPipelineInputAssemblyStateCreateInfo() const {
            return _assemblyInfo;
        }
        /** @overload */
        operator const VkPipelineInputAssemblyStateCreateInfo*() const {
            return &_assemblyInfo;
        }

    private:
        VkPipelineVertexInputStateCreateInfo _vertexInfo;
        VkPipelineInputAssemblyStateCreateInfo _assemblyInfo;

        struct State;
        Containers::Pointer<State> _state;
};

}}

#endif
