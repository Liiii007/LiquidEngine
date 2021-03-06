#pragma once

#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "D3D12.lib")
#pragma comment(lib, "dxgi.lib")

#include <DirectXColors.h>
#include <windows.h>
#include <WindowsX.h>
#include <wrl.h>
#include <dxgi1_4.h>
#include <d3d12.h>
#include <D3Dcompiler.h>
#include <DirectXMath.h>
#include <DirectXPackedVector.h>
#include <DirectXColors.h>
#include <DirectXCollision.h>
#include <string>
#include <memory>
#include <algorithm>
#include <vector>
#include <array>
#include <unordered_map>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <cassert>
#include "../Tools/Common/d3dUtil.h"
#include "../Tools/Common/UploadBuffer.h"
#include "../Tools/Singleton.h"
#include "../System/InputSystem.h"
#include "../World/Model.h"
#include "../DataStructures.h"

using namespace DirectX;
using namespace std;

using Microsoft::WRL::ComPtr;


class XIIRenderer
{
public:
	XIIRenderer()  {};
	~XIIRenderer() {};


	bool Init(HINSTANCE);

	//Constant
	

	//Upload
	void UploadVertices();
	void UploadIndices(Model* model);
	void UploadConstant();
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);
	int RenderTick();
	void Update();

private:
	//Init
	bool InitWindow();
	bool InitDirect3D();
	void CreateCommandObjects();
	void CreateSwapChain();
	void CreateDescHeaps();
	void CreateConstantBuffer();
	void CreateRootSignature();
	void BuildShader();
	void CreatePSO();
	//void CreateBoxGeometry();
	void UploadVertex();
	void OnResize();
	
	void OnMouseDown(WPARAM btnState, int x, int y);
	void OnMouseUp(WPARAM btnState, int x, int y);
	void OnMouseMove(WPARAM btnState, int x, int y);

	//Render Tick
	
	void ClearForNextFrame();
	void CommitRenderCommand();
	void RenderNextFrame();
	void FlushCommandQueue();
	ID3D12Resource* CurrentBackBuffer()const;
	D3D12_CPU_DESCRIPTOR_HANDLE CurrentBackBufferView()const;
	D3D12_CPU_DESCRIPTOR_HANDLE DepthStencilView()const;


	HINSTANCE mHInstance = nullptr;
	HWND mhMainWnd = nullptr;
	int mClientWidth{ 800 }, mClientHeight{ 600 };
	int mRefreshRate{ 60 };
	std::wstring mMainWndCaption = L"XII Renderer";

	//device
	ComPtr<IDXGIFactory4> mdxgiFactory;
	ComPtr<IDXGISwapChain> mSwapChain;
	ComPtr<ID3D12Device> md3dDevice;
	bool      m4xMsaaState = false;    // 4X MSAA enabled
	UINT      m4xMsaaQuality = 0;      // quality level of 4X MSAA

	//Format
	DXGI_FORMAT mBackBufferFormat{ DXGI_FORMAT_R8G8B8A8_UNORM };
	DXGI_FORMAT mDepthStencilFormat{ DXGI_FORMAT_D24_UNORM_S8_UINT };

	//something about Command
	ComPtr<ID3D12CommandAllocator> mCommandAllocator;
	ComPtr<ID3D12CommandQueue> mCommandQueue;
	ComPtr<ID3D12GraphicsCommandList> mCommandList;
	ComPtr<ID3D12Fence> mFence;
	UINT64 mCurrentFence{ 0 };

	//Resource and Desc
	const static int mSwapChainBufferCount = 2;
	int mCurrentBackBufferCount = 0;
	Microsoft::WRL::ComPtr<ID3D12Resource> mSwapChainBuffer[mSwapChainBufferCount];
	Microsoft::WRL::ComPtr<ID3D12Resource> mDepthStencilBuffer;

	//Descriptor and view
	ComPtr<ID3D12DescriptorHeap> mRtvHeap;
	ComPtr<ID3D12DescriptorHeap> mDsvHeap;
	ComPtr<ID3D12DescriptorHeap> mCbvHeap;
	
	UINT mRtvDescriptorSize = 0;
	UINT mDsvDescriptorSize = 0;
	UINT mCbvSrvUavDescriptorSize = 0;
	D3D12_VIEWPORT mScreenViewport;
	D3D12_RECT mScissorRect;

	std::vector<D3D12_INPUT_ELEMENT_DESC> mInputLayout;

	//Constant Buffer
	std::unique_ptr<UploadBuffer<ObjectConstants>> mObjectCB = nullptr;
	ComPtr<ID3D12Resource> mUploadCBuffer;
	UINT mCBVSize = d3dUtil::CalcConstantBufferByteSize(sizeof(ObjectConstants));
	UINT mElementNums{ 1 };

	//Root Signature
	ComPtr<ID3D12RootSignature> mRootSignature;

	//Shader
	ComPtr<ID3DBlob> mvsByteCode;
	ComPtr<ID3DBlob> mpsByteCode;

	//PSO
	ComPtr<ID3D12PipelineState> mPSO;

	//Model
	std::unique_ptr<Model> mModel{std::make_unique<Model>("box", "C:\\Users\\LiYU\\source\\repos\\LiquidEngine\\LiquidEngine\\Resources\\Model\\box.fbx")};
	std::unique_ptr<MeshGeometry> mBoxGeo;

	XMFLOAT4X4 mWorld = MathHelper::Identity4x4();
	XMFLOAT4X4 mView = MathHelper::Identity4x4();
	XMFLOAT4X4 mProj = MathHelper::Identity4x4();

	float mTheta = 1.5f * XM_PI;
	float mPhi = XM_PIDIV4;
	float mRadius = 5.0f;

	POINT mLastMousePos;
};


