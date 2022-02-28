// Hit information, aka ray payload
// This sample only carries a shading color and hit distance.
// Note that the payload should be kept as small as possible,
// and that its size must be declared in the corresponding
// D3D12_RAYTRACING_SHADER_CONFIG pipeline subobject.
struct HitInfo
{
  // color is is the color of the hit point
  // distance is distance between origin of ray and the hit point
  float4 colorAndDistance;
};

// Attributes output by the raytracing when hitting a surface,
// barycentric coordinates returned from triangle intersection shader
struct Attributes
{
  float2 bary;
};

float4 CalculateDiffuseLighting(float3 hitPosition, float3 normal, float4 color, float3 lightPos, float4 lightCol)
{
    float3 pixelToLight = normalize(lightPos - hitPosition);

    // Diffuse contribution.
    float fNDotL = max(0.1f, dot(pixelToLight, normal));

    return color * lightCol * fNDotL;
}