#ifndef RENDERERAPI_H
#define RENDERERAPI_H

#include <stdio.h>
#include <stdbool.h>
#include <memory>
#include <stdint.h>
#include <glm/glm.hpp>
#include "lynx_common.h"

#define MAX_LOG_SIZE 2048

namespace Lynx
{

    enum ShaderType : unsigned int
    {
        SHADER_NULL, // = 0
        SHADER_VERTEX,
        SHADER_FRAGMENT,
        SHADER_GEOMETRY,
        SHADER_COMPUTE
    };

    enum TextureType : unsigned int
    {
        TEXTURE_1D,
        TEXTURE_2D,
        TEXTURE_3D,
        TEXTURE_1D_ARRAY,
        TEXTURE_2D_ARRAY,
        TEXTURE_CUBE_MAP,
        TEXTURE_CUBE_MAP_ARRAY
    };

    ShaderType StrToShaderType(const char *str);
    const char *ShaderTypeToStr(ShaderType type);

    enum API : unsigned int
    {
        API_OPENGL
    };

    // Abstract base class of the renderer API's, should only be used as base for other platform API's
    class IRendererAPI
    {

    public:
        virtual void Init() = 0;

        virtual void SetViewport(uint32_t width, uint32_t height) = 0;
        virtual void Clear(glm::vec4 color) = 0;

        virtual void LoadTexture(TextureType type, unsigned char *data, int width, int height, bool useDefaults = true, int format = 0x1908) = 0;
        virtual unsigned int GenTexture() = 0;
        virtual void SetTextureParameter(unsigned int param, unsigned int value) = 0;
        virtual void UseTexture(unsigned int id) = 0;
        virtual void BindTexture(unsigned int tex) = 0;
        virtual void BindTexture(TextureType type, unsigned int tex) = 0;

        virtual unsigned int CompileShader(const char *shadersource, ShaderType type) = 0;
        virtual int GetShaderUniformLocation(unsigned int programID, const char *uniformName) = 0;
		virtual int GetShaderAttribLocation(unsigned int ProgramID, const char *attribName) = 0;
		virtual void SetShaderUniformBool(int location, bool value) = 0; // Can't use virtual templates :( sad
        virtual void SetShaderUniformInt(int location, int value) = 0;
        virtual void SetShaderUniformFloat(int location, float value) = 0;
        virtual void SetShaderUniformVec2(int location, glm::vec2 value) = 0;
        virtual void SetShaderUniformVec3(int location, glm::vec3 value) = 0;
        virtual void SetShaderUniformVec4(int location, glm::vec4 value) = 0;
        virtual void SetShaderUniformMat2(int location, glm::mat2 value) = 0;
        virtual void SetShaderUniformMat3(int location, glm::mat3 value) = 0;
        virtual void SetShaderUniformMat4(int location, glm::mat4 value) = 0;
        virtual void DestroyShader(unsigned int shaderID) = 0;

        virtual void CheckErrors(const char *f, int l) = 0;

        virtual void DrawIndexed(int n) = 0;
        virtual void DrawArrays(int n) = 0;

        char *GetError() { return error_log; };

        static const char *GetAPIStr();
        static API GetAPI() { return m_API; };
        static std::unique_ptr<IRendererAPI> Create();

    private:
        static API m_API;

    protected:
        char error_log[MAX_LOG_SIZE];
    };

    // Static instance of the renderer API
    class LYNXENGINE_API RendererAPI
    {
    public:
        static void Init()
        {
            m_rendererAPI = IRendererAPI::Create();

            m_rendererAPI->Init();
        }

        static void SetViewport(uint32_t width, uint32_t height);
        static void Clear(glm::vec4 color);

        static void LoadTexture(TextureType type, unsigned char *data, int width, int height, bool useDefaults = true);
        static unsigned int GenTexture();
        static void SetTextureParameter(unsigned int param, unsigned int value);
        static void UseTexture(unsigned int id);
        static void BindTexture(TextureType type, unsigned int tex);
        static void BindTexture(unsigned int tex);

        static unsigned int CompileShader(const char *shadersource, ShaderType type);
        static int GetShaderUniformLocation(unsigned int programID, const char *uniformName);
		static int GetShaderAttribLocation(unsigned int ProgramID, const char *attribName);
		
        template <typename T>
        static void SetShaderUniform(int locatiom, T value);

        static void DestroyShader(unsigned int shaderID);

        static void CheckErrors(const char *f, int l);

        static char *GetError();

        static void DrawIndexed(int n);
        static void DrawArrays(int n);

    private:
        static std::unique_ptr<IRendererAPI> m_rendererAPI;
    };
}

LYNXENGINE_API void check_api_errors(const char *f, int l);
#ifdef __linux__
#define API_CheckErrors() check_api_errors(__PRETTY_FUNCTION__, __LINE__)
#elif _WIN32
#define API_CheckErrors() check_api_errors(__FUNCDNAME__, __LINE__)
#endif

#endif
