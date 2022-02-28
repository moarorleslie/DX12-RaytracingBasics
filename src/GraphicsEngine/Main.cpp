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
#include "stdafx.h"
#include "GraphicsEngine.h"
//#define STB_IMAGE_IMPLEMENTATION
//#include <stb_image.h>

//#define TINYOBJLOADER_IMPLEMENTATION
//#include <tiny_obj_loader.h>

#include <comdef.h> //for bstr 
#include <unordered_map> // refactor to no stl if time

void ParseCommandLineArgs(WCHAR* argv[], int argc, ConfigInfo& config)
{
    for (int i = 1; i < argc; ++i)
    {
        if (_wcsnicmp(argv[i], L"-width", wcslen(argv[i])) == 0)
        {
            i++;
            WCHAR* wChar = argv[i];
            _bstr_t b(wChar);
            const char* argValue = b;
            config.width = atoi(argValue);
            continue;
        }

        if (_wcsnicmp(argv[i], L"-height", wcslen(argv[i])) == 0)
        {
            i++;
            WCHAR* wChar = argv[i];
            _bstr_t b(wChar);
            const char* argValue = b;
            config.height = atoi(argValue);
            continue;
        }

        if (_wcsnicmp(argv[i], L"-vsync", wcslen(argv[i])) == 0)
        {
            i++;
            WCHAR* wChar = argv[i];
            _bstr_t b(wChar);
            const char* argValue = b;
            config.vsync = atoi(argValue);
            continue;
        }
     /*   if (_wcsnicmp(argv[i], L"-model", wcslen(argv[i])) == 0)
        {
            i++;
            WCHAR* wChar = argv[i];
            _bstr_t b(wChar);
            const char* argValue = b;
            config.model = argValue;
            continue;
        }*/

    }
}

/*namespace std
{
    void hash_combine(size_t& seed, size_t hash)
    {
        hash += 0x9e3779b9 + (seed << 6) + (seed >> 2);
        seed ^= hash;
    }

    template<> struct hash<Vertex>
    {
        size_t operator()(Vertex const& vertex) const
        {
            size_t seed = 0;
            hash<float> hasher;
            hash_combine(seed, hasher(vertex.position.x));
            hash_combine(seed, hasher(vertex.position.y));
            hash_combine(seed, hasher(vertex.position.z));

            hash_combine(seed, hasher(vertex.uv.x));
            hash_combine(seed, hasher(vertex.uv.y));

            return seed;
        }
    };
}*/
// just testing, will move this somewhere else later
/*void LoadModel(std::string filePath, Model& model, Material &material) 
{
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    std::string err;

    // Load the OBJ and MTL files
    if (!tinyobj::LoadObj(&attrib, &shapes, &materials, &err, filePath.c_str(), "materials\\"))
    {
        throw std::runtime_error(err);
    }

    // Get the first material
    // Only support a single material right now
    material.name = materials[0].name;
    material.texturePath = materials[0].diffuse_texname;

    // Parse the model and store the unique vertices
    std::unordered_map<Vertex, uint32_t> uniqueVertices = {};
    for (const auto& shape : shapes)
    {
        for (const auto& index : shape.mesh.indices)
        {
            Vertex vertex = {};
            vertex.position =
            {
                attrib.vertices[3 * index.vertex_index + 2],
                attrib.vertices[3 * index.vertex_index + 1],
                attrib.vertices[3 * index.vertex_index + 0]
            };

            vertex.uv =
            {
                attrib.texcoords[2 * index.texcoord_index + 0],
                1 - attrib.texcoords[2 * index.texcoord_index + 1]
            };

            // Fast find unique vertices using a hash
            if (uniqueVertices.count(vertex) == 0)
            {
                uniqueVertices[vertex] = static_cast<uint32_t>(model.vertices.size());
                model.vertices.push_back(vertex);
            }

            model.indices.push_back(uniqueVertices[vertex]);
        }
    }

}
*/
_Use_decl_annotations_
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR lpCmdLine, int nCmdShow)
{
	ConfigInfo config;
    LPWSTR* argv = NULL;
    int argc = 0;
    argv = CommandLineToArgvW(GetCommandLine(), &argc);
    ParseCommandLineArgs(argv, argc, config);

    //Testing load model
   // Model model;
   // Material material;
  //  LoadModel(config.model, model, material);

    GraphicsEngine sample(L"D3D12 Hello Rawr", config);
   // sample.pModel = &model;
    return Win32Application::Run(&sample, hInstance, nCmdShow);
}


