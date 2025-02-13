#include <SFML/Graphics.hpp>
#include <cmath>
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
#include <iomanip>
#include <filesystem>

// Function to draw a fractal (Mandelbrot set for simplicity)
void drawFractal(sf::RenderWindow& window, double zoom, sf::Vector2<double> center, int maxIterations, float time) {
    sf::VertexArray pixels(sf::Points, window.getSize().x * window.getSize().y);

    for (int x = 0; x < window.getSize().x; ++x) {
        for (int y = 0; y < window.getSize().y; ++y) {
            double zx = 1.5 * (x - window.getSize().x / 2.0) / (0.5 * zoom * window.getSize().x) + center.x;
            double zy = (y - window.getSize().y / 2.0) / (0.5 * zoom * window.getSize().y) + center.y;
            double cx = zx, cy = zy;
            int i;
            for (i = 0; i < maxIterations; ++i) {
                double zx2 = zx * zx, zy2 = zy * zy;
                if (zx2 + zy2 > 4.0) break;
                zy = 2.0 * zx * zy + cy;
                zx = zx2 - zy2 + cx;
            }

            sf::Color color;
            if (i == maxIterations) {
                color = sf::Color::Black; // Inside the set
            }
            else {
                float smoothColor = i + 1 - std::log(std::log(std::sqrt(zx * zx + zy * zy))) / std::log(2); // Smooth coloring
                float hue = static_cast<float>(smoothColor) / maxIterations;

                // Add time-based offset to the hue
                hue += time;
                if (hue > 1.0f) hue -= 1.0f; // Wrap around if hue exceeds 1.0

                // Stylish color gradient
                float r, g, b;
                if (hue < 1.0f / 3.0f) {
                    // Transition from red to green
                    r = std::sin(hue * 3 * 3.14159f) * 255;
                    g = std::pow(std::sin(hue * 3 * 3.14159f + 6.094f), 2) * 255;
                    b = 0;
                }
                else if (hue < 2.0f / 3.0f) {
                    // Transition from green to blue
                    r = 0;
                    g = std::cos((hue - 1.0f / 3.0f) * 3 * 3.14159f) * 255;
                    b = std::pow(std::sin((hue - 1.0f / 3.0f) * 3 * 5.14159f + 2.094f), 2) * 255;
                }
                else {
                    // Transition from blue back to red
                    r = std::pow(std::sin((hue - 2.0f / 3.0f) * 3 * 4.14159f + 2.094f), 2) * 255;
                    g = 0;
                    b = std::cos((hue - 2.0f / 3.0f) * 3 * 2.14159f) * 255;
                }

                color = sf::Color(
                    static_cast<sf::Uint8>(r),
                    static_cast<sf::Uint8>(g),
                    static_cast<sf::Uint8>(b)
                );
            }

            pixels[x + y * window.getSize().x].position = sf::Vector2f(x, y);
            pixels[x + y * window.getSize().x].color = color;
        }
    }

    window.draw(pixels);
}

void recordFrames(sf::RenderWindow& window, double& zoom, sf::Vector2<double>& center, int maxIterations, const sf::Vector2<double>& endCenter, double endZoom) {
    const int framesPerInterpolation = 900; // Exactly 900 frames
    const std::string folderName = "recorded_frames";
    std::filesystem::create_directory(folderName); // Create directory for frames

    sf::Vector2<double> startCenter = center;
    double startZoom = zoom;

    // Logarithmic interpolation for zoom to give a constant perceived speed
    double zoomLogStart = std::log(startZoom);
    double zoomLogEnd = std::log(endZoom);

    for (int j = 0; j < framesPerInterpolation; ++j) {
        float t = static_cast<float>(j) / (framesPerInterpolation - 1); // Adjust t for exact interpolation

        // Linear interpolation for center position
        center.x = startCenter.x + t * (endCenter.x - startCenter.x);
        center.y = startCenter.y + t * (endCenter.y - startCenter.y);

        // Logarithmic interpolation for zoom
        zoom = std::exp(zoomLogStart + t * (zoomLogEnd - zoomLogStart));

        // Calculate time based on frame index for color cycling
        float time = static_cast<float>(j) / framesPerInterpolation;

        window.clear();
        drawFractal(window, zoom, center, maxIterations, time);
        window.display();

        // Capture frame using Texture
        sf::Texture texture;
        texture.create(window.getSize().x, window.getSize().y);
        texture.update(window);
        sf::Image screenshot = texture.copyToImage();

        std::ostringstream filename;
        filename << folderName << "/frame_" << std::setfill('0') << std::setw(5) << j << ".png";
        screenshot.saveToFile(filename.str());
        std::cout << "Frame captured: " << filename.str() << std::endl;
    }
}

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Fractal Zoom");
    window.setFramerateLimit(60);

    double zoom = 1.0;
    //sf::Vector2<double> center(-0.7467745116055939, -0.1071315080796475); //orig zoom postion
    sf::Vector2<double> center(-0.7448109501771761, -0.1071465558960558);
    int maxIterations = 1000; // Increased for better detail at high zoom

    // Define the end point for the zoom
    sf::Vector2<double> endCenter(-0.7448109501771761, -0.1071465558960558);

    double endZoom = 1.264056845e+15;

    // Load a font to display text
    sf::Font font;
    if (!font.loadFromFile("C:/Users/abroadbent/source/repos/BMP_Create/font/arial.ttf")) {
        std::cerr << "Failed to load font" << std::endl;
        return -1;
    }

    // Create text objects for center and zoom values
    sf::Text centerText, zoomText;
    centerText.setFont(font);
    zoomText.setFont(font);
    centerText.setCharacterSize(20);
    zoomText.setCharacterSize(20);
    centerText.setFillColor(sf::Color::White);
    zoomText.setFillColor(sf::Color::White);
    centerText.setPosition(10, 10);
    zoomText.setPosition(10, 40);

    bool showText = true;
    bool recording = false;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();

            if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::T) {
                    showText = !showText;
                }
                if (event.key.code == sf::Keyboard::R) {
                    recording = true;
                }
            }

            if (event.type == sf::Event::MouseWheelScrolled) {
                if (event.mouseWheelScroll.wheel == sf::Mouse::VerticalWheel) {
                    double zoomFactor = 1.1;
                    if (event.mouseWheelScroll.delta > 0)
                        zoom /= zoomFactor;
                    else
                        zoom *= zoomFactor;
                }
            }
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
            center.x -= 0.01 / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
            center.x += 0.01 / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
            center.y -= 0.01 / zoom;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
            center.y += 0.01 / zoom;
        }

        window.clear();
        drawFractal(window, zoom, center, maxIterations, 0.0f);

        if (showText) {
            std::ostringstream centerStream, zoomStream;
            centerStream << std::setprecision(std::numeric_limits<double>::digits10 + 1)
                << "Center: (" << center.x << ", " << center.y << ")";
            zoomStream << "Zoom: " << zoom;
            centerText.setString(centerStream.str());
            zoomText.setString(zoomStream.str());
            window.draw(centerText);
            window.draw(zoomText);
        }

        window.display();

        if (recording) {
            recordFrames(window, zoom, center, maxIterations, endCenter, endZoom);
            recording = false;
        }
    }

    return 0;
}