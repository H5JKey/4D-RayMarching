#include <SFML/Graphics.hpp>

#include "Render.h"
#include "User.h"


#include "lib/imgui/imgui.h"
#include "lib/imgui/imgui-SFML.h"
#include "lib/imgui/imgui_stdlib.h"


void normalize(sf::Vector3f& vec) {
    float len = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);
    vec = { vec.x / len,vec.y / len,vec.z / len };
}

int main()
{
    Render render;
    User user;
    user.r = 10;
    user.pos = sf::Vector3f(1, 1, 1)*user.r;
    user.lookAt = sf::Vector3f(0, 0, 0);

    sf::Clock clk, timer;
    clk.restart();
    timer.restart();



    Object4D obj = Object4D();
    obj.color = sf::Vector3f(0, 1, 0);

    sf::RenderWindow app(sf::VideoMode(), "4D", sf::Style::Fullscreen);
    ImGui::SFML::Init(app);
    bool rotationXY=0, rotationXZ=0, rotationXW=0, rotationYZ=0, rotationYW=0, rotationZW=0;
    while (app.isOpen()) {
        sf::Event event;
        ImGui::SFML::ProcessEvent(event);
        while (app.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                app.close();
            if (!ImGui::GetIO().WantCaptureMouse) {
                if (event.type == sf::Event::MouseWheelMoved) {
                    if (event.mouseWheel.delta > 0)
                        user.r += 0.7;
                    else if (event.mouseWheel.delta < 0)
                        user.r -= 0.7;
                    normalize(user.pos);
                    user.pos = user.pos * user.r;
                }
            }
        }
        if (!ImGui::GetIO().WantCaptureKeyboard) {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
                float z = (user.pos.x - user.lookAt.x) * sin(0.002) + (user.pos.z - user.lookAt.z) * cos(0.002) + user.lookAt.z;
                float x = (user.pos.x - user.lookAt.x) * cos(0.002) - (user.pos.z - user.lookAt.z) * sin(0.002) + user.lookAt.x;
                user.pos.x = x;
                user.pos.z = z;
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::A)) {
                float z = (user.pos.x - user.lookAt.x) * sin(-0.002) + (user.pos.z - user.lookAt.z) * cos(-0.002) + user.lookAt.z;
                float x = (user.pos.x - user.lookAt.x) * cos(-0.002) - (user.pos.z - user.lookAt.z) * sin(-0.002) + user.lookAt.x;
                user.pos.x = x;
                user.pos.z = z;
            }
        }

        app.clear();
        sf::Time dt = clk.restart();
        render.drawScene(obj, app, user, timer.getElapsedTime());
        
        ImGui::SFML::Update(app, dt);
        ImGui::Begin("Menu", nullptr, ImGuiWindowFlags_NoMove + ImGuiWindowFlags_NoResize);
        {
            ImGui::Text("ID:"); {
                if (ImGui::Button("HyperCube"))
                    obj.id = 0;
                ImGui::SameLine();
                if (ImGui::Button("HyperSphere"))
                    obj.id = 1;
                ImGui::SameLine();
                if (ImGui::Button("DuoCylinder"))
                    obj.id = 2;
            }
            ImGui::Text("Size:"); {
                if (obj.id == 1 || obj.id == 0)
                    ImGui::SliderFloat("Size", &obj.size.x,0,100);
                else {
                    ImGui::SliderFloat("Size1", &obj.size.x,0,100);
                    ImGui::SliderFloat("Size2", &obj.size.y,0,100);
                }
            }
            ImGui::Text("Position:"); {
                ImGui::SliderFloat("X:", &obj.x, -10, 10);
                ImGui::SliderFloat("Y:", &obj.y, -10, 10);
                ImGui::SliderFloat("Z:", &obj.z, -10, 10);
                ImGui::SliderFloat("W:", &obj.w, -10, 10);
            }
            ImGui::Text("Rotation:"); {
                ImGui::SliderFloat("XY:", &obj.xy, -180, 180);
                ImGui::SameLine();
                ImGui::Checkbox("##rotateXY",&rotationXY);
                ImGui::SliderFloat("XZ:", &obj.xz, -180, 180);
                ImGui::SameLine();
                ImGui::Checkbox("##rotateXZ", &rotationXZ);
                ImGui::SliderFloat("XW:", &obj.xw, -180, 180);
                ImGui::SameLine();
                ImGui::Checkbox("##rotateXW", &rotationXW);
                ImGui::SliderFloat("YZ:", &obj.yz, -180, 180);
                ImGui::SameLine();
                ImGui::Checkbox("##rotateYZ", &rotationYZ);
                ImGui::SliderFloat("YW:", &obj.yw, -180, 180);
                ImGui::SameLine();
                ImGui::Checkbox("##rotateYW", &rotationYW);
                ImGui::SliderFloat("WZ:", &obj.zw, -180, 180);
                ImGui::SameLine();
                ImGui::Checkbox("##rotateZW", &rotationZW);
            }
            ImGui::Text("Color:"); {
                float* colorArray[3] = { &obj.color.x,&obj.color.y ,&obj.color.z };
                ImGui::ColorPicker3("", *colorArray);
            }
        }
        ImGui::End();
        if (rotationXY)
            obj.xy = timer.getElapsedTime().asSeconds();
        if (rotationXZ)
            obj.xz = timer.getElapsedTime().asSeconds();
        if (rotationXW)
            obj.xw = timer.getElapsedTime().asSeconds();
        if (rotationYZ)
            obj.yz = timer.getElapsedTime().asSeconds();
        if (rotationYW)
            obj.yw = timer.getElapsedTime().asSeconds();
        if (rotationZW)
            obj.zw = timer.getElapsedTime().asSeconds();

        ImGui::SFML::Render(app);
        app.display();

    }
    ImGui::SFML::Shutdown();
}
