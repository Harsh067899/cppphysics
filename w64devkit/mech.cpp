#include "raylib.h"
#include <cmath>

const int screenWidth = 800;
const int screenHeight = 600;

// Wheel properties
const float wheelRadius = 50.0f;
const float crankLength = 100.0f;
const float massRadius = 10.0f;
const float tensionFactor = 0.1f; // Factor to scale mouse input to force
const float dampingFactor = 0.98f; // Damping to simulate energy loss
const float gravity = 9.81f; // Gravitational acceleration
const float frictionCoefficient = 0.05f; // Friction coefficient
const float stringNaturalLength = crankLength; // Natural length of the string

// Global variables
float wheelAngle = 0.0f; // Angle of the wheel in radians
Vector2 wheelPosition = { screenWidth / 2, screenHeight / 2 }; // Center position of the wheel
Vector2 massPosition; // Mass position
float stringLength = crankLength; // Initial string length
float angularVelocity = 0.0f; // Angular velocity of the wheel
float massWeight; // Weight of the mass
bool isMassActive = false; // Flag to track if the mass is active

void UpdateMassPosition() {
    massPosition.x = wheelPosition.x + cos(wheelAngle) * stringLength;
    massPosition.y = wheelPosition.y + sin(wheelAngle) * stringLength;
}

int main() {
    InitWindow(screenWidth, screenHeight, "Wheel and String Simulation");
    SetTargetFPS(60);

    massWeight = massRadius * gravity; // Calculate weight based on mass

    while (!WindowShouldClose()) {
        // Input handling for string length adjustment
        if (IsMouseButtonDown(MOUSE_LEFT_BUTTON)) {
            Vector2 mousePosition = GetMousePosition();
            float newLength = mousePosition.y - wheelPosition.y;
            if (newLength < 0) newLength = 0; // Prevent negative length
            
            // Calculate the force applied on the string
            float force = (newLength - stringLength) * tensionFactor;
            if (force > 0) { // Only apply force if the string is being pulled
                // Calculate torque and update angular velocity
                float torque = wheelRadius * (force - massWeight); // Include weight in torque calculation
                float momentOfInertia = 0.5f * wheelRadius * wheelRadius; // I = 0.5 * r^2 for solid disk
                float angularAcceleration = torque / momentOfInertia; // Angular acceleration
                angularVelocity += angularAcceleration; // Update angular velocity
                isMassActive = true; // Mass is active
            }
            stringLength = newLength; // Update string length
        } else {
            // Apply damping when not pulling the string
            angularVelocity *= dampingFactor;

            // Calculate friction torque
            float frictionTorque = frictionCoefficient * angularVelocity;
            angularVelocity -= frictionTorque; // Reduce angular velocity due to friction
        }

        // Apply gravity effect when mass is active
        if (isMassActive) {
            angularVelocity += (massWeight / crankLength); // Add torque from weight
        }

        // Update wheel rotation
        wheelAngle += angularVelocity; // Update wheel angle based on angular velocity

        // Update mass position based on the current string length
        UpdateMassPosition();

        // Draw
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw wheel
        DrawCircleV(wheelPosition, wheelRadius, DARKGRAY);
        DrawLine(wheelPosition.x, wheelPosition.y, massPosition.x, massPosition.y, BLUE); // Draw string
        DrawCircleV(massPosition, massRadius, RED); // Draw mass at the end of the string

        // Draw current string length
        DrawText(TextFormat("String Length: %.2f", stringLength), 10, 10, 20, BLACK);
        DrawText(TextFormat("Angular Velocity: %.2f", angularVelocity), 10, 30, 20, BLACK);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
