#include "Common.hlsl"

// color calculation for when a ray doesn't hit any geometry
// here it's assigned the background color to match the clear color in rasterization pipeline

[shader("miss")]
void Miss(inout HitInfo payload : SV_RayPayload)
{
    payload.colorAndDistance = float4(0.3f, 0.7f, 0.9f, -1.f);
}