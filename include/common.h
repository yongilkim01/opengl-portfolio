#ifndef __COMMON_H__
#define __COMMON_H__

#include <memory>
#include <string>
#include <optional>
#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <spdlog/spdlog.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#define CLASS_PTR(class_name) \
class class_name; \
using class_name ## UniquePtr = std::unique_ptr<class_name>; \
using class_name ## SharedPtr = std::shared_ptr<class_name>; \
using class_name ## WeakPtr = std::weak_ptr<class_name>;

std::optional<std::string> load_textfile(const std::string& file_name);

#endif // __COMMON_H__ 