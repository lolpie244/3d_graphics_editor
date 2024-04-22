#include "test_stage1.h"

#include <GL/glew.h>

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Shader.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/Graphics/Transform.hpp>
#include <SFML/Window/Event.hpp>
#include <SFML/Window/Keyboard.hpp>
#include <SFML/Window/Mouse.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <memory>

#include "data/mesh.h"
#include "data/texture.h"
#include "events/propterties/scaleable.h"
#include "gui/opengl_context.h"
#include "gui/sprite_element.h"
#include "math/points_cast.h"
#include "math/transform.h"
#include "math/vector2.h"
#include "render/mesh.h"
#include "render/opengl/frame_buffer.h"
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
    point_shader.loadFromFile("shaders/texture_shader.vert", "shaders/point_shader.frag");
    picking_shader.loadFromFile("shaders/texture_shader.vert", "shaders/picking.frag");

    texture = theme->getElement("g546")->getTexture({400, 400});

    background = std::make_unique<gui::SpriteGuiElement>(glm::vec3{600, 600, -1}, math::Vector2f{1200, 1200});
    background->SetTexture(theme->getElement("g71"));

    opengl_context = std::make_unique<gui::OpenglContext>();
    opengl_context->SetLeftCorner(50, 0);
    opengl_context->Resize(math::Vector2f(1700, 1080));
    opengl_context->SetScaleMethod<events::DefaultScale>();
    opengl_context->BindScale(observer_);

    camera_->SetOrigin(0, 0, 0);

    opengl_context->BindDrag(observer_, [this](sf::Event event, math::Vector2f moved) {
        auto move = math::to_ndc(stage::StageManager::Instance().windowSize() / 2.0f + moved);
        if (sf::Mouse::isButtonPressed(sf::Mouse::Right)) {
            stage::StageManager::Instance().Camera()->Move(-move.x, move.y);
            return true;
        }
        if (sf::Mouse::isButtonPressed(sf::Mouse::Middle)) {
            stage::StageManager::Instance().Camera()->Rotate(moved.x * 0.5, math::Transform::Y);
            stage::StageManager::Instance().Camera()->Rotate(-moved.y * 0.5, math::Transform::X);
            return true;
        }

        return false;
    });

    opengl_context->BindScroll(observer_, [this](sf::Event event) {
        stage::StageManager::Instance().Camera()->Move(0, 0, -event.mouseWheelScroll.delta);
        return true;
    });

    camera_->Move(0, 0, 3.0f);
    camera_->Rotate(-40, math::Transform::X);
    ///////////////////////////////////////////
    mesh = data::Mesh::loadFromFile("resources/cube.obj");

    mesh->Scale(0.5, 0.5, 0.5);
    mesh->texture = data::PngTexture::loadFromFile("resources/cube.png")->getTexture({0, 0});

    opengl_context->BindRelease(observer_, [this](sf::Event event) {
        if (event.mouseButton.button != sf::Mouse::Left)
            return false;

        auto info = picking.ReadPixel(event.mouseButton.x, 800 - event.mouseButton.y);
        return true;
    });
}

void TestStage1::Run() {
    PollEvents();
    // window_->pushGLStates();
    // window_->draw(*background);
    // window_->popGLStates();

    picking.buffer_.Bind();
    picking.buffer_.Bind(render::FrameBuffer::Write);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	picking_shader.setUniform("u_ObjectIndex", (unsigned int)2);
	render::GL_render::Instance().Draw(*mesh, picking_shader);
	
    glReadBuffer(GL_COLOR_ATTACHMENT0);
    constexpr std::array<GLenum, 2> attachments { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
    glDrawBuffers(attachments.size(), attachments.data());

    picking.buffer_.Unbind(render::FrameBuffer::Write);
    glBlitFramebuffer(
        0, 0, 800, 800,
        0, 0, 800, 800,
        GL_COLOR_BUFFER_BIT, GL_NEAREST);

    picking.buffer_.Unbind(render::FrameBuffer::Default);

    FrameEnd();
}
