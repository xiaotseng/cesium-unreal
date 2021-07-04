// Copyright 2020-2021 CesiumGS, Inc. and Contributors

#pragma once

#include "PCH.h"
#include "Engine/Texture.h"
#include "StaticMeshResources.h"
#include <cstdint>
#include <glm/mat4x4.hpp>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

struct LoadedTexture {
  std::unique_ptr<FTexturePlatformData> pTextureData;
  TextureAddress addressX;
  TextureAddress addressY;
  TextureFilter filter;
};

/**
 * @brief Indicates the type of water mask used in this tile.
 */
enum class WaterMaskType {
  /**
   * @brief The tile has no water, it is only land.
   */
  OnlyLand,

  /**
   * @brief The tile has no land, it is only water.
   */
  OnlyWater,

  /**
   * @brief The tile has a mix of land and water, so a texture defines where each
   * are found.
   */
  MixOfLandAndWater
};

struct LoadedWaterMask {
  /**
   * @brief Indicates the type of water mask used in this tile.
   */
  WaterMaskType type;

  /**
   * @brief The texture describing which parts of the tile are water.
   * 
   * Ignored unless {@link LoadedWaterMask::type} is
   * {@link WaterMaskType::MixOfLandAndWater}.
   */
  std::optional<LoadedTexture> waterMaskTexture;

  /**
   * @brief The translation to apply to the water mask.
   */
  glm::dvec2 waterMaskTranslation;

  /**
   * @brief The scale to apply to the water mask.
   */
  double waterMaskScale;
};

struct LoadedModel {
  // The indices of the node, mesh, and primitive in the glTF.
  size_t nodeIndex;
  size_t meshIndex;
  size_t primitiveIndex;

  // The complete transform from this model to the tileset's coordinate system,
  // which is usually ECEF.
  glm::dmat4x4 transform;

  // The Unreal rendering data for this model.
  std::unique_ptr<FStaticMeshRenderData> RenderData;

  // The cooked collision data for this model.
#if PHYSICS_INTERFACE_PHYSX
  PxTriangleMesh* pCollisionMesh;
#else
  TSharedPtr<Chaos::FTriangleMeshImplicitObject, ESPMode::ThreadSafe>
      pCollisionMesh;
#endif

  // Maps material texture coordinate index parameter names, such as
  // baseColorTextureCoordinateIndex, to the UV channel that contains the
  // texture coordinates for this element. The total size of this map
  // is the total number of UV channels required.
  std::unordered_map<std::string, uint32_t> textureCoordinateParameters;

  // Details of the standard glTF textures that may or may not be used.
  std::optional<LoadedTexture> baseColorTexture;
  std::optional<LoadedTexture> metallicRoughnessTexture;
  std::optional<LoadedTexture> normalTexture;
  std::optional<LoadedTexture> emissiveTexture;
  std::optional<LoadedTexture> occlusionTexture;

  LoadedWaterMask waterMask;
};

class GltfLoader {
public:
};
