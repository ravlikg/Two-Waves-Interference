#include <SFML/Graphics.hpp>
#include <imgui.h>
#include <imgui-SFML.h>

int main() {
    constexpr sf::Vector2u windowSize{1920, 1080};
    sf::ContextSettings settings;
    settings.antiAliasingLevel = 8;
    sf::RenderWindow window(sf::VideoMode{windowSize}, "Wave Interference", sf::Style::Default, sf::State::Fullscreen, settings);
    window.setFramerateLimit(60);

    if (!ImGui::SFML::Init(window))
        return -1;
    ImGui::GetIO().IniFilename = nullptr;

    sf::Texture texture(windowSize);
    sf::Sprite sprite(texture);

    sf::Shader shader;
    if (!shader.loadFromFile("wave.frag", sf::Shader::Type::Fragment)) {
        return -1;
    }

    // Амплітуда, частота, фаза
    float amp1 = 30.0f, freq1 = 0.12f, phase1 = 0.0f;
    float amp2 = 30.0f, freq2 = 0.12f, phase2 = 0.0f;
    float distance = 0.3f; // Расстояние между источниками

    int currentTheme = 0;
    const char* themes[] = { "Deep Ocean", "Cinematic Monochrome", "Cyberpunk Magma", "Thermal Plasma" };

    bool isPaused = false;

    float t = 0.0f;
    sf::Clock deltaClock;

    while (window.isOpen()) {
        while (const auto event = window.pollEvent()) {
            ImGui::SFML::ProcessEvent(window, *event);
            if (event->is<sf::Event::Closed>())
                window.close();

            if (const auto* keyPressed = event->getIf<sf::Event::KeyPressed>()) {
                if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
                    window.close();
            }
        }

        if (!isPaused) {
            t += 0.15f;
        }

        ImGui::SFML::Update(window, deltaClock.restart());

        ImGui::SetNextWindowPos(ImVec2(20.0f, 20.0f), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(400.0f, 335.0f), ImGuiCond_FirstUseEver);

        ImGui::Begin("Wave Interference");

        ImGui::Text("Global Settings:");
        ImGui::SliderFloat("Source Distance", &distance, 0.05f, 0.45f);
        ImGui::Combo("Color Theme", &currentTheme, themes, IM_ARRAYSIZE(themes));

        ImGui::Dummy(ImVec2(0.0f, 5.0f));

        if (isPaused) {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.15f, 0.6f, 0.15f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.2f, 0.75f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.1f, 0.5f, 0.1f, 1.0f));
        } else {
            ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0.6f, 0.15f, 0.15f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonHovered, ImVec4(0.75f, 0.2f, 0.2f, 1.0f));
            ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0.5f, 0.1f, 0.1f, 1.0f));
        }

        const char* buttonText = isPaused ? "[ > ] Run Simulation" : "[ || ] Pause Simulation";

        if (ImGui::Button(buttonText, ImVec2(-1.0f, 35.0f))) {
            isPaused = !isPaused;
        }

        ImGui::PopStyleColor(3);

        ImGui::Dummy(ImVec2(0.0f, 5.0f));
        ImGui::Separator();

        ImGui::TextColored(ImVec4(0.3f, 0.6f, 1.0f, 1.0f), "Left Wave:");
        ImGui::SliderFloat("Amplitude 1", &amp1, 0.0f, 100.0f);
        ImGui::SliderFloat("Frequency 1", &freq1, 0.01f, 0.5f);
        ImGui::SliderFloat("Phase 1", &phase1, 0.0f, 6.28f);

        ImGui::Separator();

        ImGui::TextColored(ImVec4(1.0f, 0.4f, 0.4f, 1.0f), "Right Wave:");
        ImGui::SliderFloat("Amplitude 2", &amp2, 0.0f, 100.0f);
        ImGui::SliderFloat("Frequency 2", &freq2, 0.01f, 0.5f);
        ImGui::SliderFloat("Phase 2", &phase2, 0.0f, 6.28f);

        ImGui::End();

        // Координаты источников волн
        sf::Vector2f source1(static_cast<float>(windowSize.x) * distance, static_cast<float>(windowSize.y) * 0.5f);
        sf::Vector2f source2(static_cast<float>(windowSize.x) * (1.0f - distance), static_cast<float>(windowSize.y) * 0.5f);

        // Передача параметров в видеокарту
        shader.setUniform("t", t);
        shader.setUniform("amp1", amp1);
        shader.setUniform("freq1", freq1);
        shader.setUniform("phase1", phase1);
        shader.setUniform("amp2", amp2);
        shader.setUniform("freq2", freq2);
        shader.setUniform("phase2", phase2);
        shader.setUniform("source1", source1);
        shader.setUniform("source2", source2);
        shader.setUniform("theme", currentTheme);

        window.clear();
        window.draw(sprite, &shader);
        ImGui::SFML::Render(window);
        window.display();
    }

    ImGui::SFML::Shutdown();
    return 0;
}