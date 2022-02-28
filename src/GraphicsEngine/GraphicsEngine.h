//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************

#pragma once
#include <dxcapi.h>
#include <vector>
#include "nv_helpers_DX12/TopLevelASGenerator.h"
#include "nv_helpers_dx12/ShaderBindingTableGenerator.h"
#include "DXSample.h"

using namespace DirectX;

// Note that while ComPtr is used to manage the lifetime of resources on the CPU,
// it has no understanding of the lifetime of resources on the GPU. Apps must account
// for the GPU lifetime of resources to avoid destroying objects that may still be
// referenced by the GPU.
// An example of this can be found in the class method: OnDestroy().
using Microsoft::WRL::ComPtr;

class GraphicsEngine : public DXSample
{
public:
	GraphicsEngine(std::wstring name, ConfigInfo& config);

	virtual void OnDeviceLost() override;
	virtual void OnDeviceRestored() override;

	virtual void OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();

private:

	// In this sample we overload the meaning of FrameCount to mean both the maximum
	// number of frames that will be queued to the GPU at a time, as well as the number
	// of back buffers in the DXGI swap chain. 
	static const UINT FrameCount = 2;

	// TODO when adding import model feature, add normals
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT4 color; 

		Vertex(XMFLOAT4 pos, XMFLOAT4 /*n*/, XMFLOAT4 col)
			:position(pos.x, pos.y, pos.z), color(col){}
		Vertex(XMFLOAT3 pos, XMFLOAT4 col)
			:position(pos), color(col){}
	};


	// Pipeline objects.

	// DX12 objects
	ComPtr<ID3D12Device5> m_device;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12GraphicsCommandList4> m_commandList;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;

	// Swap chain objects
	ComPtr<IDXGIFactory4>  m_factory;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12Resource> m_depthStencil;

	// Presentation fence (synchronization) objects
	ComPtr<ID3D12Fence> m_fence;
	HANDLE m_fenceEvent;
	UINT64 m_fenceValue;
	UINT m_frameIndex;

	// DX12 Rendering Objects
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	UINT m_rtvDescriptorSize;
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;

	// root signatures
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12PipelineState> m_pipelineState;

	// Geometry
	ComPtr<ID3D12Resource> m_tetraVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_tetraVertexBufferView;

	ComPtr<ID3D12Resource> m_tetraIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_tetraIndexBufferView;

	// #DXR Extra: Per-Instance Data
	ComPtr<ID3D12Resource> m_planeVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_planeVertexBufferView;

	ComPtr<ID3D12Resource> m_cubeVertexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_cubeVertexBufferView;

	ComPtr<ID3D12Resource> m_cubeIndexBuffer;
	D3D12_INDEX_BUFFER_VIEW m_cubeIndexBufferView;

	void CreateGeometry();
	void CreateTetrahedronVB();
	void CreatePlaneVB();
	void CreateCubeVB();
	void CreateDepthBuffer();

	void InitCamera();
	void LoadPipeline();
	void InitializeRasterAssets();
	void OneTimeCommandList();
	void PopulateCommandList();
	void SetRasterCommandList();
	void SetRayTracingCommandList();
	void WaitForPreviousFrame();
	void CheckRaytracingSupport();

	virtual void OnKeyUp(UINT8 key);
	bool isRasterMode = true;
	bool isAnimating = true;
	// #DXR
	struct AccelerationStructureBuffers
	{
		ComPtr<ID3D12Resource> pScratch;      // Scratch memory for AS builder
		ComPtr<ID3D12Resource> pResult;       // Where the AS is
		ComPtr<ID3D12Resource> pInstanceDesc; // Hold the matrices of the instances
	};

	nv_helpers_dx12::TopLevelASGenerator m_topLevelASGenerator;
	AccelerationStructureBuffers m_topLevelASBuffers;
	std::vector<std::pair<ComPtr<ID3D12Resource>, DirectX::XMMATRIX>> m_instances;

	/// Create the acceleration structure of an instance
///
/// param     vVertexBuffers : pair of buffer and vertex count
/// return    AccelerationStructureBuffers for TLAS

	AccelerationStructureBuffers CreateBottomLevelAS(
		std::vector<std::pair<ComPtr<ID3D12Resource>, uint32_t>> vVertexBuffers,
		std::vector<std::pair<ComPtr<ID3D12Resource>, uint32_t>> vIndexBuffers = {});
	/// Create the main acceleration structure that holds
	/// all instances of the scene
	/// \param     instances : pair of BLAS and transform
	void CreateTopLevelAS(const std::vector<std::pair<ComPtr<ID3D12Resource>, DirectX::XMMATRIX>>& instances, bool updateOnly = false);

	/// Create all acceleration structures, bottom and top
	void CreateAccelerationStructures();

	ComPtr<ID3D12RootSignature> CreateRayGenSignature();
	ComPtr<ID3D12RootSignature> CreateMissSignature();
	ComPtr<ID3D12RootSignature> CreateHitSignature();

	void CreateRaytracingPipeline();

	ComPtr<IDxcBlob> m_rayGenLibrary;
	ComPtr<IDxcBlob> m_hitLibrary;
	ComPtr<IDxcBlob> m_missLibrary;
	ComPtr<IDxcBlob> m_shadowLibrary;

	ComPtr<ID3D12RootSignature> m_rayGenSignature;
	ComPtr<ID3D12RootSignature> m_hitSignature;
	ComPtr<ID3D12RootSignature> m_missSignature;
	ComPtr<ID3D12RootSignature> m_shadowSignature;

	// Ray tracing pipeline state
	ComPtr<ID3D12StateObject> m_rtStateObject;

	// Ray tracing pipeline state properties, retaining the shader identifiers
	// to use in the Shader Binding Table
	ComPtr<ID3D12StateObjectProperties> m_rtStateObjectProps;

	void CreateRaytracingOutputBuffer();
	void CreateShaderResourceHeap();
	ComPtr<ID3D12Resource> m_outputResource;
	ComPtr<ID3D12DescriptorHeap> m_srvUavHeap;
	// #DXR
	void CreateShaderBindingTable();
	nv_helpers_dx12::ShaderBindingTableGenerator m_sbtHelper;
	ComPtr<ID3D12Resource> m_sbtStorage;

	// #DXR Extra: Perspective Camera
	void CreateCameraBuffer();
	void UpdateCameraBuffer();
	ComPtr<ID3D12Resource> m_cameraBuffer;
	ComPtr< ID3D12DescriptorHeap> m_constHeap;
	uint32_t m_cameraBufferSize = 0;

	// Data
	ConfigInfo* myConfig;

	// #DXR Extra: Perspective Camera++
	void OnButtonDown(UINT32 lParam);
	void OnMouseMove(UINT8 wParam, UINT32 lParam);
	void OnMouseWheel(UINT8 wParam);

	uint32_t m_time = 0;

	// Animation for raster pipeline
	struct InstanceProperties
	{
		XMMATRIX objectToWorld;
		//# DXR Extra - Simple Lighting
		XMMATRIX objectToWorldNormal;
	};

	ComPtr<ID3D12Resource> m_instanceProperties;
	void CreateInstancePropertiesBuffer();
	void UpdateInstancePropertiesBuffer();

	void InitializeDXRAssets();
	void InitializeDXGIAdapter();
};
