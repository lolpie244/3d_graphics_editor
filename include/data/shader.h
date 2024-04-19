#pragma once

#include <GL/glew.h>

#include <SFML/Graphics/Shader.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "mesh.h"

namespace data {
class Shader : public sf::Shader {
   public:
	using sf::Shader::setUniform;

    void setUniform(const std::string& name, const glm::mat4& data) {
		sf::Shader::bind(this);
        unsigned int transformLoc = glGetUniformLocation(getNativeHandle(), name.c_str());
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(data));
    }

    void setUniform(const std::string& name, sf::Color color) {
		this->setUniform(name, (sf::Glsl::Vec4)color);
    }
};
}  // namespace utils
