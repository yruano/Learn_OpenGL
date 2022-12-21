#ifndef __COMMON_H__
#define __COMMON_H__

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

#define CLASS_PTR(klassName)                          \
  class klassName;                                    \
  using klassName##UPtr = std::unique_ptr<klassName>; \
  using klassName##Ptr = std::shared_ptr<klassName>;  \
  using klassName##WPtr = std::weak_ptr<klassName>;

// CLASS_PTR(Shader); 선언을 하면 밑으로 4줄이 자동으로 생성되는 메트로
// class klassName; \ 
// using klassName ## UPtr = std::unique_ptr<klassName>; \ klassName이 Shader이 바뀌고 ShaderUPtr이 됨 이걸 위해서 ##을 사용함
// using klassName ## Ptr = std::shared_ptr<klassName>; \ 
// using klassName ## WPtr = std::weak_ptr<klassName>;

std::optional<std::string> LoadTextFile(const std::string &filename);

#endif //  __COMMON_H__