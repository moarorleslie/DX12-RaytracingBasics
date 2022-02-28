# DX12-RaytracingBasics

- Basic DX12 Raytracing Engine
  - Reference: https://developer.nvidia.com/rtx/raytracing/dxr/DX12-Raytracing-tutorial-Part-2 
- Features
  - Engine that can render geometry in rasterization pipeline or raytracing pipeline using spacebar key as a switch
  - A Key to toggle animation, mouse down to orbit, scroll wheel zoom
  - Implemented DXR Diffuse Lighting Example from Microsoft
  - Used instancing from acceleration structures
  - Added cube to pipeline with its own closest hit shader


- If incompatibility errors are showing, most likely the wrong gpu is being used, the app can be added to Graphics Settings in Windows, and the GPU can be set manually.
