/* Copyright � 2019, Mykos Hudson-Crisp <micklionheart@gmail.com>
* All rights reserved. */
#pragma once

// WARNING: This file is incomplete and the API listed here will most assuredly change.

#ifdef _WIN32
#define UNICODE
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#ifdef USE_X11
#define Window X11Window
#include <X11/Xlib.h>
#undef Window
#endif

#ifdef USE_WAYLAND
#error Include wayland headers here
#endif

typedef unsigned long long	u64;
typedef unsigned int		u32;
typedef unsigned short		u16;
typedef unsigned char		u8;

typedef signed long long	s64;
typedef signed int		s32;
typedef signed short		s16;
typedef signed char		s8;

typedef double			f64;
typedef float			f32;

struct GGD_RenderEngine;
struct GGD_PhysicsEngine;

struct GGD_Window;
struct GGD_ImageData;
struct GGD_MeshData;
struct GGD_MeshGroup;
struct GGD_MeshInstance;
struct GGD_DrawBuffer;
struct GGD_Camera;
struct GGD_FontData;

struct GGD_Shape;
struct GGD_Simulation;
struct GGD_ShapeInstance;

struct GGPosition {
	f32 x, y, z, w;
};
struct GGRotation {
	f32 x, y, z, w;
};

struct GGTransform {
	GGPosition Position;
	GGRotation Rotation;
};

enum GGPlatform {
	PLAT_UNDEFINED = 0,
	PLAT_WIN32,
	PLAT_X11,
	PLAT_WAYLAND,
	PLAT_OSX
};

enum GGProjectionType {
	PROJ_UNDEFINED = 0,
	PROJ_ORTHOGRAPHIC,
	PROJ_PERSPECTIVE,
	PROJ_PARABOLIC
};

enum GGVertexFormat {
	VFMT_UNDEFINED = 0,
	VFMT_PNTL_32F,
	VFMT_PNTLB3_32F,
	VFMT_PNTLB7_32F
};

enum GGIndexFormat {
	IFMT_UNDEFINED = 0,
	IFMT_SOUP_16U,
	IFMT_SOUP_32U,
	IFMT_STRIP_16U,
	IFMT_STRIP_32U
};

enum GGDistanceFormat {
	DFMT_UNDEFINED = 0,
	DFMT_EXACT_DISTANCE_8,
	DFMT_EXACT_DISTANCE_32F,
	DFMT_BOUND_DISTANCE_8,
	DFMT_BOUND_DISTANCE_32F,
};

enum GGPixelFormat {
	PFMT_UNDEFINED = 0,

	PFMT_RGBA8,
	PFMT_RGBA8_SRGB,
	PFMT_RGBE8_SRGB,
	PFMT_A2_BGR10,
	PFMT_A2_BGR10_QRGB,
	PFMT_RGB9_E5_QRGB,
	PFMT_RGBA16F,
	PFMT_RG16F,
	PFMT_R16F,
	PFMT_RGBA32F,
	PFMT_RG32F,
	PFMT_R32F,

	PFMT_BC1,
	PFMT_BC1_SRGB,
	PFMT_BC2,
	PFMT_BC2_SRGB,
	PFMT_BC3,
	PFMT_BC3_SRGB,
	PFMT_BC4,
	PFMT_BC4_SIGNED,
	PFMT_BC5,
	PFMT_BC5_SIGNED,
	PFMT_BC6H,
	PFMT_BC6H_SIGNED,
	PFMT_BC7,
	PFMT_BC7_SRGB,

	PFMT_INVALID
};

enum GGPhysicsMode {
	PHYS_GHOST = 0,
	PHYS_STATIC,
	PHYS_BOX,
	PHYS_SPHERE,
	PHYS_CYLINDER,
	PHYS_CONVEX,
	PHYS_CONVEX_UNWRAP,
	PHYS_TRIMESH
};

enum GGButtonCode {
	BTN_NULL = 0,
	BTN_LEFT_CONTROL,
	BTN_RIGHT_CONTROL,
	BTN_LEFT_SHIFT,
	BTN_RIGHT_SHIFT,
	BTN_LEFT_ALT,
	BTN_RIGHT_ALT,
	BTN_CAPS_LOCK,
	BTN_BACKSPACE,
	BTN_TAB,
	BTN_ENTER,
	BTN_MOUSE0,
	BTN_MOUSE1,
	BTN_MOUSE2,
	BTN_MOUSE3,
	BTN_MOUSE4,
	BTN_MOUSE5,
	BTN_MOUSE6,
	BTN_MOUSE7,
	BTN_LEFT_LOGO,
	BTN_RIGHT_LOGO,
	BTN_UNKNOWN21,
	BTN_UNKNOWN22,
	BTN_KP_MODIFIER,
	BTN_QUIT,
	BTN_F_MODIFIER,
	BTN_MENU,
	BTN_ESCAPE,
	BTN_UP,
	BTN_DOWN,
	BTN_LEFT,
	BTN_RIGHT,
	BTN_SPACE,
	// 33 - 126 (ASCII)
	BTN_DELETE = 127,
	BTN_AUX0 = 128,
	BTN_AUX31 = 128 + 31,
	BTN_LEFT_BUMPER,
	BTN_RIGHT_BUMPER,
	BTN_LEFT_THUMB,
	BTN_RIGHT_THUMB,
	BTN_LEFT_TRIGGER,
	BTN_RIGHT_TRIGGER,
	BTN_LEFT_THUMB_HORIZONTAL,
	BTN_LEFT_THUMB_VERTICAL,
	BTN_RIGHT_THUMB_HORIZONTAL,
	BTN_RIGHT_THUMB_VERTICAL,
	BTN_INVALID
};

enum GGDriverStatus {
	GGD_STATUS_DRIVER_INVALID = 0,
	GGD_STATUS_DRIVER_READY = 1,
	GGD_STATUS_DRIVER_ERROR = 2,
	GGD_STATUS_VERSION_INVALID = 3,
	GGD_STATUS_SIGNATURE_INVALID = 4,
};

enum GGCacheStatus {
	GGD_CACHE_RETAIN,
	GGD_CACHE_UPDATE,
	GGD_CACHE_DISCARD,
	GGD_CACHE_RESIZE_BUFFER,
	GGD_CACHE_RELOAD_SOURCE
};

enum GGImageUsage {
	IMG_USAGE_STATIC	= 0,
	IMG_USAGE_TARGET	= 1,
	IMG_USAGE_OVERLAY	= 2,
	IMG_USAGE_GLYPH		= 4,
	IMG_USAGE_SKYBOX	= 8,
	IMG_USAGE_EMISSIVE	= 16,
	// ... incomplete list ...
};

enum GGMaterialLayer {
	MATL_SURFACE_COLOR,
	MATL_SURFACE_FINISH,
	MATL_AMBIENT_OCCLUSION,
	MATL_LIGHTMAP0,
	MATL_LIGHTMAP1,
	MATL_LIGHTMAP2,
	MATL_LIGHTMAP3,
};

enum GGTextOrigin {
	TEXT_BASELINE,
	TEXT_TOP_LEFT,
	TEXT_TOP,
	TEXT_TOP_RIGHT,
	TEXT_LEFT,
	TEXT_CENTER,
	TEXT_RIGHT,
	TEXT_BOTTOM_LEFT,
	TEXT_BOTTOM,
	TEXT_BOTTOM_RIGHT
};

struct GGBoneIndexWeight_32F {
	f32 Index;
	f32 Weight;
};
struct GGVertexFormat_PNTL_32F {
	f32 Position[3];
	f32 Normal[3];
	f32 TexCoord[2];
	f32 LightCoord[2];
};
struct GGVertexFormat_PNTLB3_32F {
	f32 Position[3];
	f32 Normal[3];
	f32 TexCoord[2];
	f32 LightCoord[2];
	GGBoneIndexWeight_32F Bone[3];
};
struct GGVertexFormat_PNTLB7_32F {
	f32 Position[3];
	f32 Normal[3];
	f32 TexCoord[2];
	f32 LightCoord[2];
	GGBoneIndexWeight_32F Bone[7];
};

#define GGD_API_VERSION 0
#ifdef _WIN32
#ifdef GGD_PLUGIN_CODE
#define GGD_ENTRY_POINT_DECORATION __declspec(dllexport)
#else
#define GGD_ENTRY_POINT_DECORATION __declspec(dllimport)
#endif
#else
#define GGD_ENTRY_POINT_DECORATION __attribute__((__visibility__("default")))
#endif
#define GGD_ENTRY_POINT(X) extern GGD_ENTRY_POINT_DECORATION GGDriverStatus GGD_DriverMain(GGD_DriverContext* X)

extern "C" {
	enum GGD_BufferToken {
		GGD_BUFFER_QUERY,
		GGD_BUFFER_CLOSED,
		GGD_BUFFER_READ,
		GGD_BUFFER_WRITE,
		GGD_BUFFER_WRITE_AND_TRUNCATE,
	};

	struct GGD_BufferRequest {
		void(*callback)(GGD_BufferRequest*, GGD_BufferToken status, void* buffer, u64 bytes);
	};

	struct GGD_BufferInfo {
		void(*access)(const GGD_BufferInfo*, GGD_BufferToken access, u64 offset, u64 bytes, GGD_BufferRequest* request);
	};

	struct GGD_SubShape {
		GGD_Shape* shape;
		GGTransform xform;
		f32 mass;
	};

	struct GGD_RenderEngine {
		const char* Name;
		u64 Priority;
		u64 GraphicsAPI;
/* opt */	int(*Validate)();
/* opt */	int(*Shutdown)(GGD_RenderEngine*);

		GGD_Window*(*Window_Alloc)();
		void(*Window_Free)(GGD_Window*);
		int(*Window_IsValid)(GGD_Window*);
		void(*Window_Resize)(GGD_Window*, u32 w, u32 h);
		void(*Window_SetCamera)(GGD_Window* dst, GGD_Camera* camera);
		void(*Window_SetOverlay)(GGD_Window* dst, GGD_ImageData* overlay);
		int(*Window_Update)(GGD_Window*);

		GGD_MeshData*(*MeshData_Alloc_Polygon)(GGVertexFormat vertexFormat, u32 vertexCount, const GGD_BufferInfo* vertexBuffer, GGIndexFormat indexFormat, u32 indexCount, const GGD_BufferInfo* indexBuffer, GGD_BufferInfo* cacheBuffer);
/* opt */	GGD_MeshData*(*MeshData_Alloc_Field)(GGDistanceFormat fieldFormat, u32 x, u32 y, u32 z, const GGD_BufferInfo* fieldBuffer, GGD_BufferInfo* cacheBuffer);
		void(*MeshData_Free)(GGD_MeshData*);

		GGD_ImageData*(*ImageData_Alloc)(GGImageUsage usage, u32 x, u32 y, GGPixelFormat format, const GGD_BufferInfo* pixelBuffer, GGD_BufferInfo* cacheBuffer);
		void(*ImageData_Free)(GGD_ImageData*);
		void(*ImageData_ReadPixelData)(GGD_ImageData* image, GGD_BufferInfo* buffer);
		void(*ImageData_DrawPixelData)(GGD_ImageData* dst, const GGD_BufferInfo* buffer);
		void(*ImageData_DrawCamera)(GGD_ImageData* dst, GGD_Camera* src);
		void(*ImageData_DrawImage)(GGD_ImageData* dst, GGD_ImageData* src, f32 x, f32 y, f32 w, f32 h);
		void(*ImageData_DrawText)(GGD_ImageData* dst, GGD_FontData* src, f32 x, f32 y, GGTextOrigin origin, const char* text);

		GGD_FontData*(*FontData_Alloc)();
		void(*FontData_Free)(GGD_FontData*);
		void(*FontData_SetGlyph)(GGD_FontData* image, u32 codepoint, GGD_ImageData* img, f32 x, f32 y, f32 w, f32 h, f32 basex, f32 basey);

		GGD_MeshGroup*(*MeshGroup_Alloc)(GGD_BufferInfo* cacheBuffer);
		void(*MeshGroup_Free)(GGD_MeshGroup*);
		void(*MeshGroup_SetSky)(GGD_MeshGroup*, GGD_ImageData* image);

		GGD_MeshInstance*(*MeshInstance_Alloc)(GGD_MeshGroup*, GGD_BufferInfo* cacheBuffer);
		void(*MeshInstance_Free)(GGD_MeshInstance*);
		void(*MeshInstance_SetMeshData)(GGD_MeshInstance*, GGD_MeshData* mesh, u32 index);
		void(*MeshInstance_SetMeshSubset)(GGD_MeshInstance*, u32 offset, u32 count);
		void(*MeshInstance_SetImageData)(GGD_MeshInstance*, GGD_ImageData* image, GGMaterialLayer layer);
		void(*MeshInstance_SetAnimation)(GGD_MeshInstance*, u32 firstIndex, u32 lastIndex, f32 frameRate);
		void(*MeshInstance_SetTransform)(GGD_MeshInstance*, const GGTransform* pose);
		void(*MeshInstance_SetBoneTransform)(GGD_MeshInstance*, u32 bone, const GGTransform* pose);

		GGD_Camera*(*Camera_Alloc)();
		void(*Camera_Free)(GGD_Camera*);
		void(*Camera_SetPerspective)(GGD_Camera*, f32 aspect, f32 fovy, f32 zNear, f32 zFar);
/* opt */	void(*Camera_SetOrthographic)(GGD_Camera*, f32 w, f32 h, f32 zNear, f32 zFar);
/* opt */	void(*Camera_SetParabolic)(GGD_Camera*, f32 scale);
		void(*Camera_SetMeshGroup)(GGD_Camera*, GGD_MeshGroup*);
		void(*Camera_SetTransform)(GGD_Camera*, const GGTransform*);
	};

	struct GGD_PhysicsEngine {
		const char* Name;
		u64 Priority;
		int(*Validate)();
		int(*Shutdown)(GGD_PhysicsEngine*);

		GGD_Shape*(*Shape_Alloc_Box)(f32 x, f32 y, f32 z);
		GGD_Shape*(*Shape_Alloc_Sphere)(f32 radius);
/* opt */	GGD_Shape*(*Shape_Alloc_Cylinder)(f32 radius, f32 height);
/* opt */	GGD_Shape*(*Shape_Alloc_ConvexMesh)(GGVertexFormat vertexFormat, const GGD_BufferInfo* vertexBuffer, GGD_BufferInfo* cacheBuffer);
/* opt */	GGD_Shape*(*Shape_Alloc_TriangleMesh)(GGVertexFormat vertexFormat, const GGD_BufferInfo* vertexBuffer, GGIndexFormat indexFormat, const GGD_BufferInfo* indexBuffer, GGD_BufferInfo* cacheBuffer);
/* opt */	GGD_Shape*(*Shape_Alloc_DistanceField)(GGDistanceFormat fieldFormat, u32 x, u32 y, u32 z, const GGD_BufferInfo* fieldBuffer, GGD_BufferInfo* cacheBuffer);
/* opt */	GGD_Shape*(*Shape_Alloc_Composite)(GGD_SubShape* shapes, u32 count, GGD_BufferInfo* cacheBuffer);
		void(*Shape_Free)(GGD_Shape*);

		GGD_Simulation*(*Simulation_Alloc)();
		void(*Simulation_Free)(GGD_Simulation*);
		void(*Simulation_Gravity)(GGD_Simulation*, f32 x, f32 y, f32 z);
		void(*Simulation_Update)(GGD_Simulation*, f32 dt);

		GGD_ShapeInstance*(*ShapeInstance_Alloc)(GGD_Simulation*, GGD_Shape*, GGD_BufferInfo* cacheBuffer);
		void(*ShapeInstance_Free)(GGD_ShapeInstance*);
		void(*ShapeInstance_SetMass)(GGD_ShapeInstance*, f32 mass);
		f32(*ShapeInstance_GetMass)(GGD_ShapeInstance*);
		void(*ShapeInstance_SetFriction)(GGD_ShapeInstance*, f32 friction);
		f32(*ShapeInstance_GetFriction)(GGD_ShapeInstance*);
		void(*ShapeInstance_SetVelocity)(GGD_ShapeInstance*, const GGTransform* poseDt);
		void(*ShapeInstance_GetVelocity)(GGD_ShapeInstance*, GGTransform* poseDt);
		void(*ShapeInstance_SetTransform)(GGD_ShapeInstance*, const GGTransform* pose);
		void(*ShapeInstance_GetTransform)(GGD_ShapeInstance*, GGTransform* pose);
		void(*ShapeInstance_SetVelocityPointer)(GGD_ShapeInstance*, GGTransform* poseDtPtr);
		void(*ShapeInstance_SetTransformPointer)(GGD_ShapeInstance*, GGTransform* posePtr);
	};

	struct GGD_DriverContext {
		u64 APIVersion, GameVersion;
		const char* GameName;
		void(*RegisterRenderEngine)(const GGD_RenderEngine*);
		void(*RegisterPhysicsEngine)(const GGD_PhysicsEngine*);
	};
};

typedef GGDriverStatus(*GGD_DriverMainPtr)(GGD_DriverContext*);
