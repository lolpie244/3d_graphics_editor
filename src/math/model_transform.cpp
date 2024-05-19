#include <glm/ext/quaternion_float.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtx/transform.hpp>
#include "math/transform.h"

namespace math {
const glm::mat4 LocalModelTransform::GetTransformation() const {
    if (changed_) {
        changed_ = false;
        final_matrix_ = glm::translate(glm::mat4(1.0f), position_ + origin_);
        final_matrix_ = glm::rotate(final_matrix_, glm::radians(rotation_.x), {1, 0, 0});
        final_matrix_ = glm::rotate(final_matrix_, glm::radians(rotation_.y), {0, 1, 0});
        final_matrix_ = glm::rotate(final_matrix_, glm::radians(rotation_.z), {0, 0, 1});
        final_matrix_ = glm::scale(final_matrix_, scale_);
        final_matrix_ = glm::translate(final_matrix_, -origin_);
    }
    return final_matrix_;
}

const glm::mat4 GlobalModelTransform::GetTransformation(glm::vec3 origin) const {
    if (changed_) {
        changed_ = false;
        final_matrix_ = glm::translate(glm::mat4(1.0f), position_);

		auto rotation_matrix = glm::translate(glm::mat4(1.0f), origin);
		rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_.y), {0, 1, 0});
        rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_.x), {1, 0, 0});
        rotation_matrix = glm::rotate(rotation_matrix, glm::radians(rotation_.z), {0, 0, 1});
		rotation_matrix = glm::translate(rotation_matrix, -origin);

		final_matrix_ = glm::inverse(glm::inverse(final_matrix_) * rotation_matrix);
        final_matrix_ = glm::scale(final_matrix_, scale_);
    }
    return final_matrix_;
}

const glm::mat4 ModelTransform::GetTransformation() const {
	return GlobalTransform.GetTransformation(this->GetPosition()) * LocalModelTransform::GetTransformation();
}

}  // namespace math
