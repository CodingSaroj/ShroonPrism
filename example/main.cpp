#include <chrono>
#include <fstream>

#define SHRN_PRISM_DEFAULT_ERROR_REPORTER
#include "Shroon/Prism/Prism.hpp"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace Shroon::Prism;

API Shroon::Prism::s_GraphicsAPI = API::VK12;

const float Width = 800.0f;
const float Height = 600.0f;

float Vertices[] = {
    0.5f, 0.5f, 0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f
};

float Normals[] = {
    1.0f, 1.0f, 1.0f,
    1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, -1.0f,
    -1.0f, 1.0f, 1.0f,
    -1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, 1.0f,
    1.0f, -1.0f, -1.0f,
    -1.0f, -1.0f, -1.0f
};

uint32_t Indices[] = {
    0, 3, 4,
    0, 4, 5,

    1, 0, 5,
    1, 5, 6,

    1, 2, 3,
    1, 3, 0,

    2, 1, 6,
    2, 6, 7,

    3, 2, 7,
    3, 7, 4,

    7, 6, 5,
    7, 5, 4
};

float QVertices[] = {
    1.0f, 1.0f, 0.0f,
    -1.0f, 1.0f, 0.0f,
    -1.0f, -1.0f, 0.0f,
    1.0f, -1.0f, 0.0f,
};

uint32_t QIndices[] = {
    0, 1, 2,
    2, 3, 0
};

bool Left = false, Right = false, Up = false, Down = false;

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

/*int main()
{
    APIManager::InitAPI();

    WindowSpecification spec;
    spec.Resizable = false;
    spec.Width = Width;
    spec.Height = Height;
    spec.Title = "Prism Test";

    Window window;
    window.Create(spec);

    Mesh mesh;

    {
        MeshSpecification spec;

        BufferSpecification vertBufSpec;

        vertBufSpec.Data = Vertices;
        vertBufSpec.Size = sizeof(float) * 24;
        vertBufSpec.Type.Type = BufferType::VertexArray;

        BufferSpecification nrmBufSpec;

        nrmBufSpec.Data = Normals;
        nrmBufSpec.Size = sizeof(float) * 24;
        nrmBufSpec.Type.Type = BufferType::VertexArray;

        BufferSpecification indBufSpec;

        indBufSpec.Data = Indices;
        indBufSpec.Size = sizeof(uint32_t) * 36;
        indBufSpec.Type.Type = BufferType::VertexArrayIndex;

        MeshSpecification::MeshBufferSpecification vertMBSpec;
        vertMBSpec.IsVertexAttrib = true;
        vertMBSpec.Spec = vertBufSpec;
        vertMBSpec.AttribIndex = 0;
        vertMBSpec.AttribType.Type = VertexAttribType::Float;
        vertMBSpec.AttribComponentCount = 3;
        vertMBSpec.AttribStride = sizeof(float) * 3;
        vertMBSpec.AttribOffset = 0;

        MeshSpecification::MeshBufferSpecification nrmMBSpec;
        nrmMBSpec.IsVertexAttrib = true;
        nrmMBSpec.Spec = vertBufSpec;
        nrmMBSpec.AttribIndex = 1;
        nrmMBSpec.AttribType.Type = VertexAttribType::Float;
        nrmMBSpec.AttribComponentCount = 3;
        nrmMBSpec.AttribStride = sizeof(float) * 3;
        nrmMBSpec.AttribOffset = 0;

        MeshSpecification::MeshBufferSpecification indMBSpec;
        indMBSpec.IsVertexAttrib = false;
        indMBSpec.Spec = indBufSpec;

        spec.VertexCount = 36;
        spec.Buffers = {vertMBSpec, indMBSpec, nrmMBSpec};

        mesh.Create(spec);
    }

    Mesh qmesh;

    {
        MeshSpecification spec;

        BufferSpecification vertBufSpec;

        vertBufSpec.Data = QVertices;
        vertBufSpec.Size = sizeof(float) * 12;
        vertBufSpec.Type.Type = BufferType::VertexArray;

        BufferSpecification indBufSpec;

        indBufSpec.Data = QIndices;
        indBufSpec.Size = sizeof(uint32_t) * 6;
        indBufSpec.Type.Type = BufferType::VertexArrayIndex;

        MeshSpecification::MeshBufferSpecification vertMBSpec;
        vertMBSpec.IsVertexAttrib = true;
        vertMBSpec.Spec = vertBufSpec;
        vertMBSpec.AttribIndex = 0;
        vertMBSpec.AttribType.Type = VertexAttribType::Float;
        vertMBSpec.AttribComponentCount = 3;
        vertMBSpec.AttribStride = sizeof(float) * 3;
        vertMBSpec.AttribOffset = 0;

        MeshSpecification::MeshBufferSpecification indMBSpec;
        indMBSpec.IsVertexAttrib = false;
        indMBSpec.Spec = indBufSpec;

        spec.VertexCount = 6;
        spec.Buffers = {vertMBSpec, indMBSpec};

        qmesh.Create(spec);
    }

    Pipeline pipeline;

    {
        ShaderSpecification spec;
        spec.TessellationControlShader = nullptr;
        spec.TessellationEvaluationShader = nullptr;
        spec.GeometryShader = nullptr;
        spec.ComputeShader = nullptr;

        auto vert = std::move(ReadSPIRVBinary("shader.vert.spv"));
        auto frag = std::move(ReadSPIRVBinary("shader.frag.spv"));

        spec.VertexShader = &vert;
        spec.FragmentShader = &frag;

        PipelineSpecification plspec;
        plspec.DepthTesting = true;
        plspec.StencilTesting = false;
        plspec.PipelineShaderSpec = spec;

        pipeline.Create(plspec);
    }
    
    Pipeline pipeline1;

    {
        ShaderSpecification spec;
        spec.TessellationControlShader = nullptr;
        spec.TessellationEvaluationShader = nullptr;
        spec.GeometryShader = nullptr;
        spec.ComputeShader = nullptr;

        auto vert = std::move(ReadSPIRVBinary("pass2.vert.spv"));
        auto frag = std::move(ReadSPIRVBinary("pass2.frag.spv"));

        spec.VertexShader = &vert;
        spec.FragmentShader = &frag;

        PipelineSpecification plspec;
        plspec.DepthTesting = true;
        plspec.StencilTesting = false;
        plspec.PipelineShaderSpec = spec;

        pipeline1.Create(plspec);
    }

    struct RendererData
    {
        alignas(16) glm::vec3 LightDirection;
        alignas(16) glm::vec3 LightColor;
    } rdata{{1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}};

    UniformManager::CreateUniformBlock("RendererData", sizeof(RendererData));
    UniformManager::UpdateUniformBlock("RendererData", &rdata, sizeof(RendererData));

    std::vector<PipelineObject> objects;

    glm::mat4 projection(1.0f);

    float scale = 500.0f;

    //projection = glm::ortho(-Width / (2 * scale), Width / (2 * scale), -Height / (2 * scale), Height / (2 * scale), -10.0f, 10.0f);
    projection = glm::perspective(glm::radians(45.0f), float(Width) / float(Height), 0.01f, 1000.0f);

    //for (int32_t y = -20; y < 20; y++)
    //{
    //    for (int32_t x = -20; x < 20; x++)
        {
            PipelineObject obj;
            obj.ObjectMesh = mesh;
            obj.ObjectData.resize(sizeof(float) * 32);

            glm::mat4 Model = glm::identity<glm::mat4>();

            memcpy(obj.ObjectData.data(), glm::value_ptr(Model), sizeof(float) * 16);
            memcpy(obj.ObjectData.data() + (sizeof(float) * 16), glm::value_ptr(projection), sizeof(float) * 16);

            objects.emplace_back(std::move(obj));
        }
    //}

    std::vector<PipelineObject> objects1;

    {
        PipelineObject obj;
        obj.ObjectMesh = qmesh;
        obj.ObjectData.clear();

        objects1.emplace_back(std::move(obj));
    }

    Framebuffer fb;

    {
        FramebufferSpecification spec;
        spec.Width = Width;
        spec.Height = Height;
        spec.Depth = true;
        spec.Stencil = true;

        TextureSpecification cspec;
        cspec.Type = TextureType::Texture2D;
        cspec.Width = Width;
        cspec.Height = Height;
        cspec.Format = TextureFormat::RGBA;
        cspec.InternalFormat = TextureFormat::RGBA;
        cspec.MinFilter = TextureFilter::Linear;
        cspec.MagFilter = TextureFilter::Linear;
        cspec.WrapS = TextureWrap::Repeat;
        cspec.WrapT = TextureWrap::Repeat;
        cspec.UseMipmaps = false;
        cspec.Data = nullptr;

        spec.ColorAttachments.emplace_back(cspec);

        spec.DepthStencilAttachment.Type = TextureType::Texture2D;
        spec.DepthStencilAttachment.Width = Width;
        spec.DepthStencilAttachment.Height = Height;
        spec.DepthStencilAttachment.Format = TextureFormat::DEPTH24_STENCIL8;
        spec.DepthStencilAttachment.InternalFormat = TextureFormat::DEPTH24_STENCIL8;
        spec.DepthStencilAttachment.MinFilter = TextureFilter::Linear;
        spec.DepthStencilAttachment.MagFilter = TextureFilter::Linear;
        spec.DepthStencilAttachment.WrapS = TextureWrap::Repeat;
        spec.DepthStencilAttachment.WrapT = TextureWrap::Repeat;
        spec.DepthStencilAttachment.UseMipmaps = false;
        spec.DepthStencilAttachment.Data = nullptr;

        fb.Create(spec);
    }

    UniformManager::CreateUniformBlock("ObjectData", sizeof(float) * 32);

    struct UserData
    {
        glm::mat4 Projection;
        Framebuffer fb;
        Pipeline pipeline;
        Pipeline pipeline1;
        std::vector<PipelineObject> & objects;
        std::vector<PipelineObject> & objects1;
    } udata{projection, fb, pipeline, pipeline1, objects, objects1};

    window.SetKeyCallback([](uint32_t key, bool down, bool repeat)
    {
        if (key == KeyCode::UpArrow && (down || repeat))
        {
            Up = true;
        }
        else
        {
            Up = false;
        }

        if (key == KeyCode::DownArrow && (down || repeat))
        {
            Down = true;
        }
        else
        {
            Down = false;
        }

        if (key == KeyCode::LeftArrow && (down || repeat))
        {
            Left = true;
        }
        else
        {
            Left = false;
        }

        if (key == KeyCode::RightArrow && (down || repeat))
        {
            Right = true;
        }
        else
        {
            Right = false;
        }
    });

    window.Update(fb, [](void * data)
    {
        auto & udata = *reinterpret_cast<UserData *>(data);

        glm::mat4 & model = *reinterpret_cast<glm::mat4 *>(udata.objects[0].ObjectData.data());

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

        model = glm::identity<glm::mat4>();
        model = glm::translate(model, translation);
        model = glm::rotate(model, glm::radians(rotation.x), glm::vec3{1.0f, 0.0f, 0.0f});
        model = glm::rotate(model, glm::radians(rotation.y), glm::vec3{0.0f, 1.0f, 0.0f});
        model = glm::rotate(model, glm::radians(rotation.z), glm::vec3{0.0f, 0.0f, 1.0f});

        udata.fb.Enable();
        udata.fb.SetClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        udata.fb.ClearBuffers();
        udata.pipeline.Run(udata.objects);
        udata.fb.Disable();
    }, &udata);

    fb.Destroy();
    pipeline1.Destroy();
    pipeline.Destroy();

    UniformManager::Destroy();

    qmesh.Destroy();
    mesh.Destroy();
}
*/
