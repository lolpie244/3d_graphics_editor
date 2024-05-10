#pragma once

#include <lunasvg.h>

#include <SFML/Graphics/Texture.hpp>
#include <glm/ext/vector_float2.hpp>
#include <memory>
#include <string>



namespace data {
class mTextureType {
   public:
    virtual ~mTextureType() = default;
    sf::Texture getTexture(glm::vec2 size) {
        if (size == old_size_)
            return texture_;
        texture_ = this->loadTexture(size);
        old_size_ = size;

        return texture_;
    }

   protected:
    virtual sf::Texture loadTexture(glm::vec2 size) = 0;

   private:
    glm::vec2 old_size_ = {-1, -1};
    sf::Texture texture_;
};

typedef std::shared_ptr<mTextureType> Texture;
class SvgTexture;

class SvgTextureElement : public mTextureType {
    friend SvgTexture;

   public:
    SvgTextureElement() = delete;

    sf::Texture loadTexture(glm::vec2 size) override {
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

class SvgTexture : public mTextureType {
   public:
    static std::shared_ptr<SvgTexture> loadFromFile(const std::string& file) {
        return std::shared_ptr<SvgTexture>(new SvgTexture(file));
    }

    sf::Texture loadTexture(glm::vec2 size) {
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

class PngTexture : public mTextureType {
   public:
    static std::shared_ptr<PngTexture> loadFromFile(const std::string& file) {
        return std::shared_ptr<PngTexture>(new PngTexture(file));
    }

   protected:
    PngTexture(const std::string& filename) { texture_.loadFromFile(filename); }
    virtual sf::Texture loadTexture(glm::vec2 size) override { return texture_; };

   protected:
    sf::Texture texture_;
};

}  // namespace data
