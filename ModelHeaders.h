#ifndef MODELHEADERS_H
#define MODELHEADERS_H

#include <stdint.h>
#include "vector3D.h"

struct ModelHeader
{
  char id[4];
  uint8_t version[4];
  uint32_t nameLength;
  uint32_t nameOfs;
  uint32_t Flags;

  uint32_t nGlobalSequences;
  uint32_t ofsGlobalSequences;
  uint32_t nAnimations;
  uint32_t ofsAnimations;
  uint32_t nAnimationLookup;
  uint32_t ofsAnimationLookup;
  uint32_t nBones;
  uint32_t ofsBones;
  uint32_t nKeyBoneLookup;
  uint32_t ofsKeyBoneLookup;

  uint32_t nVertices;
  uint32_t ofsVertices;
  uint32_t nViews;

  uint32_t nColors;
  uint32_t ofsColors;

  uint32_t nTextures;
  uint32_t ofsTextures;

  uint32_t nTransparency; // H
  uint32_t ofsTransparency;
  uint32_t nTexAnims; // J
  uint32_t ofsTexAnims;
  uint32_t nTexReplace;
  uint32_t ofsTexReplace;

  uint32_t nTexFlags; // Render Flags
  uint32_t ofsTexFlags; // Blending modes / render flags.
  uint32_t nBoneLookup; // BonesAndLookups
  uint32_t ofsBoneLookup; // A bone lookup table.

  uint32_t nTexLookup;
  uint32_t ofsTexLookup;

  uint32_t nTexUnitLookup; // L
  uint32_t ofsTexUnitLookup;
  uint32_t nTransparencyLookup; // M
  uint32_t ofsTransparencyLookup;
  uint32_t nTexAnimLookup;
  uint32_t ofsTexAnimLookup;

  //not sure about these :/
  vector3D VertexBoxMin;//?
  vector3D VertexBoxMax;//?
  float VertexBoxRadius;
  vector3D BoundingBoxMin;//?
  vector3D BoundingBoxMax;//?
  float BoundingBoxRadius;

  uint32_t nBoundingTriangles;
  uint32_t ofsBoundingTriangles;
  uint32_t nBoundingVertices;
  uint32_t ofsBoundingVertices;
  uint32_t nBoundingNormals;
  uint32_t ofsBoundingNormals;

  uint32_t nAttachments; // O
  uint32_t ofsAttachments;
  uint32_t nAttachLookup; // P
  uint32_t ofsAttachLookup;
  uint32_t nEvents; // SoundEvents
  uint32_t ofsEvents;
  uint32_t nLights; // R
  uint32_t ofsLights;
  uint32_t nCameras; // S
  uint32_t ofsCameras;
  uint32_t nCameraLookup;
  uint32_t ofsCameraLookup;
  uint32_t nRibbonEmitters; // U
  uint32_t ofsRibbonEmitters;
  uint32_t nParticleEmitters; // V
  uint32_t ofsParticleEmitters;

};

struct ModelViewStruct
{
  char id[4]; // Signature
  uint32_t nIndex, ofsIndex; // Vertices in this model (index into vertices[])
  uint32_t nTris, ofsTris; // indices
  uint32_t nProps, ofsProps; // additional vtx properties
  uint32_t nSub, ofsSub; // materials/renderops/submeshes
  uint32_t nTex, ofsTex; // material properties/textures
  int32_t lod; // LOD bones
};

struct ModelVertex {
  vector3D pos;
  uint8_t weights[4];
  uint8_t bones[4];
  vector3D normal;
  float texcoordsX,texcoordsY;
  int unk1, unk2; // always 0,0 so this is probably unused
};

/// One material + render operation
struct ModelGeoset {
  uint16_t id; // mesh part id?
  uint16_t d2; // ?
  uint16_t vstart; // first vertex
  uint16_t vcount; // num vertices
  uint16_t istart; // first index
  uint16_t icount; // num indices
  uint16_t d3; // number of bone indices
  uint16_t d4; // ? always 1 to 4
  uint16_t d5; // ?
  uint16_t d6; // root bone?
  vector3D BoundingBox[2];
  float radius;
};

/// A texture unit (sub of material)
struct ModelTexUnit {
  // probably the texture units
  // size always >=number of materials it seems
  uint16_t flags;    // Flags
  uint16_t shading;    // If set to 0x8000: shaders. Used in skyboxes to ditch the need for depth buffering. See below.
  uint16_t op;      // Material this texture is part of (index into mat)
  uint16_t op2;      // Always same as above?
  int16_t colorIndex;  // color or -1
  uint16_t flagsIndex;  // more flags...
  uint16_t texunit;    // Texture unit (0 or 1)
  uint16_t mode;      // ? (seems to be always 1)
  uint16_t textureid;  // Texture id (index into global texture list)
  uint16_t texunit2;  // copy of texture unit value?
  uint16_t transid;    // transparency id (index into transparency list)
  uint16_t texanimid;  // texture animation id
};

#endif // MODELHEADERS_H
