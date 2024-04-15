#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transformable.hpp>
#include <SFML/System/Vector2.hpp>
#include <future>
#include <memory>
#include <utility>

#include "utils/tree.h"
#include "utils/vector2.h"

namespace gui {
class GuiElement : public sf::Drawable, public Tree {
   public:
    virtual ~GuiElement() = default;

    int Id() const;
    int Depth() const;
    utils::Vector2f Position() const;
    utils::Vector2f Size() const;
    virtual sf::Rect<float> Rect() const = 0;

    virtual void Enable();
    virtual void Disable();

    virtual void SetPosition(utils::Vector2f position);
    virtual void Resize(utils::Vector2f size);
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const;

   protected:
    int id_ = max_object_id++;
    bool is_active_ = true;

    utils::Vector2f size_;
    utils::Vector2f position_;
    int depth_ = 0;

   private:
    static std::atomic<int> max_object_id;
};
}  // namespace gui
