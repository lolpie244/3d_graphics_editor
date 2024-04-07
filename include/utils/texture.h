#pragma once

#include <lunasvg.h>

#include <SFML/Graphics/Image.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Vector2.hpp>
#include <cmath>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

namespace utils {

class Texture {
   public:
    virtual ~Texture() = default;
    virtual sf::Texture getTexture(sf::Vector2f size) = 0;
};
class SvgTexture;

class SvgTextureElement : public Texture {
    friend SvgTexture;

   public:
    SvgTextureElement() = delete;

    sf::Texture getTexture(sf::Vector2f size) override {
        sf::Image image;
        sf::Texture result;

        auto element = document_->getElementById(id_);
        auto bitmap_ = element.renderToBitmap(size.x, size.y);
        bitmap_.convertToRGBA();
        image.create(bitmap_.width(), bitmap_.height(), bitmap_.data());
        result.loadFromImage(image);
        return result;
    }

   private:
    SvgTextureElement(const std::string& id, std::shared_ptr<lunasvg::Document> document)
        : id_(id), document_(document) {}

   private:
    std::string id_;
    std::shared_ptr<lunasvg::Document> document_;
};

class SvgTexture : public Texture {
   public:
    static std::shared_ptr<SvgTexture> loadFromFile(const std::string& file) {
        return std::shared_ptr<SvgTexture>(new SvgTexture(file));
    }

    sf::Texture getTexture(sf::Vector2f size) {
        sf::Image image;
        sf::Texture result;

        auto bitmap_ = document_->renderToBitmap(size.x, size.y);
        bitmap_.convertToRGBA();

        image.create(bitmap_.width(), bitmap_.height(), bitmap_.data());
        result.loadFromImage(image);

        return result;
    }

    std::shared_ptr<SvgTextureElement> getElement(const std::string& id) {
        return std::shared_ptr<SvgTextureElement>(new SvgTextureElement(id, document_));
    }

   private:
    SvgTexture(const std::string& file) { document_ = lunasvg::Document::loadFromFile(file); }

    std::shared_ptr<lunasvg::Document> document_;
};

}  // namespace utils
