#include "raylib.h"
#include <cstdlib>      // For rand()
#include <ctime>        // For time()
#include <vector>       // For dynamic asteroids

// Constants
const int screenWidth = 800;
const int screenHeight = 600;
const int maxAsteroids = 5;
const int asteroidSpeed = 4;
const int playerSpeed = 5;

// Structs for the player and asteroids
struct Player {
    Vector2 position;
    float radius;
    Color color;
};

struct Asteroid {
    Vector2 position;
    float radius;
    Color color;
    Vector2 speed;
};

// Helper function to reset an asteroid's position
void ResetAsteroid(Asteroid &asteroid) {
    asteroid.position.x = screenWidth + rand() % 200;
    asteroid.position.y = rand() % screenHeight;
    asteroid.speed.x = -(asteroidSpeed + rand() % 3);
    asteroid.speed.y = (rand() % 5) - 2;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Asteroid Avoider");
    SetTargetFPS(60);
    srand(time(0));

    // Initialize player
    Player player = { { 50, screenHeight / 2 }, 20, SKYBLUE };

    // Initialize asteroids
    std::vector<Asteroid> asteroids(maxAsteroids);
    for (int i = 0; i < maxAsteroids; i++) {
        asteroids[i] = { { 0, 0 }, 30.0f, DARKGRAY };
        ResetAsteroid(asteroids[i]);
    }

    int score = 0;
    bool gameOver = false;

    // Main game loop
    while (!WindowShouldClose()) {
        if (!gameOver) {
            // Player movement
            if (IsKeyDown(KEY_W) && player.position.y > player.radius) player.position.y -= playerSpeed;
            if (IsKeyDown(KEY_S) && player.position.y < screenHeight - player.radius) player.position.y += playerSpeed;

            // Update asteroids
            for (auto &asteroid : asteroids) {
                asteroid.position.x += asteroid.speed.x;
                asteroid.position.y += asteroid.speed.y;

                // Wrap asteroid vertically
                if (asteroid.position.y > screenHeight || asteroid.position.y < 0) {
                    asteroid.speed.y = -asteroid.speed.y;
                }

                // Reset position if off-screen
                if (asteroid.position.x < -asteroid.radius) {
                    score++;
                    ResetAsteroid(asteroid);
                }

                // Collision detection
                if (CheckCollisionCircles(player.position, player.radius, asteroid.position, asteroid.radius)) {
                    gameOver = true;
                }
            }
        } else {
            // Restart on Enter key
            if (IsKeyPressed(KEY_ENTER)) {
                gameOver = false;
                score = 0;
                player.position = { 50, screenHeight / 2 };
                for (auto &asteroid : asteroids) {
                    ResetAsteroid(asteroid);
                }
            }
        }

        // Draw
        BeginDrawing();
        ClearBackground(BLACK);

        if (!gameOver) {
            // Draw player
            DrawCircleV(player.position, player.radius, player.color);

            // Draw asteroids
            for (const auto &asteroid : asteroids) {
                DrawCircleV(asteroid.position, asteroid.radius, asteroid.color);
            }

            // Draw score
            DrawText(TextFormat("Score: %d", score), 10, 10, 20, RAYWHITE);
        } else {
            // Game over screen
            DrawText("GAME OVER!", screenWidth / 2 - 100, screenHeight / 2 - 50, 30, RED);
            DrawText(TextFormat("Final Score: %d", score), screenWidth / 2 - 80, screenHeight / 2, 20, RAYWHITE);
            DrawText("Press ENTER to Restart", screenWidth / 2 - 120, screenHeight / 2 + 50, 20, GRAY);
        }

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
