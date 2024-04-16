#include "test_stage1.h"

#include <GL/glew.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <memory>

#include "gui/elements/buttons_list.h"
#include "renderer/mesh.h"
#include "renderer/renderer.h"
#include "stage/stage.h"
#include "utils/vector2.h"

TestStage1::TestStage1() {
    events.push_back(observer_.Bind(sf::Event::KeyPressed, [this](sf::Event event) {
        if (event.key.code == sf::Keyboard::Escape) {
            this->Stop(stage::StageState::Exit);
            return true;
        }
        return false;
    }));

    auto theme = utils::SvgTexture::loadFromFile("resources/theme.svg");
    shader.loadFromFile("shaders/texture_shader.vert", "shaders/texture_shader.frag");
    texture = theme->getElement("g586")->getTexture({300, 300});

    std::vector<renderer::Mesh::Vertex> vertices{
        {{-0.5, -0.5, 0}, {0, 0}}, {{0.5, -0.5, 0}, {1, 0}}, {{0.5, 0.5, 0}, {1, 1}}, {{-0.5, 0.5, 0}, {0, 1}}};

    std::vector<unsigned int> indices{0, 1, 2, 2, 3, 0};
    mesh = std::make_shared<renderer::Mesh>(vertices, indices);
}

void TestStage1::Run() {
    PollEvents();
    shader.setUniform("u_Texture", texture);
    sf::Texture::bind(&texture);
    renderer::GL_Renderer::Instance().Draw(*mesh, shader);
}
