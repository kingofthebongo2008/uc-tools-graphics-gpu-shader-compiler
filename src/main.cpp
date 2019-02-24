// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"
#include <cstdint>
#include <assert.h>
#include <string>
#include <tuple>
#include <array>

#include "resource.h"

#pragma optimize("",off)
#pragma once

namespace uc
{
    namespace build
    {
        namespace tasks
        {
            enum shader_pipeline_stage : uint32_t
            {
                pixel = 0,
                vertex = 1,
                geometry = 2,
                hull = 3,
                domain = 4,
                compute = 5,
                root_signature = 6
            };

            enum backend : uint32_t
            {
                UniqueCreatorDev = 0,
                UniqueCreatorPublic = 1
            };

            namespace
            {
                std::string load_shader_resource(uint32_t resource)
                {
                    auto handle          = ::GetModuleHandle(NULL);
                    auto rc              = ::FindResource(handle, MAKEINTRESOURCE(resource), MAKEINTRESOURCE(TEXTFILE));
                    auto rcData          = ::LoadResource(handle, rc);
                    
                    auto    data         = static_cast<const char*>(::LockResource(rcData));
                    auto    size         = ::SizeofResource(handle, rc);
                    std::string s(data, size);
                    ::FreeResource(handle);
                    return s;
                }
            }

            std::tuple<std::string, std::string> cpp_generate_find_templates(uc::build::tasks::shader_pipeline_stage stage, backend b)
            {
                using namespace uc::build::tasks;

                std::array< std::uint32_t, 7 > cpp_name_backend_private;
                std::array< std::uint32_t, 7 > header_name_backend_private;

                std::array< std::uint32_t, 7 > cpp_name_backend_public;
                std::array< std::uint32_t, 7 > header_name_backend_public;

                std::array < std::array< std::uint32_t, 7 >*, 2> backend_cpp;
                std::array < std::array< std::uint32_t, 7 >*, 2> backend_header;


                cpp_name_backend_private[shader_pipeline_stage::pixel] = dx12_gpu_shaders_myshader_type_cpp_pixel_txt;
                cpp_name_backend_private[shader_pipeline_stage::vertex] = dx12_gpu_shaders_myshader_type_cpp_vertex_txt;
                cpp_name_backend_private[shader_pipeline_stage::geometry] = dx12_gpu_shaders_myshader_type_cpp_geometry_txt;

                cpp_name_backend_private[shader_pipeline_stage::hull] = dx12_gpu_shaders_myshader_type_cpp_hull_txt;
                cpp_name_backend_private[shader_pipeline_stage::domain] = dx12_gpu_shaders_myshader_type_cpp_domain_txt;
                cpp_name_backend_private[shader_pipeline_stage::compute] = dx12_gpu_shaders_myshader_type_cpp_compute_txt;
                cpp_name_backend_private[shader_pipeline_stage::root_signature] = dx12_gpu_root_signatures_myshader_type_cpp_txt;
                //--------------------------------------------------------------------------------------------------------------------------------------------
                header_name_backend_private[shader_pipeline_stage::pixel] = dx12_gpu_shaders_myshader_type_h_pixel_txt;
                header_name_backend_private[shader_pipeline_stage::vertex] = dx12_gpu_shaders_myshader_type_h_vertex_txt;
                header_name_backend_private[shader_pipeline_stage::geometry] = dx12_gpu_shaders_myshader_type_h_geometry_txt;

                header_name_backend_private[shader_pipeline_stage::hull] = dx12_gpu_shaders_myshader_type_h_hull_txt;
                header_name_backend_private[shader_pipeline_stage::domain] = dx12_gpu_shaders_myshader_type_h_domain_txt;
                header_name_backend_private[shader_pipeline_stage::compute] = dx12_gpu_shaders_myshader_type_h_compute_txt;
                header_name_backend_private[shader_pipeline_stage::root_signature] = dx12_gpu_root_signatures_myshader_type_h_txt;
                //--------------------------------------------------------------------------------------------------------------------------------------------
                cpp_name_backend_public[shader_pipeline_stage::pixel] = unique_creator_graphics_myshader_type_cpp_pixel_txt;
                cpp_name_backend_public[shader_pipeline_stage::vertex] = unique_creator_graphics_myshader_type_cpp_vertex_txt;
                cpp_name_backend_public[shader_pipeline_stage::geometry] = unique_creator_graphics_myshader_type_cpp_geometry_txt;

                cpp_name_backend_public[shader_pipeline_stage::hull] = unique_creator_graphics_myshader_type_cpp_hull_txt;
                cpp_name_backend_public[shader_pipeline_stage::domain] = unique_creator_graphics_myshader_type_cpp_domain_txt;
                cpp_name_backend_public[shader_pipeline_stage::compute] = unique_creator_graphics_myshader_type_cpp_compute_txt;
                cpp_name_backend_public[shader_pipeline_stage::root_signature] = unique_creator_graphics_root_signatures_myshader_type_cpp_txt;
                //--------------------------------------------------------------------------------------------------------------------------------------------
                header_name_backend_public[shader_pipeline_stage::pixel] = unique_creator_graphics_myshader_type_h_pixel_txt;
                header_name_backend_public[shader_pipeline_stage::vertex] = unique_creator_graphics_myshader_type_h_vertex_txt;
                header_name_backend_public[shader_pipeline_stage::geometry] = unique_creator_graphics_myshader_type_h_geometry_txt;

                header_name_backend_public[shader_pipeline_stage::hull] = unique_creator_graphics_myshader_type_h_hull_txt;
                header_name_backend_public[shader_pipeline_stage::domain] = unique_creator_graphics_myshader_type_h_domain_txt;
                header_name_backend_public[shader_pipeline_stage::compute] = unique_creator_graphics_myshader_type_h_compute_txt;
                header_name_backend_public[shader_pipeline_stage::root_signature] = unique_creator_graphics_root_signatures_myshader_type_h_txt;
                //--------------------------------------------------------------------------------------------------------------------------------------------
                auto index  = static_cast<uint32_t> (stage);
                auto backend = static_cast<uint32_t> (b);

                backend_cpp[0] = &cpp_name_backend_private;
                backend_cpp[1] = &cpp_name_backend_public;


                backend_header[0] = &header_name_backend_private;
                backend_header[1] = &header_name_backend_public;

                return std::tuple<std::string, std::string> (load_shader_resource( backend_cpp[backend]->operator[](index)), load_shader_resource(backend_header[backend]->operator[](index)));
            }
        }
    }
}

int32_t main(int32_t argc, const char* argv[])
{

    using namespace uc::build::tasks;

    static auto r = cpp_generate_find_templates(shader_pipeline_stage::pixel, backend::UniqueCreatorDev);
    
    

    return 0;
}

