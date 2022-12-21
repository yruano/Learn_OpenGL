#include "shader.h"

ShaderUPtr Shader::CreateFromFile(const std::string &filename, GLenum shaderType)
{
  auto shader = ShaderUPtr(new Shader());
  if (!shader->LoadFile(filename, shaderType))
  {
    return nullptr;
  }
  return std::move(shader);
}

Shader::~Shader()
{
  if (m_shader)
  {
    // glDeleteShader : shader object제거
    glDeleteShader(m_shader);
  }
}

bool Shader::LoadFile(const std::string &filename, GLenum shaderType)
{
  auto result = LoadTextFile(filename);
  if (!result.has_value())
  {
    return false;
  }

  auto &code = result.value();
  const char *codePtr = code.c_str();
  int32_t codeLength = (int32_t)code.length();

  // create and compile shader
  // glCreateShader : OpenGL shader object 생성
  // glShaderSource : shader에 소스 코드 설정
  // glCompileShader : shader 컴파일
  m_shader = glCreateShader(shaderType);
  glShaderSource(m_shader, 1, (const GLchar *const *)&codePtr, &codeLength);
  glCompileShader(m_shader);

  // check compile error
  int success = 0;
  // glGetShaderiv : shader에 대한 정수형 정보를 얻어옴
  glGetShaderiv(m_shader, GL_COMPILE_STATUS, &success);
  if (!success)
  {
    char infoLog[1024];
    // glGetShaderInfoLog : shader에 대한 로그를 얻어옴. 컴파일 에러 얻어내는 용도로 사용
    glGetShaderInfoLog(m_shader, 1024, nullptr, infoLog);
    SPDLOG_ERROR("failed to compile shader : \"{}\"", filename);
    SPDLOG_ERROR("reason : {}", infoLog);
    return false;
  }

  return true;
}