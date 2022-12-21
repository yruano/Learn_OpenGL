#include "program.h"

ProgramUPtr Program::Create(const std::vector<ShaderPtr>& shaders)
{
  auto program = ProgramUPtr(new Program());
  if (!program->Link(shaders)) { return nullptr; }
  return std::move(program);
}

Program::~Program()
{
  if (m_program)
  {
    // glDeleteProgram : program object 제거
    glDeleteProgram(m_program);
  }
}

bool Program::Link(const std::vector<ShaderPtr>& shaders)
{
  // glCreateProgram : OpenGL program object 생성
  m_program = glCreateProgram();
  // glAttachShader : program에 shader를 붙이기
  for (auto& shader : shaders) { glAttachShader(m_program, shader->Get()); }
  //  glLinkProgram : program 링크
  glLinkProgram(m_program);

  int success = 0;
  // glGetProgramiv : program에 대한 정수형 정보를 얻어옴
  glGetProgramiv(m_program, GL_LINK_STATUS, &success);
  if (!success)
  {
    char infoLog[1024];
    // glGetProgramInfoLog : program에 대한 로그를 얻어옴. 링크 에러를 얻어내는 용도로 사용
    glGetProgramInfoLog(m_program, 1024, nullptr, infoLog);
    SPDLOG_ERROR("failed to link program: {}", infoLog);
    return false;
  }

  return true;
}

void Program::Use() const { glUseProgram(m_program); }

void Program::SetUniform (const std::string& name, int value) const
{
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniform1i(loc, value);
}

void Program::SetUniform (const std::string& name, float value) const
{
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniform1f(loc, value);
}

void Program::SetUniform (const std::string& name, const glm::vec3& value) const
{
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniform3fv(loc, 1, glm::value_ptr(value));
}

void Program::SetUniform (const std::string& name, const glm::mat4& value) const 
{
  auto loc = glGetUniformLocation(m_program, name.c_str());
  glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(value));
}

