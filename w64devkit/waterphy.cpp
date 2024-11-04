#include "raylib.h"
#include <vector>
#include <cmath>
#include "include/raymath.h"

// Constants
const int screenWidth = 800;
const int screenHeight = 600;
const int particleCount = 400; // Number of particles in the water
const int glassWidth = 400;
const int glassHeight = 300;
const int particleRadius = 4;
const float gravity = 0.5f;
const float drag = 0.99f;
const float interactionRadius = 15.0f;

// Particle structure
struct Particle {
    Vector2 position;
    Vector2 velocity;
    Color color;
};

// Initialize particles (half-full tub)
std::vector<Particle> InitializeParticles() {
    std::vector<Particle> particles;
    int rows = glassHeight / (particleRadius * 2);
    int cols = glassWidth / (particleRadius * 2);

    // Fill the lower half of the glass with particles
    for (int i = 0; i < rows / 2; i++) { // Only fill the lower half
        for (int j = 0; j < cols; j++) {
            Particle p;
            p.position = { 
                static_cast<float>(screenWidth) / 2 - static_cast<float>(glassWidth) / 2 + 
                static_cast<float>(j) * particleRadius * 2 + static_cast<float>(particleRadius),
                static_cast<float>(screenHeight) / 2 + static_cast<float>(i) * particleRadius * 2 
            };
            p.velocity = { 0, 0 };
            p.color = BLUE; // Water color
            particles.push_back(p);
        }
    }

    return particles;
}

// Apply forces to the particles
void ApplyForces(std::vector<Particle>& particles, Vector2 mousePos) {
    for (auto &particle : particles) {
        // Apply gravity
        particle.velocity.y += gravity;

        // Dampen the velocity for a fluid effect
        particle.velocity.x *= drag;
        particle.velocity.y *= drag;

        // Interaction with the mouse
        Vector2 toMouse = Vector2Subtract(mousePos, particle.position);
        float distance = Vector2Length(toMouse);
        if (distance < interactionRadius) {
            Vector2 repel = Vector2Scale(Vector2Normalize(toMouse), -1.5f);
            particle.velocity = Vector2Add(particle.velocity, repel);
        }
    }
}

// Update particle positions and handle collisions with glass
void UpdateParticles(std::vector<Particle>& particles) {
    for (auto &particle : particles) {
        // Update position
        particle.position = Vector2Add(particle.position, particle.velocity);

        // Check collision with glass walls
        if (particle.position.x < screenWidth / 2 - glassWidth / 2 + particleRadius) {
            particle.position.x = screenWidth / 2 - glassWidth / 2 + particleRadius;
            particle.velocity.x *= -0.8f; // Reflect and reduce speed
        }
        if (particle.position.x > screenWidth / 2 + glassWidth / 2 - particleRadius) {
            particle.position.x = screenWidth / 2 + glassWidth / 2 - particleRadius;
            particle.velocity.x *= -0.8f; // Reflect and reduce speed
        }
        if (particle.position.y > screenHeight / 2 + glassHeight / 2 - particleRadius) {
            particle.position.y = screenHeight / 2 + glassHeight / 2 - particleRadius;
            particle.velocity.y *= -0.8f; // Reflect and reduce speed
        }
        // Prevent particles from going above the glass
        if (particle.position.y < screenHeight / 2 - glassHeight / 2 + particleRadius) {
            particle.position.y = screenHeight / 2 - glassHeight / 2 + particleRadius;
            particle.velocity.y *= -0.8f; // Reflect and reduce speed
        }
    }
}

// Draw fluid properties
void DrawFluidProperties(const std::vector<Particle>& particles) {
    // Calculate average velocity or density
    Vector2 avgVelocity = {0, 0};
    for (const auto& particle : particles) {
        avgVelocity = Vector2Add(avgVelocity, particle.velocity);
    }
    avgVelocity = Vector2Scale(avgVelocity, 1.0f / particles.size());

    DrawText(TextFormat("Average Velocity: (%.2f, %.2f)", avgVelocity.x, avgVelocity.y), 10, 10, 20, DARKGRAY);
}

int main() {
    InitWindow(screenWidth, screenHeight, "Fluid Simulation - Half-Full Tub");
    SetTargetFPS(60);

    // Initialize particles
    std::vector<Particle> particles = InitializeParticles();

    // Main game loop
    while (!WindowShouldClose()) {
        Vector2 mousePos = GetMousePosition();

        // Apply forces and update particle positions
        ApplyForces(particles, mousePos);
        UpdateParticles(particles);

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw glass
        DrawRectangleLines(screenWidth / 2 - glassWidth / 2, screenHeight / 2 - glassHeight / 2, glassWidth, glassHeight, DARKGRAY);

        // Draw particles (water)
        for (const auto &particle : particles) {
            DrawCircleV(particle.position, particleRadius, particle.color);
        }

        // Draw fluid properties
        DrawFluidProperties(particles);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
