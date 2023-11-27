#include <SFML/Graphics.hpp>

class Paddle {
public:
    sf::RectangleShape shape;
    float speed;

    Paddle(float x, float y) : speed(300.0f) {
        shape.setSize(sf::Vector2f(20, 100));
        shape.setPosition(x, y);
    }

    void move(float dt, int direction) {
        shape.move(0, direction * speed * dt);
    }
};

class Ball {
public:
    sf::CircleShape shape;
    sf::Vector2f velocity;

    Ball(float x, float y) : velocity(200.0f, 200.0f) {
        shape.setRadius(15);
        shape.setPosition(x, y);
    }

    void move(float dt) {
        shape.move(velocity * dt);
    }
};

int main() {
    sf::RenderWindow window(sf::VideoMode(800, 600), "Ping Pong Game");

    Paddle playerPaddle(50, 250);
    Paddle botPaddle(730, 250);
    Ball ball(390, 290);

    sf::Clock clock;

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Calculate the elapsed time since the last frame
        float dt = clock.restart().asSeconds();

        // Move the player paddle based on keyboard input
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::W) && playerPaddle.shape.getPosition().y > 0) {
            playerPaddle.move(dt, -1);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::S) && playerPaddle.shape.getPosition().y + playerPaddle.shape.getSize().y < window.getSize().y) {
            playerPaddle.move(dt, 1);
        }

        // Move the bot paddle to follow the ball
        if (ball.shape.getPosition().y + ball.shape.getRadius() < botPaddle.shape.getPosition().y + botPaddle.shape.getSize().y / 2 && botPaddle.shape.getPosition().y > 0) {
            botPaddle.move(dt, -1);
        }
        if (ball.shape.getPosition().y + ball.shape.getRadius() > botPaddle.shape.getPosition().y + botPaddle.shape.getSize().y / 2 && botPaddle.shape.getPosition().y + botPaddle.shape.getSize().y < window.getSize().y) {
            botPaddle.move(dt, 1);
        }

        // Move the ball
        ball.move(dt);

        // Check collisions with window boundaries
        if (ball.shape.getPosition().y < 0 || ball.shape.getPosition().y + ball.shape.getRadius() * 2 > window.getSize().y) {
            ball.velocity.y = -ball.velocity.y;
        }

        // Check collisions with paddles
        if (ball.shape.getGlobalBounds().intersects(playerPaddle.shape.getGlobalBounds()) || ball.shape.getGlobalBounds().intersects(botPaddle.shape.getGlobalBounds())) {
            ball.velocity.x = -ball.velocity.x;
        }

        // Check if ball is out of bounds (score point)
        if (ball.shape.getPosition().x < 0 || ball.shape.getPosition().x + ball.shape.getRadius() * 2 > window.getSize().x) {
            // Reset ball position
            ball.shape.setPosition(390, 290);

            // Invert the horizontal velocity
            ball.velocity.x = -ball.velocity.x;
        }

        // Clear the window
        window.clear();

        // Draw paddles and ball
        window.draw(playerPaddle.shape);
        window.draw(botPaddle.shape);
        window.draw(ball.shape);

        // Display the contents of the window
        window.display();
    }

    return 0;
}
