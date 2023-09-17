#pragma once
#include <d3dx12.h>
#include <vector>

namespace MyFunction {

    template<class T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;

    static const UINT FrameBufferCount = 2;

    // ���\�[�X����
    ComPtr<ID3D12Resource1> CreateResource(
        const CD3DX12_RESOURCE_DESC& desc,
        D3D12_RESOURCE_STATES resourceStates,
        const D3D12_CLEAR_VALUE* clearValue,
        D3D12_HEAP_TYPE heapType
    );

    void WriteToUploadHeapMemory(ID3D12Resource1* resource, uint32_t size, const void* data);
}