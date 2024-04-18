#include "test_stage1.h"

#include <GL/glew.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "data/texture.h"
#include "render/mesh.h"
#include "render/renderer.h"
#include "stage/stage.h"
#include "stage/stage_manager.h"

TestStage1::TestStage1() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

    auto theme = data::SvgTexture::loadFromFile("resources/theme.svg");
    shader.loadFromFile("shaders/texture_shader.vert", "shaders/texture_shader.frag");
    texture = theme->getElement("g587")->getTexture({280, 420});

    std::vector<render::Mesh::Vertex> vertices{
        {{-0.5, 0.0, 0}, {0, 0}}, {{-0.5, 0.5, 0}, {1, 0}}, {{0.0, 0.5, 0}, {1, 1}}, {{0.0, 0.0, 0}, {0, 1}}};

    std::vector<unsigned int> indices{0, 1, 2, 2, 3, 0};
    mesh1 = std::make_shared<render::Mesh>(vertices, indices);
    mesh2 = std::make_shared<render::Mesh>(vertices, indices);

    mesh2->SetOrigin(-0.25, 0.25);
    mesh2->Move(0.2, -0.2, -0.3f);
    mesh2->Scale(0.2, 0.2);

    events.push_back(observer_.Bind(sf::Event::MouseMoved, [this](sf::Event event) {
        if (!sf::Mouse::isButtonPressed(sf::Mouse::Left))
            return false;

        math::Vector2f new_pos_ = {(event.mouseMove.x - stage::StageManager::Instance().windowSize().x / 2.0f) /
                                       stage::StageManager::Instance().windowSize().x,
                                   (-event.mouseMove.y + stage::StageManager::Instance().windowSize().y / 2.0f) /
                                       stage::StageManager::Instance().windowSize().y};

        if (old_pos_ == math::Vector2f{0, 0}) {
            old_pos_ = new_pos_;
            return true;
        }

        auto move = (new_pos_ - old_pos_);

        auto& camera = stage::StageManager::Instance().Camera();

        auto clip = camera->ProjectionMatrix() * camera->GetTransformation() * glm::vec4(move.x, move.y, 0, 1);
        stage::StageManager::Instance().Camera()->Move(move.x, move.y);

        old_pos_ = new_pos_;
        return true;
    }));
}

void TestStage1::Run() {
    PollEvents();

    mesh2->Rotate(1, math::Transform::Z);

    shader.setUniform("u_Texture", texture);
    sf::Texture::bind(&texture);
    render::GL_render::Instance().Draw(*mesh1, shader);
    render::GL_render::Instance().Draw(*mesh2, shader);
}
