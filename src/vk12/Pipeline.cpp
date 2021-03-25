#include "Pipeline.hpp"

namespace Shroon
{
    namespace Prism
    {
        void VK12Pipeline::Create(Pipeline & pl, PipelineSpecification spec)
        {
            std::array<VkShaderModule, 6> modules;
            std::vector<VkPipelineShaderStageCreateInfo> shaderStages;

            struct ShaderData
            {
                std::string Type;
                VkShaderStageFlagBits Enum;
                std::vector<uint32_t> * Code;
            };

            std::vector<ShaderData> shaderData{
                {"Vertex", VK_SHADER_STAGE_VERTEX_BIT, spec.VertexShader},
                {"Tessellation Control", VK_SHADER_STAGE_TESSELLATION_CONTROL_BIT, spec.TessellationControlShader},
                {"Tessellation Evaluation", VK_SHADER_STAGE_TESSELLATION_EVALUATION_BIT, spec.TessellationEvaluationShader},
                {"Geometry", VK_SHADER_STAGE_GEOMETRY_BIT, spec.GeometryShader},
                {"Fragment", VK_SHADER_STAGE_FRAGMENT_BIT, spec.FragmentShader},
                {"Compute", VK_SHADER_STAGE_COMPUTE_BIT, spec.ComputeShader}
            };

            for (uint16_t i = 0; i < 6; i++)
            {
                if (shaderData[i].Code)
                {
                    VkShaderModuleCreateInfo shaderModuleInfo{};
                    shaderModuleInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
                    shaderModuleInfo.codeSize = shaderData[i].Code->size();
                    shaderModuleInfo.pCode = shaderData[i].Code->data();

                    if (vkCreateShaderModule(VK12State::s_Device, &shaderModuleInfo, nullptr, &modules[i]) != VK_SUCCESS)
                    {
                        ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create " + shaderData[i].Type + " Shader module.");
                    }

                    VkPipelineShaderStageCreateInfo stageInfo{};
                    stageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
                    stageInfo.stage = shaderData[i].Enum;
                    stageInfo.module = modules[i];
                    stageInfo.pName = "main";

                    shaderStages.emplace_back(stageInfo);
                }
                else
                {
                    modules[i] = VK_NULL_HANDLE;
                }
            }

            VkPipelineVertexInputStateCreateInfo vertexInputInfo{};
            vertexInputInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
            vertexInputInfo.vertexBindingDescriptionCount = 1;
            vertexInputInfo.pVertexBindingDescriptions = nullptr;
            vertexInputInfo.vertexAttributeDescriptionCount = 2;
            vertexInputInfo.pVertexAttributeDescriptions = nullptr;

            VkPipelineInputAssemblyStateCreateInfo inputAssembly{};
            inputAssembly.sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
            inputAssembly.topology = static_cast<VkPrimitiveTopology>(spec.PipelineGeometryMode.Type);
            inputAssembly.primitiveRestartEnable = VK_FALSE;

            VkViewport viewport{};
            viewport.x = 0.0f;
            viewport.y = 0.0f;
            viewport.width = VK12State::s_SwapExtent.width;
            viewport.height = VK12State::s_SwapExtent.height;
            viewport.minDepth = 0.0f;
            viewport.minDepth = 1.0f;

            VkRect2D scissor{};
            scissor.offset = {0, 0};
            scissor.extent = VK12State::s_SwapExtent;

            VkPipelineViewportStateCreateInfo viewportStateInfo{};
            viewportStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
            viewportStateInfo.viewportCount = 1;
            viewportStateInfo.pViewports = &viewport;
            viewportStateInfo.scissorCount = 1;
            viewportStateInfo.pScissors = &scissor;

            VkPipelineRasterizationStateCreateInfo rasterizationStateInfo{};
            rasterizationStateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
            rasterizationStateInfo.depthClampEnable = VK_FALSE;
            rasterizationStateInfo.rasterizerDiscardEnable = VK_FALSE;
            rasterizationStateInfo.polygonMode = static_cast<VkPolygonMode>(spec.PipelinePolygonMode.Type);
            rasterizationStateInfo.lineWidth = 1.0f;
            rasterizationStateInfo.cullMode = VK_CULL_MODE_BACK_BIT;
            rasterizationStateInfo.frontFace = VK_FRONT_FACE_CLOCKWISE;
            rasterizationStateInfo.depthBiasEnable = VK_FALSE;
            rasterizationStateInfo.depthBiasConstantFactor = 0.0f;
            rasterizationStateInfo.depthBiasClamp = 0.0f;
            rasterizationStateInfo.depthBiasSlopeFactor = 0.0f;

            VkPipelineMultisampleStateCreateInfo multisampleInfo{};
            multisampleInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
            multisampleInfo.sampleShadingEnable = VK_FALSE;
            multisampleInfo.rasterizationSamples = VK_SAMPLE_COUNT_1_BIT;
            multisampleInfo.minSampleShading = 1.0f;
            multisampleInfo.pSampleMask = nullptr;
            multisampleInfo.alphaToCoverageEnable = VK_FALSE;
            multisampleInfo.alphaToOneEnable = VK_FALSE;

            VkPipelineColorBlendAttachmentState colorBlendAttachment{};
            colorBlendAttachment.colorWriteMask =   VK_COLOR_COMPONENT_R_BIT
                                                    | VK_COLOR_COMPONENT_G_BIT
                                                    | VK_COLOR_COMPONENT_B_BIT
                                                    | VK_COLOR_COMPONENT_A_BIT;
            colorBlendAttachment.blendEnable = VK_FALSE;
            colorBlendAttachment.srcColorBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachment.dstColorBlendFactor = VK_BLEND_FACTOR_ZERO;
            colorBlendAttachment.colorBlendOp = VK_BLEND_OP_ADD;
            colorBlendAttachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
            colorBlendAttachment.alphaBlendOp = VK_BLEND_OP_ADD;

            VkPipelineColorBlendStateCreateInfo colorBlendInfo{};
            colorBlendInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
            colorBlendInfo.logicOpEnable = VK_FALSE;
            colorBlendInfo.logicOp = VK_LOGIC_OP_COPY;
            colorBlendInfo.attachmentCount = 1;
            colorBlendInfo.pAttachments = &colorBlendAttachment;
            colorBlendInfo.blendConstants[0] = 0.0f;
            colorBlendInfo.blendConstants[1] = 0.0f;
            colorBlendInfo.blendConstants[2] = 0.0f;
            colorBlendInfo.blendConstants[3] = 0.0f;

            VkPipelineLayoutCreateInfo pipelineLayoutInfo{};
            pipelineLayoutInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
            pipelineLayoutInfo.setLayoutCount = 0;
            pipelineLayoutInfo.pSetLayouts = nullptr;
            pipelineLayoutInfo.pushConstantRangeCount = 0;
            pipelineLayoutInfo.pPushConstantRanges = nullptr;

            if (vkCreatePipelineLayout(VK12State::s_Device, &pipelineLayoutInfo, nullptr, pl.m_LayoutHandle.AsPtrOf<VkPipelineLayout>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create pipeline layout.");
            }

            VkGraphicsPipelineCreateInfo pipelineInfo{};
            pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
            pipelineInfo.stageCount = shaderStages.size();
            pipelineInfo.pStages = shaderStages.data();
            pipelineInfo.pVertexInputState = &vertexInputInfo;
            pipelineInfo.pInputAssemblyState = &inputAssembly;
            pipelineInfo.pViewportState = &viewportStateInfo;
            pipelineInfo.pRasterizationState = &rasterizationStateInfo;
            pipelineInfo.pMultisampleState = &multisampleInfo;
            pipelineInfo.pDepthStencilState = nullptr;
            pipelineInfo.pColorBlendState = &colorBlendInfo;
            pipelineInfo.pDynamicState = nullptr;
            pipelineInfo.layout = pl.m_LayoutHandle.As<VkPipelineLayout>();
            pipelineInfo.renderPass = spec.TargetPass.GetRaw().As<VkRenderPass>();
            pipelineInfo.subpass = spec.TargetSubpassIndex;
            pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
            pipelineInfo.basePipelineIndex = -1;

            for (auto & module : modules)
            {
                if (module != VK_NULL_HANDLE)
                {
                    vkDestroyShaderModule(VK12State::s_Device, module, nullptr);
                }
            }

            if (vkCreateGraphicsPipelines(VK12State::s_Device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, pl.m_Handle.AsPtrOf<VkPipeline>()) != VK_SUCCESS)
            {
                ErrorReporter(SHRN_PRISM_LEVEL_FATAL, "Vulkan1.2", "Failed to create pipeline.");
            }
        }

        void VK12Pipeline::Destroy(Pipeline & pl)
        {
            vkDestroyPipeline(VK12State::s_Device, pl.m_Handle.As<VkPipeline>(), nullptr);
            vkDestroyPipelineLayout(VK12State::s_Device, pl.m_LayoutHandle.As<VkPipelineLayout>(), nullptr);
        }

        void VK12Pipeline::Run(Pipeline & pl, std::vector<PipelineObject> & objects)
        {
        }
    }
}
