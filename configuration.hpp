#pragma once

namespace conf
{
// Window configuration
	sf::Vector2u const window_size = { 1920, 1080 };
	sf::Vector2f const window_size_f = static_cast<sf::Vector2f>(window_size);
	uint32_t const max_framerate = 144;
	float const dt = 1.0f / static_cast<float>(max_framerate);

// Star configuration
	uint32_t const count = 50000;
	float const radius = 40.0f;
	float const far = 10.0f;
	float const near = 0.1f;
	float const speed = 1.0f;
}
