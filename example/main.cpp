#include <chrono>
#include <fstream>

#define SHRN_PRISM_DEFAULT_ERROR_REPORTER
#include "Shroon/Prism/Prism.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Shroon::Prism;

HL::API Shroon::Prism::HL::s_GraphicsAPI = HL::API::ES30; // Can be HL::API::GL33.

const float Width = 800.0f;
const float Height = 600.0f;

float Vertices[] = {
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f,  0.5f,
};

float UVs[] = {
    0.0f,  0.0f,
    0.0f,  1.0f,
    1.0f,  1.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,
    0.0f,  0.0f,

    0.0f,  1.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,
    1.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  1.0f,

    1.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  1.0f,
    0.0f,  1.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,

    1.0f,  1.0f,
    1.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  1.0f,
    1.0f,  1.0f,

    0.0f,  1.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,
    1.0f,  0.0f,
    0.0f,  0.0f,
    0.0f,  1.0f,

    0.0f,  0.0f,
    1.0f,  1.0f,
    1.0f,  0.0f,
    1.0f,  1.0f,
    0.0f,  0.0f,
    0.0f,  1.0f,
};

float Normals[] = {
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
    0.0f,  0.0f, -1.0f,
                       
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
    0.0f,  0.0f,  1.0f,
                     
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
    -1.0f,  0.0f,  0.0f,
                     
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
    1.0f,  0.0f,  0.0f,
                       
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
    0.0f, -1.0f,  0.0f,
                       
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f,
    0.0f,  1.0f,  0.0f
};


bool Left = false, Right = false, Up = false, Down = false, RoughPlus = false, RoughMinus = false;

glm::vec3 translation(0.0f, 0.0f, -5.0f);
glm::vec3 rotation(0.0f);

std::vector<uint32_t> ReadSPIRVBinary(const std::string & path)
{
    std::vector<uint32_t> binary;

    {
        std::ifstream in(path, std::ios_base::ate | std::ios_base::binary);

        size_t fileSize = in.tellg();
        binary.resize(fileSize / sizeof(uint32_t));

        in.seekg(0);
        in.read(reinterpret_cast<char *>(binary.data()), fileSize);

        in.close();
    }

    return binary;
}

struct
{
    alignas(16) glm::vec3 LightDir;
    alignas(16) glm::vec3 LightCol;
} RendererData;

struct
{
    glm::mat4 Model;
    glm::mat4 Projection;
    float Roughness;
    float Specular;
} ObjectData;

int main()
{
    unsigned char * texData = new unsigned char[256 * 256 * 4];

    for (uint16_t y = 0; y < 256; y++)
    {
        for (uint16_t x = 0; x < 256; x++)
        {
            texData[(y * 256 + x) * 4] = x;
            texData[(y * 256 + x) * 4 + 1] = 255;
            texData[(y * 256 + x) * 4 + 2] = y;
            texData[(y * 256 + x) * 4 + 3] = 255;
        }
    }

    HL::APIManager::InitAPI();

    HL::WindowSpecification spec;
    spec.Resizable = false;
    spec.Width = Width;
    spec.Height = Height;
    spec.Title = "Prism Test";

    HL::Window window;
    window.Create(spec);

    window.SetKeyCallback([](uint32_t key, bool down, bool repeat)
    {
        if (key == HL::KeyCode::UpArrow && (down || repeat))
        {
            Up = true;
        }
        else
        {
            Up = false;
        }

        if (key == HL::KeyCode::DownArrow && (down || repeat))
        {
            Down = true;
        }
        else
        {
            Down = false;
        }

        if (key == HL::KeyCode::LeftArrow && (down || repeat))
        {
            Left = true;
        }
        else
        {
            Left = false;
        }

        if (key == HL::KeyCode::RightArrow && (down || repeat))
        {
            Right = true;
        }
        else
        {
            Right = false;
        }

        if (key == HL::KeyCode::W && (down || repeat))
        {
            RoughPlus = true;
        }
        else
        {
            RoughPlus = false;
        }

        if (key == HL::KeyCode::S && (down || repeat))
        {
            RoughMinus = true;
        }
        else
        {
            RoughMinus = false;
        }
    });

    RendererData.LightDir = {0.0f, 0.0f, 1.0f};
    RendererData.LightCol = {1.0f, 1.0f, 1.0f};

    ObjectData.Projection = glm::perspective(45.0f, (float)Width / (float)Height, 0.01f, 1000.0f);
    ObjectData.Roughness = 0.0f;
    ObjectData.Specular = 1.0f;

    HL::Buffer rendererDataUBO;
    HL::Buffer objectDataUBO;

    {
        HL::BufferSpecification spec;
        spec.Type = HL::BufferType::Uniform;
        spec.Dynamic = true;
        spec.Data = &RendererData;
        spec.Size = sizeof(RendererData);

        rendererDataUBO.Create(spec);

        spec.Data = &ObjectData;
        spec.Size = sizeof(ObjectData);

        objectDataUBO.Create(spec);
    }

    HL::VertexBufferInputDescription desc1, desc2, desc3;

    desc1.Rate = HL::VertexBufferInputDescription::InputRatePerVertex;

    HL::VertexBufferInputDescription::VertexAttributeDescription & attr1 = desc1.AttributeDescriptions.emplace_back();
    attr1.Type = HL::VertexAttribType::Float;
    attr1.Location = 0;
    attr1.ComponentCount = 3;
    attr1.Stride = sizeof(float) * 3;
    attr1.Offset = 0;

    desc2.Rate = HL::VertexBufferInputDescription::InputRatePerVertex;

    HL::VertexBufferInputDescription::VertexAttributeDescription & attr2 = desc2.AttributeDescriptions.emplace_back();
    attr2.Type = HL::VertexAttribType::Float;
    attr2.Location = 1;
    attr2.ComponentCount = 2;
    attr2.Stride = sizeof(float) * 2;
    attr2.Offset = 0;

    desc3.Rate = HL::VertexBufferInputDescription::InputRatePerVertex;

    HL::VertexBufferInputDescription::VertexAttributeDescription & attr3 = desc3.AttributeDescriptions.emplace_back();
    attr3.Type = HL::VertexAttribType::Float;
    attr3.Location = 2;
    attr3.ComponentCount = 3;
    attr3.Stride = sizeof(float) * 3;
    attr3.Offset = 0;

    HL::Pipeline pl;

    {
        HL::PipelineSpecification spec{};

        spec.VertexBufferInputDescriptions.emplace_back(desc1);
        spec.VertexBufferInputDescriptions.emplace_back(desc2);
        spec.VertexBufferInputDescriptions.emplace_back(desc3);

        auto vs = std::move(ReadSPIRVBinary("shader.vert.spv"));
        auto fs = std::move(ReadSPIRVBinary("shader.frag.spv"));

        spec.VertexShader = &vs;
        spec.FragmentShader = &fs;

        pl.Create(spec);
    }

    HL::Mesh mesh;

    {
        HL::MeshSpecification spec{};
        spec.VertexCount = 36;

        HL::BufferSpecification & posSpec = spec.BufferDescriptions.emplace_back().first;
        spec.BufferDescriptions.back().second = desc1;
        posSpec.Type = HL::BufferType::VertexArray;
        posSpec.Data = Vertices;
        posSpec.Size = sizeof(float) * 36 * 3;

        HL::BufferSpecification & uvSpec = spec.BufferDescriptions.emplace_back().first;
        spec.BufferDescriptions.back().second = desc2;
        uvSpec.Type = HL::BufferType::VertexArray;
        uvSpec.Data = UVs;
        uvSpec.Size = sizeof(float) * 36 * 2;

        HL::BufferSpecification & nrmSpec = spec.BufferDescriptions.emplace_back().first;
        spec.BufferDescriptions.back().second = desc3;
        nrmSpec.Type = HL::BufferType::VertexArray;
        nrmSpec.Data = Normals;
        nrmSpec.Size = sizeof(float) * 36 * 3;

        mesh.Create(spec);
    }

    HL::Texture tex;

    {
        HL::TextureSpecification spec{};
        spec.Type = HL::TextureType::Texture2D;
        spec.Width = 256;
        spec.Height = 256;
        spec.MinFilter = HL::TextureFilter::Nearest;
        spec.MagFilter = HL::TextureFilter::Linear;
        spec.WrapS = HL::TextureWrap::Repeat;
        spec.WrapT = HL::TextureWrap::Repeat;
        spec.InternalFormat = HL::TextureFormat::RGBA;
        spec.Format = HL::TextureFormat::RGBA;
        spec.Multisample = false;
        spec.UseMipmaps = false;
        spec.Data = texData;

        tex.Create(spec);
    }

    HL::Pass pass;

    {
        HL::PassSpecification::SubpassSpecification subpassSpec{};
        subpassSpec.SampleCount = 1;
        subpassSpec.Offscreen = false;
        subpassSpec.ClearFramebuffer = true;
        subpassSpec.UsePreviousOutput = false;
        subpassSpec.UseDepthStencil = true;
        subpassSpec.TargetPipeline = &pl;
        subpassSpec.TargetPipelineState.Flags = HL::PipelineState::FaceCullingBit;
        subpassSpec.TargetPipelineState.Primitive = HL::PrimitiveType::Triangles;
        subpassSpec.TargetPipelineState.Fill = HL::FillType::Fill;
        subpassSpec.TargetPipelineState.Cull = HL::CullFace::Back;

        HL::PassSpecification spec;
        spec.SubpassSpecs.emplace_back(subpassSpec);

        pass.Create(spec);
    }

    HL::Framebuffer fb;

    {
        HL::FramebufferSpecification spec{};
        spec.Width = Width;
        spec.Height = Height;
        spec.Depth = true;
        spec.Stencil = true;

        HL::TextureSpecification ca1Spec;
        ca1Spec.Type = HL::TextureType::Texture2D;
        ca1Spec.Width = Width;
        ca1Spec.Height = Height;
        ca1Spec.MinFilter = HL::TextureFilter::Nearest;
        ca1Spec.MagFilter = HL::TextureFilter::Linear;
        ca1Spec.WrapS = HL::TextureWrap::Repeat;
        ca1Spec.WrapT = HL::TextureWrap::Repeat;
        ca1Spec.InternalFormat = HL::TextureFormat::RGBA;
        ca1Spec.Format = HL::TextureFormat::RGBA;
        ca1Spec.Multisample = false;
        ca1Spec.UseMipmaps = false;
        ca1Spec.Data = nullptr;

        HL::TextureSpecification dsSpec;
        dsSpec.Type = HL::TextureType::Texture2D;
        dsSpec.Width = Width;
        dsSpec.Height = Height;
        dsSpec.MinFilter = HL::TextureFilter::Nearest;
        dsSpec.MagFilter = HL::TextureFilter::Linear;
        dsSpec.WrapS = HL::TextureWrap::Repeat;
        dsSpec.WrapT = HL::TextureWrap::Repeat;
        dsSpec.InternalFormat = HL::TextureFormat::DEPTH24_STENCIL8;
        dsSpec.Format = HL::TextureFormat::DEPTH24_STENCIL8;
        dsSpec.Multisample = false;
        dsSpec.UseMipmaps = false;
        dsSpec.Data = nullptr;

        spec.ColorAttachments.emplace_back(ca1Spec);
        spec.DepthStencilAttachment = dsSpec;

        fb.Create(spec);
    }

    HL::CommandBuffer cb;

    cb.Create();

    cb.BeginRecording();

    cb.CmdBindPass(pass, 0);
    cb.CmdBindFramebuffer(fb);

    cb.CmdClear();

    cb.CmdBindMesh(mesh);

    cb.CmdBindTexture(tex, 0);

    cb.CmdBindUBO("RendererData", rendererDataUBO.GetUBOBindPoint());
    cb.CmdBindUBO("ObjectData", objectDataUBO.GetUBOBindPoint());

    cb.CmdDraw(mesh.GetVertexCount(), 1);

    cb.EndRecording();

    struct FrameData
    {
        HL::CommandBuffer & CB;
        HL::Buffer & ObjDataUBO, & RenderDataUBO;
    } framedata{cb, objectDataUBO, rendererDataUBO};

    window.Update(fb, [](void * data)
    {
        FrameData & dt = *reinterpret_cast<FrameData *>(data);

        if (Up)
        {
            rotation.x -= 2.0f;
        }

        if (Down)
        {
            rotation.x += 2.0f;
        }

        if (Left)
        {
            rotation.y -= 2.0f;
        }

        if (Right)
        {
            rotation.y += 2.0f;
        }

        if (RoughPlus)
        {
            ObjectData.Roughness += 0.02f;

            ObjectData.Roughness = std::min(ObjectData.Roughness, 1.0f);
        }
        else if (RoughMinus)
        {
            ObjectData.Roughness -= 0.02f;

            ObjectData.Roughness = std::max(ObjectData.Roughness, 0.0f);
        }

        ObjectData.Model = glm::identity<glm::mat4>();
        ObjectData.Model = glm::translate(ObjectData.Model, translation);
        ObjectData.Model = glm::rotate(ObjectData.Model, glm::radians(rotation.x), glm::vec3{1.0f, 0.0f, 0.0f});
        ObjectData.Model = glm::rotate(ObjectData.Model, glm::radians(rotation.y), glm::vec3{0.0f, 1.0f, 0.0f});
        ObjectData.Model = glm::rotate(ObjectData.Model, glm::radians(rotation.z), glm::vec3{0.0f, 0.0f, 1.0f});

        dt.ObjDataUBO.Write(&ObjectData, 0, sizeof(ObjectData));

        dt.CB.Submit();
    }, &framedata);
}
