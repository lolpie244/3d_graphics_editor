#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <memory>
#include <utility>

namespace gui {
class GuiElement {
   public:
    GuiElement();
    virtual ~GuiElement();

    virtual void Enable() = 0;
    virtual void Disable() = 0;

    int Id() const;
    int Depth() const;
    sf::Vector2f Position() const;
    sf::Vector2f Size() const;
    sf::Vector2f LeftCorner() const;

    virtual void Resize(sf::Vector2f size);

   protected:
    int id_ = max_object_id++;
    bool is_active_ = true;

    sf::Sprite sprite_;
	sf::Texture texture_;
    sf::Vector2f size_;
    sf::Vector2f position_;
    int depth_;

   private:
    static int max_object_id;
};
}  // namespace gui
