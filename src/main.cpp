// ucdev_include_parser.cpp : Defines the entry point for the console application.
//
#include "pch.h"
#include <cstdint>
#include <assert.h>
#include <string>
#include <tuple>
#include <array>

#include "resource.h"
#include <d3dcompiler.h>
#include <cxxopts.hpp>
#include <fstream>

#include <uc/util/utf8_conv.h>

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

            static uc::build::tasks::shader_pipeline_stage to_pipeline_stage(const std::string& s)
            {
                const std::array<shader_pipeline_stage, 7>  stages =
                {
                    shader_pipeline_stage::pixel,
                    shader_pipeline_stage::vertex,
                    shader_pipeline_stage::geometry,
                    shader_pipeline_stage::hull,
                    shader_pipeline_stage::domain,
                    shader_pipeline_stage::compute,
                    shader_pipeline_stage::root_signature
                };

                const std::array<std::string, 7> stages_text =
                {
                    "pixel",
                    "vertex",
                    "geometry",
                    "hull",
                    "domain",
                    "compute",
                    "rootsignature"
                };

                for (auto i = 0U; i < stages.size(); ++i)
                {
                    if (s == stages_text[i])
                    {
                        return stages[i];
                    }
                }

                return shader_pipeline_stage::vertex;
            }

            static std::string stage_to_command_line2(uc::build::tasks::shader_pipeline_stage s)
            {
                switch (s)
                {
                    case shader_pipeline_stage::pixel:
                    {
                        return std::string("ps_5_1");
                    }

                    case shader_pipeline_stage::vertex:
                    {
                        return std::string("vs_5_1");
                    }

                    case shader_pipeline_stage::geometry:
                    {
                        return std::string("gs_5_1");
                    }

                    case shader_pipeline_stage::hull:
                    {
                        return std::string("hs_5_1");
                    }

                    case shader_pipeline_stage::domain:
                    {
                        return std::string("ds_5_1");
                    }

                    case shader_pipeline_stage::compute:
                    {
                        return std::string("cs_5_1");
                    }

                    case shader_pipeline_stage::root_signature:
                    {
                        return std::string("rootsig_1_1");
                    }

                    default:
                    {
                        return std::string("vs_5_1");
                    }
                }
            }

            static std::string stage_to_command_line(uc::build::tasks::shader_pipeline_stage s)
            {
                return std::string("/T ") + stage_to_command_line2(s);
            }

            static std::string make_option(const std::vector<std::string>& data, const std::string& prefix, const std::string& delimiter = " ")
            {
                std::string result;

                for (auto i = 0U; i < data.size(); ++i)
                {
                    result += prefix;
                    result += delimiter;
                    result += data[i];
                    result += delimiter;
                }

                return result;
            }

            static inline std::string make_includes(const std::vector<std::string>& data)
            {
                return make_option(data, "/I");
            }

            static inline std::string make_macros(const std::vector<std::string>& data)
            {
                return make_option(data, "/D");
            }

            template <class Container>
            void split(const std::string& str, Container& cont,
                char delim = ' ')
            {
                std::size_t current, previous = 0;
                current = str.find(delim);
                while (current != std::string::npos) {
                    cont.push_back(str.substr(previous, current - previous));
                    previous = current + 1;
                    current = str.find(delim, previous);
                }
                cont.push_back(str.substr(previous, current - previous));
            }

            struct shader_macro
            {
                std::string m_name;
                std::string m_definition;

            };

            std::vector<shader_macro> make_shader_macros(const std::vector<std::string>& v)
            {
                std::vector<shader_macro> macros;

                for (auto i = 0U; i < v.size(); ++i)
                {
                    std::vector< std::string > s;
                    split(v[i], s, '=');

                    if (!s.empty())
                    {
                        shader_macro m;
                        m.m_name = s[0];

                        if (s.size() > 1)
                        {
                            m.m_definition = s[1];
                        }
                        macros.push_back(m);
                    }
                }

                return macros;
            }

            std::string hex_encode(const uint8_t* b, uint32_t size)
            {
                std::string s;

                std::array<const char, 16> hex_numbers = 
                {
                    '0', '1', '2', '3',
                    '4', '5', '6', '7',
                    '8', '9', 'a', 'b',
                    'c', 'd', 'e', 'f'
                };

                auto break_line = 0;
                for (auto i = 0U; i < size - 1; ++i)
                {
                    int32_t t = b[i];
                    int a = t / 16;
                    int b = t % 16;

                    s.push_back('0');
                    s.push_back('x');
                    s.push_back(hex_numbers[a]);
                    s.push_back(hex_numbers[b]);
                    s.push_back(',');

                    break_line++;

                    if (break_line == 16)
                    {
                        s.push_back('\n');
                        break_line = 0;
                    }
                }

                //one more iteration
                {
                    int32_t t = b[size-1];
                    int a = t / 16;
                    int b = t % 16;

                    s.push_back('0');
                    s.push_back('x');
                    s.push_back(hex_numbers[a]);
                    s.push_back(hex_numbers[b]);
                }

                return s;
            }

            static std::string replace_string(const std::string& base, const::std::string& search, const::std::string& replace)
            {
                if (search == replace)
                {
                    return base;
                }
                else
                {
                    const std::string s = search;
                    std::string header = base;
                    const std::string r = replace;

                    auto pos = header.find(s);

                    while (pos != std::string::npos)
                    {
                        header = header.replace(pos, s.length(), replace);
                        pos = header.find(s);
                    }

                    return header;
                }
            }

        }
    }
}

int32_t main(int32_t argc, char** argv)
{
    using namespace uc::build::tasks;

    cxxopts::Options options("uc-gpu-shader-compiler", "command line options");

    options.add_options()
        ("file", "shader file to compile", cxxopts::value<std::string>())
        ("backend", "backend for cpp files", cxxopts::value<std::string>())
        ("main", "entry point for shader", cxxopts::value<std::string>())
        ("type", "shader type (pixe, vertex, geometry, hull, domain, compute, rootsignature", cxxopts::value<std::string>())
        ("defines", "preprocessor definitions", cxxopts::value<std::vector<std::string>>())
        ("includes", "preprocessor includes", cxxopts::value<std::vector<std::string>>())
        ("cpp", "cpp file output", cxxopts::value<std::string>())
        ("header", "cpp header output", cxxopts::value<std::string>())
        ("type_name", "cpp type name", cxxopts::value<std::string>())
        ("help", "help");


    auto result = options.parse(argc, argv);

    std::vector<std::string> defines;
    std::vector<std::string> includes;
    std::string              file;
    std::string              backend;
    std::string              main;
    std::string              type;
    std::string              cpp;
    std::string              header;
    std::string              help;
    std::string              type_name;

    if (result["defines"].count() > 0)
    {
        defines = result["defines"].as<std::vector<std::string>>();
    }

    if (result["includes"].count() > 0)
    {
        defines = result["includes"].as<std::vector<std::string>>();
    }

    if (result["file"].count() > 0)
    {
        file = result["file"].as<std::string>();
    }

    if (result["backend"].count() > 0)
    {
        backend = result["backend"].as<std::string>();
    }

    if (result["main"].count() > 0)
    {
        main = result["main"].as<std::string>();
    }

    if (result["type_name"].count() > 0)
    {
        type_name = result["type_name"].as<std::string>();
    }

    if (result["type"].count() > 0)
    {
        type = result["type"].as<std::string>();
    }

    if (result["cpp"].count() > 0)
    {
        cpp = result["cpp"].as<std::string>();
    }

    if (result["header"].count() > 0)
    {
        header = result["header"].as<std::string>();
    }

    try
    {
        if (result["help"].count() > 0)
        {
            std::cout << options.help() << std::endl;
            return 0;
        }

        if (file.empty())
        {
            throw std::exception("missing file");
        }

        if (type.empty())
        {
            throw std::exception("missing type");
        }

        if (cpp.empty())
        {
            throw std::exception("missing cpp");
        }

        if (header.empty())
        {
            throw std::exception("missing header");
        }

        if (backend.empty())
        {
            backend = "dev";
        }

        if (main.empty())
        {
            main = "main";
        }

        if (type_name.empty())
        {
            type_name = "my_shader_type";
        }

        shader_pipeline_stage stage = to_pipeline_stage(type);

        auto macros = make_shader_macros(defines);
        std::vector< D3D_SHADER_MACRO  > d3d_macros(macros.size() + 1);
        for (auto i = 0U; i < macros.size(); ++i)
        {
            d3d_macros[i].Name = &macros[i].m_name[0];

            if (macros[i].m_definition.empty())
            {
                d3d_macros[i].Definition = nullptr;
            }
            else
            {
                d3d_macros[i].Definition = &macros[i].m_definition[0];
            }
        }
        d3d_macros[macros.size()].Name = nullptr;
        d3d_macros[macros.size()].Definition = nullptr;


        ID3DBlob* code = nullptr;
        ID3DBlob* errors = nullptr;

        std::wstring filew  = uc::util::utf16_from_utf8(file);
        std::string  target = stage_to_command_line2(stage);

        HRESULT hr = D3DCompileFromFile(
            filew.c_str(),
            &d3d_macros[0],
            D3D_COMPILE_STANDARD_FILE_INCLUDE,
            main.c_str(),
            target.c_str(),
            D3DCOMPILE_ENABLE_STRICTNESS | D3DCOMPILE_OPTIMIZATION_LEVEL3,
            0,
            &code,
            &errors
        );
        
        if ( SUCCEEDED( hr) )
        {
            auto encoded    = hex_encode(reinterpret_cast<uint8_t*>(code->GetBufferPointer()), static_cast<uint32_t>(code->GetBufferSize()));
            auto code       = cpp_generate_find_templates(stage, backend::UniqueCreatorDev);

            auto&& cpp_file_name_header = std::get<1>(code);
            auto&& cpp_file_name        = std::get<0>(code);
            auto&& header1              = replace_string(cpp_file_name_header, "my_shader_type", type_name);
            auto&& cpp_file             = replace_string(cpp_file_name, "my_shader_type_blob", std::move(encoded));
            auto&& cpp_file1            = replace_string(cpp_file, "my_shader_type", type_name);
            
            
            {
                std::ofstream f(cpp);
                f << cpp_file1;
            }

            {
                std::ofstream f(header);
                f << header1;
            }
        }
        else
        {


        }

        if (code)
        {
            code->Release();
        }

        if (errors)
        {
            std::string e;
            e.resize(errors->GetBufferSize());
            std::memcpy(&e[0], errors->GetBufferPointer(), errors->GetBufferSize());
            errors->Release();
            std::cout << e << std::endl;
            return 1;
        }
    }

    catch (const std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}

