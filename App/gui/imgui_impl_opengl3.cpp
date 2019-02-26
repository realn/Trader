// dear imgui: Renderer for OpenGL3 / OpenGL ES2 / OpenGL ES3 (modern OpenGL with shaders / programmatic pipeline)
// This needs to be used along with a Platform Binding (e.g. GLFW, SDL, Win32, custom..)
// (Note: We are using GL3W as a helper library to access OpenGL functions since there is no standard header to access modern OpenGL functions easily. Alternatives are GLEW, Glad, etc..)

// Implemented features:
//  [X] Renderer: User texture binding. Use 'GLuint' OpenGL texture identifier as void*/ImTextureID. Read the FAQ about ImTextureID in imgui.cpp.

// You can copy and use unmodified imgui_impl_* files in your project. See main.cpp for an example of using this.
// If you are new to dear imgui, read examples/README.txt and read the documentation at the top of imgui.cpp.
// https://github.com/ocornut/imgui

// CHANGELOG
// (minor and older changes stripped away, please see git history for details)
//  2019-02-20: OpenGL: Fix for OSX not supporting OpenGL 4.5, we don't try to read GL_CLIP_ORIGIN even if defined by the headers/loader.
//  2019-02-11: OpenGL: Projecting clipping rectangles correctly using draw_data->FramebufferScale to allow multi-viewports for retina display.
//  2019-02-01: OpenGL: Using GLSL 410 shaders for any version over 410 (e.g. 430, 450).
//  2018-11-30: Misc: Setting up io.BackendRendererName so it can be displayed in the About Window.
//  2018-11-13: OpenGL: Support for GL 4.5's glClipControl(GL_UPPER_LEFT) / GL_CLIP_ORIGIN.
//  2018-08-29: OpenGL: Added support for more OpenGL loaders: glew and glad, with comments indicative that any loader can be used.
//  2018-08-09: OpenGL: Default to OpenGL ES 3 on iOS and Android. GLSL version default to "#version 300 ES".
//  2018-07-30: OpenGL: Support for GLSL 300 ES and 410 core. Fixes for Emscripten compilation.
//  2018-07-10: OpenGL: Support for more GLSL versions (based on the GLSL version string). Added error output when shaders fail to compile/link.
//  2018-06-08: Misc: Extracted imgui_impl_opengl3.cpp/.h away from the old combined GLFW/SDL+OpenGL3 examples.
//  2018-06-08: OpenGL: Use draw_data->DisplayPos and draw_data->DisplaySize to setup projection matrix and clipping rectangle.
//  2018-05-25: OpenGL: Removed unnecessary backup/restore of GL_ELEMENT_ARRAY_BUFFER_BINDING since this is part of the VAO state.
//  2018-05-14: OpenGL: Making the call to glBindSampler() optional so 3.2 context won't fail if the function is a NULL pointer.
//  2018-03-06: OpenGL: Added const char* glsl_version parameter to ImGui_ImplOpenGL3_Init() so user can override the GLSL version e.g. "#version 150".
//  2018-02-23: OpenGL: Create the VAO in the render function so the setup can more easily be used with multiple shared GL context.
//  2018-02-16: Misc: Obsoleted the io.RenderDrawListsFn callback and exposed ImGui_ImplSdlGL3_RenderDrawData() in the .h file so you can call it yourself.
//  2018-01-07: OpenGL: Changed GLSL shader version from 330 to 150.
//  2017-09-01: OpenGL: Save and restore current bound sampler. Save and restore current polygon mode.
//  2017-05-01: OpenGL: Fixed save and restore of current blend func state.
//  2017-05-01: OpenGL: Fixed save and restore of current GL_ACTIVE_TEXTURE.
//  2016-09-05: OpenGL: Fixed save and restore of current scissor rectangle.
//  2016-07-29: OpenGL: Explicitly setting GL_UNPACK_ROW_LENGTH to reduce issues because SDL changes it. (#752)

//----------------------------------------
// OpenGL    GLSL      GLSL
// version   version   string
//----------------------------------------
//  2.0       110       "#version 110"
//  2.1       120
//  3.0       130
//  3.1       140
//  3.2       150       "#version 150"
//  3.3       330
//  4.0       400
//  4.1       410       "#version 410 core"
//  4.2       420
//  4.3       430
//  ES 2.0    100       "#version 100"
//  ES 3.0    300       "#version 300 es"
//----------------------------------------

#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#include "imgui_impl_opengl3.h"
#include <stdio.h>
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif
#if defined(__APPLE__)
#include "TargetConditionals.h"
#endif

// iOS, Android and Emscripten can use GL ES 3
// Call ImGui_ImplOpenGL3_Init() with "#version 300 es"
#if (defined(__APPLE__) && TARGET_OS_IOS) || (defined(__ANDROID__)) || (defined(__EMSCRIPTEN__))
#define USE_GL_ES3
#endif

//#ifdef USE_GL_ES3
//// OpenGL ES 3
//#include <GLES3/gl3.h>  // Use GL ES 3
//#else
//// Regular OpenGL
//// About OpenGL function loaders: modern OpenGL doesn't have a standard header file and requires individual function pointers to be loaded manually.
//// Helper libraries are often used for this purpose! Here we are supporting a few common ones: gl3w, glew, glad.
//// You may use another loader/header of your choice (glext, glLoadGen, etc.), or chose to manually implement your own.
//#if defined(IMGUI_IMPL_OPENGL_LOADER_GL3W)
//#include <GL/gl3w.h>
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLEW)
//#include <GL/glew.h>
//#elif defined(IMGUI_IMPL_OPENGL_LOADER_GLAD)
//#include <glad/glad.h>
//#else
//#include IMGUI_IMPL_OPENGL_LOADER_CUSTOM
//#endif
//#endif

#include <glm/glm.hpp>

#include <CBGL/Consts.h>
#include <CBGL/Texture.h>
#include <CBGL/Program.h>
#include <CBGL/Shader.h>
#include <CBGL/Buffer.h>
#include <CBGL/State.h>
#include <CBGL/Rendering.h>
#include <CBGL/VertexDefinition.h>

#include <CBCore/StringConvert.h>

std::unique_ptr<cb::gl::CTexture> g_FontTexture;
std::unique_ptr<cb::gl::CProgram> g_ShaderHangle;
std::unique_ptr<cb::gl::CShader> g_VertHandle;
std::unique_ptr<cb::gl::CShader> g_FragHandle;
std::unique_ptr<cb::gl::CBuffer> g_VboHandle;
std::unique_ptr<cb::gl::CBuffer> g_ElementsHandle;

// OpenGL Data
static char         g_GlslVersionString[32] = "";
//static GLuint       g_FontTexture = 0;
//static GLuint       g_ShaderHandle = 0, g_VertHandle = 0, g_FragHandle = 0;
static int          g_AttribLocationTex = 0, g_AttribLocationProjMtx = 0;
static int          g_AttribLocationPosition = 0, g_AttribLocationUV = 0, g_AttribLocationColor = 0;
//static unsigned int g_VboHandle = 0, g_ElementsHandle = 0;

// Functions
bool    ImGui_ImplOpenGL3_Init(const char* glsl_version)
{
  ImGuiIO& io = ImGui::GetIO();
  io.BackendRendererName = "imgui_impl_opengl3";

  // Store GLSL version string so we can refer to it later in case we recreate shaders. Note: GLSL version is NOT the same as GL version. Leave this to NULL if unsure.
#ifdef USE_GL_ES3
  if (glsl_version == NULL)
    glsl_version = "#version 300 es";
#else
  if (glsl_version == NULL)
    glsl_version = "#version 130";
#endif
  IM_ASSERT((int)strlen(glsl_version) + 2 < IM_ARRAYSIZE(g_GlslVersionString));
  strcpy(g_GlslVersionString, glsl_version);
  strcat(g_GlslVersionString, "\n");

  return true;
}

void    ImGui_ImplOpenGL3_Shutdown()
{
  ImGui_ImplOpenGL3_DestroyDeviceObjects();
}

void    ImGui_ImplOpenGL3_NewFrame()
{
  if (!g_FontTexture)
    ImGui_ImplOpenGL3_CreateDeviceObjects();
}

// OpenGL3 Render function.
// (this used to be set in io.RenderDrawListsFn and called by ImGui::Render(), but you can now call this directly from your main loop)
// Note that this implementation is little overcomplicated because we are saving/setting up/restoring every OpenGL state explicitly, in order to be able to run within any OpenGL engine that doesn't do so.
void    ImGui_ImplOpenGL3_RenderDrawData(ImDrawData* draw_data)
{
  // Avoid rendering when minimized, scale coordinates for retina displays (screen coordinates != framebuffer coordinates)
  int fb_width = (int)(draw_data->DisplaySize.x * draw_data->FramebufferScale.x);
  int fb_height = (int)(draw_data->DisplaySize.y * draw_data->FramebufferScale.y);
  if (fb_width <= 0 || fb_height <= 0)
    return;

  // Backup GL state
  //GLenum last_active_texture; glGetIntegerv(GL_ACTIVE_TEXTURE, (GLint*)&last_active_texture);
  //glActiveTexture(GL_TEXTURE0);
  //GLint last_program; glGetIntegerv(GL_CURRENT_PROGRAM, &last_program);
  //GLint last_texture; glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
#ifdef GL_SAMPLER_BINDING
  GLint last_sampler; glGetIntegerv(GL_SAMPLER_BINDING, &last_sampler);
#endif
  //GLint last_array_buffer; glGetIntegerv(GL_ARRAY_BUFFER_BINDING, &last_array_buffer);
  //GLint last_vertex_array; glGetIntegerv(GL_VERTEX_ARRAY_BINDING, &last_vertex_array);
  auto last_viewport = cb::gl::getViewport();
  auto last_scissor_box = cb::gl::getScissorRect();
  auto last_blend_state = cb::gl::getBlendState();
  auto last_cull_state = cb::gl::getCullState();
  auto last_depth_state = cb::gl::getDepthState();

  // Setup render state: alpha-blending enabled, no face culling, no depth testing, scissor enabled, polygon fill
  {
    auto state = cb::gl::CBlendState();
    state.SrcFactor = cb::gl::BlendFactor::SRC_ALPHA;
    state.DstFactor = cb::gl::BlendFactor::ONE_MINUS_SRC_ALPHA;
    state.ColorFunc = cb::gl::BlendFunc::ADD;
    cb::gl::setState(state);
  }

  cb::gl::setStateEnabled(cb::gl::State::BLEND, true);
  cb::gl::setStateEnabled(cb::gl::State::CULL_FACE, false);
  cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, false);

  // Setup viewport, orthographic projection matrix
  // Our visible imgui space lies from draw_data->DisplayPos (top left) to draw_data->DisplayPos+data_data->DisplaySize (bottom right). DisplayMin is typically (0,0) for single viewport apps.
  {
    auto viewport = cb::gl::CViewport{
      {0, 0},
      { (cb::u32)fb_width, (cb::u32)fb_height }
    };
    cb::gl::setViewport(viewport);
  }

  float L = draw_data->DisplayPos.x;
  float R = draw_data->DisplayPos.x + draw_data->DisplaySize.x;
  float T = draw_data->DisplayPos.y;
  float B = draw_data->DisplayPos.y + draw_data->DisplaySize.y;
  auto ortho_projection = glm::mat4
  {
      { 2.0f / (R - L),   0.0f,         0.0f,   0.0f },
      { 0.0f,         2.0f / (T - B),   0.0f,   0.0f },
      { 0.0f,         0.0f,        -1.0f,   0.0f },
      { (R + L) / (L - R),  (T + B) / (B - T),  0.0f,   1.0f },
  };

  g_ShaderHangle->Bind();
  g_ShaderHangle->SetUniform(g_AttribLocationTex, 0);
  g_ShaderHangle->SetUniform(g_AttribLocationProjMtx, ortho_projection);
  bool clip_origin_lower_left = true;

  //    glUseProgram(g_ShaderHandle);
  //    glUniform1i(g_AttribLocationTex, 0);
  //    glUniformMatrix4fv(g_AttribLocationProjMtx, 1, GL_FALSE, &ortho_projection[0][0]);
  //#ifdef GL_SAMPLER_BINDING
  //    glBindSampler(0, 0); // We use combined texture/sampler state. Applications using GL 3.3 may set that otherwise.
  //#endif
      // Recreate the VAO every time
      // (This is to easily allow multiple GL contexts. VAO are not shared among GL contexts, and we don't track creation/deletion of windows so we don't have an obvious key to use to cache them.)

  auto vao_list = std::vector<cb::gl::CVertexSource>{
    cb::gl::CVertexSource(g_AttribLocationPosition, cb::gl::DataType::FLOAT, 2, sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, pos)),
    cb::gl::CVertexSource(g_AttribLocationUV, cb::gl::DataType::FLOAT, 2, sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, uv)),
    cb::gl::CVertexSource(g_AttribLocationColor, cb::gl::DataType::UNSIGNED_BYTE, 4, sizeof(ImDrawVert), IM_OFFSETOF(ImDrawVert, col)),
  };
  auto vao_handle = cb::gl::CVertexDefinition(vao_list);

  vao_handle.Bind();

  // Will project scissor/clipping rectangles into framebuffer space
  ImVec2 clip_off = draw_data->DisplayPos;         // (0,0) unless using multi-viewports
  ImVec2 clip_scale = draw_data->FramebufferScale; // (1,1) unless using retina display which are often (2,2)

  // Render command lists
  for (int n = 0; n < draw_data->CmdListsCount; n++)
  {
    const ImDrawList* cmd_list = draw_data->CmdLists[n];
    size_t idx_buffer_offset = 0;

    g_VboHandle->Bind();
    g_VboHandle->SetDataRaw((const void*)cmd_list->VtxBuffer.Data, cmd_list->VtxBuffer.Size * sizeof(ImDrawVert), cb::gl::BufferUsage::STREAM_DRAW);

    g_ElementsHandle->Bind();
    g_ElementsHandle->SetDataRaw((const void*)cmd_list->IdxBuffer.Data, cmd_list->IdxBuffer.Size * sizeof(ImDrawIdx), cb::gl::BufferUsage::STREAM_DRAW);


    for (int cmd_i = 0; cmd_i < cmd_list->CmdBuffer.Size; cmd_i++)
    {
      const ImDrawCmd* pcmd = &cmd_list->CmdBuffer[cmd_i];
      if (pcmd->UserCallback)
      {
        // User callback (registered via ImDrawList::AddCallback)
        pcmd->UserCallback(cmd_list, pcmd);
      }
      else
      {
        // Project scissor/clipping rectangles into framebuffer space
        ImVec4 clip_rect;
        clip_rect.x = (pcmd->ClipRect.x - clip_off.x) * clip_scale.x;
        clip_rect.y = (pcmd->ClipRect.y - clip_off.y) * clip_scale.y;
        clip_rect.z = (pcmd->ClipRect.z - clip_off.x) * clip_scale.x;
        clip_rect.w = (pcmd->ClipRect.w - clip_off.y) * clip_scale.y;

        if (clip_rect.x < fb_width && clip_rect.y < fb_height && clip_rect.z >= 0.0f && clip_rect.w >= 0.0f)
        {
          // Apply scissor/clipping rectangle
          cb::gl::CViewport viewport;
          if (clip_origin_lower_left)
            viewport = cb::gl::CViewport{ { (int)clip_rect.x, (int)(fb_height - clip_rect.w) }, { (int)(clip_rect.z - clip_rect.x), (int)(clip_rect.w - clip_rect.y) } };
          else
            viewport = cb::gl::CViewport{ {(int)clip_rect.x, (int)clip_rect.y}, {(int)clip_rect.z, (int)clip_rect.w} }; // Support for GL 4.5's glClipControl(GL_UPPER_LEFT)

          cb::gl::setScissorRect(viewport);

          auto texture = reinterpret_cast<cb::gl::CTexture*>(pcmd->TextureId);

          // Bind texture, Draw
          texture->Bind();
          cb::gl::drawElements(cb::gl::PrimitiveType::TRIANGLES, pcmd->ElemCount, sizeof(ImDrawIdx) == 2 ? cb::gl::DataType::UNSIGNED_SHORT : cb::gl::DataType::UNSIGNED_INT, idx_buffer_offset);
        }
      }
      idx_buffer_offset += pcmd->ElemCount * sizeof(ImDrawIdx);
    }
  }
  vao_handle.UnBind();

  // Restore modified GL state
  g_ShaderHangle->UnBind();
  g_FontTexture->UnBind();

  g_VboHandle->UnBind();
  g_ElementsHandle->UnBind();

//  glUseProgram(last_program);
//  glBindTexture(GL_TEXTURE_2D, last_texture);
//#ifdef GL_SAMPLER_BINDING
//  glBindSampler(0, last_sampler);
//#endif
//  glActiveTexture(last_active_texture);
//  glBindVertexArray(last_vertex_array);
  //glBindBuffer(GL_ARRAY_BUFFER, last_array_buffer);

  cb::gl::setState(last_blend_state);
  cb::gl::setState(last_cull_state);
  cb::gl::setState(last_depth_state);

  cb::gl::setScissorTest(false);
  cb::gl::setStateEnabled(cb::gl::State::BLEND, false);
  cb::gl::setStateEnabled(cb::gl::State::CULL_FACE, true);
  cb::gl::setStateEnabled(cb::gl::State::DEPTH_TEST, true);

  cb::gl::setViewport(last_viewport);
  cb::gl::setScissorRect(last_scissor_box);
}

bool ImGui_ImplOpenGL3_CreateFontsTexture()
{
  // Build texture atlas
  ImGuiIO& io = ImGui::GetIO();
  unsigned char* pixels;
  int width, height;
  io.Fonts->GetTexDataAsRGBA32(&pixels, &width, &height);   // Load as RGBA 32-bits (75% of the memory is wasted, but default font is so small) because it is more likely to be compatible with user's existing shaders. If your ImTextureId represent a higher-level concept than just a GL texture id, consider calling GetTexDataAsAlpha8() instead to save on GPU memory.

  g_FontTexture = std::make_unique<cb::gl::CTexture>(glm::uvec2{ width, height }, cb::gl::TextureFormat::RGBA8);
  g_FontTexture->Bind();
  g_FontTexture->SetFilter(cb::gl::TextureFilter::LINEAR, cb::gl::TextureFilter::LINEAR, cb::gl::TextureFilter::LINEAR);
  g_FontTexture->SetDataRaw(cb::gl::InputFormat::RGBA, cb::gl::DataType::UNSIGNED_BYTE, pixels);

  // Upload texture to graphics system
  //GLint last_texture;
  //glGetIntegerv(GL_TEXTURE_BINDING_2D, &last_texture);
  //glGenTextures(1, &g_FontTexture);
  //glBindTexture(GL_TEXTURE_2D, g_FontTexture);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  //glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  //glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
  //glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixels);

  // Store our identifier
  io.Fonts->TexID = (ImTextureID)(intptr_t)g_FontTexture.get();

  g_FontTexture->UnBind();
  // Restore state
  //glBindTexture(GL_TEXTURE_2D, last_texture);

  return true;
}

void ImGui_ImplOpenGL3_DestroyFontsTexture()
{
  if (g_FontTexture)
  {
    ImGuiIO& io = ImGui::GetIO();
    g_FontTexture.reset();
    io.Fonts->TexID = 0;
  }
}

//// If you get an error please report on github. You may try different GL context version or GLSL version. See GL<>GLSL version table at the top of this file.
//static bool CheckShader(GLuint handle, const char* desc)
//{
//    GLint status = 0, log_length = 0;
//    glGetShaderiv(handle, GL_COMPILE_STATUS, &status);
//    glGetShaderiv(handle, GL_INFO_LOG_LENGTH, &log_length);
//    if ((GLboolean)status == GL_FALSE)
//        fprintf(stderr, "ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to compile %s!\n", desc);
//    if (log_length > 0)
//    {
//        ImVector<char> buf;
//        buf.resize((int)(log_length + 1));
//        glGetShaderInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
//        fprintf(stderr, "%s\n", buf.begin());
//    }
//    return (GLboolean)status == GL_TRUE;
//}

// If you get an error please report on GitHub. You may try different GL context version or GLSL version.
//static bool CheckProgram(GLuint handle, const char* desc)
//{
//    GLint status = 0, log_length = 0;
//    glGetProgramiv(handle, GL_LINK_STATUS, &status);
//    glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &log_length);
//    if ((GLboolean)status == GL_FALSE)
//        fprintf(stderr, "ERROR: ImGui_ImplOpenGL3_CreateDeviceObjects: failed to link %s! (with GLSL '%s')\n", desc, g_GlslVersionString);
//    if (log_length > 0)
//    {
//        ImVector<char> buf;
//        buf.resize((int)(log_length + 1));
//        glGetProgramInfoLog(handle, log_length, NULL, (GLchar*)buf.begin());
//        fprintf(stderr, "%s\n", buf.begin());
//    }
//    return (GLboolean)status == GL_TRUE;
//}

bool    ImGui_ImplOpenGL3_CreateDeviceObjects()
{

  // Parse GLSL version string
  int glsl_version = 130;
  sscanf(g_GlslVersionString, "#version %d", &glsl_version);

  const char* vertex_shader_glsl_120 =
    "uniform mat4 ProjMtx;\n"
    "attribute vec2 Position;\n"
    "attribute vec2 UV;\n"
    "attribute vec4 Color;\n"
    "varying vec2 Frag_UV;\n"
    "varying vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    Frag_UV = UV;\n"
    "    Frag_Color = Color;\n"
    "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

  const char* vertex_shader_glsl_130 =
    "uniform mat4 ProjMtx;\n"
    "in vec2 Position;\n"
    "in vec2 UV;\n"
    "in vec4 Color;\n"
    "out vec2 Frag_UV;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    Frag_UV = UV;\n"
    "    Frag_Color = Color;\n"
    "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

  const char* vertex_shader_glsl_300_es =
    "precision mediump float;\n"
    "layout (location = 0) in vec2 Position;\n"
    "layout (location = 1) in vec2 UV;\n"
    "layout (location = 2) in vec4 Color;\n"
    "uniform mat4 ProjMtx;\n"
    "out vec2 Frag_UV;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    Frag_UV = UV;\n"
    "    Frag_Color = Color;\n"
    "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

  const char* vertex_shader_glsl_410_core =
    "layout (location = 0) in vec2 Position;\n"
    "layout (location = 1) in vec2 UV;\n"
    "layout (location = 2) in vec4 Color;\n"
    "uniform mat4 ProjMtx;\n"
    "out vec2 Frag_UV;\n"
    "out vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    Frag_UV = UV;\n"
    "    Frag_Color = Color;\n"
    "    gl_Position = ProjMtx * vec4(Position.xy,0,1);\n"
    "}\n";

  const char* fragment_shader_glsl_120 =
    "#ifdef GL_ES\n"
    "    precision mediump float;\n"
    "#endif\n"
    "uniform sampler2D Texture;\n"
    "varying vec2 Frag_UV;\n"
    "varying vec4 Frag_Color;\n"
    "void main()\n"
    "{\n"
    "    gl_FragColor = Frag_Color * texture2D(Texture, Frag_UV.st);\n"
    "}\n";

  const char* fragment_shader_glsl_130 =
    "uniform sampler2D Texture;\n"
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
    "}\n";

  const char* fragment_shader_glsl_300_es =
    "precision mediump float;\n"
    "uniform sampler2D Texture;\n"
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "layout (location = 0) out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
    "}\n";

  const char* fragment_shader_glsl_410_core =
    "in vec2 Frag_UV;\n"
    "in vec4 Frag_Color;\n"
    "uniform sampler2D Texture;\n"
    "layout (location = 0) out vec4 Out_Color;\n"
    "void main()\n"
    "{\n"
    "    Out_Color = Frag_Color * texture(Texture, Frag_UV.st);\n"
    "}\n";

  // Select shaders matching our GLSL versions
  const char* vertex_shader = NULL;
  const char* fragment_shader = NULL;
  if (glsl_version < 130)
  {
    vertex_shader = vertex_shader_glsl_120;
    fragment_shader = fragment_shader_glsl_120;
  }
  else if (glsl_version >= 410)
  {
    vertex_shader = vertex_shader_glsl_410_core;
    fragment_shader = fragment_shader_glsl_410_core;
  }
  else if (glsl_version == 300)
  {
    vertex_shader = vertex_shader_glsl_300_es;
    fragment_shader = fragment_shader_glsl_300_es;
  }
  else
  {
    vertex_shader = vertex_shader_glsl_130;
    fragment_shader = fragment_shader_glsl_130;
  }

  // Create shaders
  const cb::string vertex_shader_with_version = cb::fromUtf8(cb::utf8string(g_GlslVersionString) + "\n" + cb::utf8string(vertex_shader));
  g_VertHandle = std::make_unique<cb::gl::CShader>(cb::gl::ShaderType::VERTEX);
  g_VertHandle->LoadSource(vertex_shader_with_version);
  if (!g_VertHandle->Compile()) {
    // TODO ERROR
  }

  const cb::string fragment_shader_with_version = cb::fromUtf8(cb::utf8string(g_GlslVersionString) + "\n" + cb::utf8string(fragment_shader));
  g_FragHandle = std::make_unique<cb::gl::CShader>(cb::gl::ShaderType::FRAGMENT);
  g_FragHandle->LoadSource(fragment_shader_with_version);
  g_FragHandle->Compile();

  g_ShaderHangle = std::make_unique<cb::gl::CProgram>();
  g_ShaderHangle->Attach(*g_VertHandle);
  g_ShaderHangle->Attach(*g_FragHandle);

  g_ShaderHangle->Link();

  g_AttribLocationPosition = 0;
  g_AttribLocationUV = 1;
  g_AttribLocationColor = 2;

  g_ShaderHangle->SetInLocation(g_AttribLocationPosition, L"Position");
  g_ShaderHangle->SetInLocation(g_AttribLocationUV, L"UV");
  g_ShaderHangle->SetInLocation(g_AttribLocationColor, L"Color");

  g_AttribLocationProjMtx = g_ShaderHangle->GetUniformId(L"ProjMtx");
  g_AttribLocationTex = g_ShaderHangle->GetUniformId(L"Texture");

  // Create buffers
  g_VboHandle = std::make_unique<cb::gl::CBuffer>(cb::gl::BufferTarget::ARRAY);
  g_ElementsHandle = std::make_unique<cb::gl::CBuffer>(cb::gl::BufferTarget::ELEMENT_ARRAY);

  ImGui_ImplOpenGL3_CreateFontsTexture();

  // Restore modified GL state

  return true;
}

void    ImGui_ImplOpenGL3_DestroyDeviceObjects()
{
  g_VboHandle.reset();
  g_ElementsHandle.reset();
  g_VertHandle.reset();
  g_FragHandle.reset();
  g_ShaderHangle.reset();

  ImGui_ImplOpenGL3_DestroyFontsTexture();
}
