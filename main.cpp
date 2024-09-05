#include <SFML/Graphics.hpp>
#include <random>
#include "events.hpp"
#include "configuration.hpp"
#include "star.hpp"

std::vector<Star> createStars(uint32_t count)
{
    std::vector<Star> stars;
    stars.reserve(count);

    //Random numbers generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // Create randomly distributed stars on the screen
    for (uint32_t i{ count }; i--;)
    {
        float const x = (dis(gen) - 0.5f) * conf::window_size_f.x;
        float const y = (dis(gen) - 0.5f) * conf::window_size_f.y;
        float const z = dis(gen) * (conf::far - conf::near) + conf::near;

        stars.push_back({ {x, y}, z });
    }

    // Depth ordering
    std::sort(stars.begin(), stars.end(), [](Star const& s_1, Star const& s_2) {
        return s_1.z > s_2.z;
    });

    return stars;
}

int main()
{
    auto window = sf::RenderWindow{ { conf::window_size.x, conf::window_size.y }, "CMake SFML Project", sf::Style::Fullscreen };
    window.setFramerateLimit(conf::max_framerate);

    std::vector<Star> stars = createStars(conf::count);

    uint32_t first = 0;
    while (window.isOpen())
    {
        processEvents(window);

        // Fake travel toward increasing Z
        for (uint32_t i{0}; i < conf::count; ++i)
        {
            Star& s = stars[i];
            s.z -= conf::dt * conf::speed;
            if (s.z < conf::near)
            {
                // Offset the star by the excess travel it made behind near to keep spacing constant
                s.z = conf::far - (conf::near - s.z);
                // This star is now the first we need to draw because it is  the further away from us
                first = i;
            }
        }


        window.clear();

        // Rendering occurs here
        
        sf::CircleShape shape{ conf::radius };
        shape.setOrigin(conf::radius, conf::radius);
        for (uint32_t i{ 0 }; i < conf::count; ++i)
        {
            uint32_t const idx = (i + first) % conf::count;
            Star const& s = stars[idx];
            float const scale = 1.0f / s.z;
            shape.setPosition(s.position * scale + conf::window_size_f * 0.5f);
            shape.setScale(scale, scale);
            float const depth_ratio = (s.z - conf::near) / (conf::far - conf::near);
            float const color_ratio = 1.0f - depth_ratio;
            auto const c = static_cast<uint8_t>(color_ratio * 255.0f);
            shape.setFillColor({ c,c,c });

            window.draw(shape);
        }

        window.display();
    }
}