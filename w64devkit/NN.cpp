#include "raylib.h"
#include <cmath>
#include <vector>
#include <iostream>
using namespace std;
// Activation Function (Sigmoid)
float sigmoid(float x) {
    return 1.0f / (1.0f + exp(-x));
}

// Derivative of the Sigmoid (for backpropagation)
float sigmoid_derivative(float x) {
    return x * (1 - x);
}

// XOR Dataset
float XOR_inputs[4][2] = {
    {0, 0},
    {0, 1},
    {1, 0},
    {1, 1}
};

float XOR_outputs[4] = {0, 1, 1, 0};

// Neural Network class
class NeuralNetwork {
public:
    vector<float> hiddenLayer;
    vector<float> outputLayer;
    vector<float> weights_input_hidden;
    vector<float> weights_hidden_output;
    float learningRate = 0.1f;

    NeuralNetwork() {
        hiddenLayer.resize(4);
        outputLayer.resize(1);

        // Initialize weights randomly
        weights_input_hidden = {RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat()};
        weights_hidden_output = {RandomFloat(), RandomFloat(), RandomFloat(), RandomFloat()};
    }

    float RandomFloat() {
        return (float)rand() / RAND_MAX * 2.0f - 1.0f;
    }

    // Feedforward
    void feedForward(float input1, float input2) {
        // Input to Hidden
        hiddenLayer[0] = sigmoid(input1 * weights_input_hidden[0] + input2 * weights_input_hidden[1]);
        hiddenLayer[1] = sigmoid(input1 * weights_input_hidden[2] + input2 * weights_input_hidden[3]);
        hiddenLayer[2] = sigmoid(input1 * weights_input_hidden[4] + input2 * weights_input_hidden[5]);
        hiddenLayer[3] = sigmoid(input1 * weights_input_hidden[6] + input2 * weights_input_hidden[7]);

        // Hidden to Output
        outputLayer[0] = sigmoid(hiddenLayer[0] * weights_hidden_output[0] +
                                 hiddenLayer[1] * weights_hidden_output[1] +
                                 hiddenLayer[2] * weights_hidden_output[2] +
                                 hiddenLayer[3] * weights_hidden_output[3]);
    }

    // Backpropagation (simple XOR training)
    void backpropagate(float input1, float input2, float expected_output) {
        // Calculate output error
        float outputError = expected_output - outputLayer[0];
        
        // Calculate gradients for output layer weights
        for (int i = 0; i < 4; i++) {
            weights_hidden_output[i] += learningRate * outputError * sigmoid_derivative(outputLayer[0]) * hiddenLayer[i];
        }

        // Backpropagate to hidden layer
        for (int i = 0; i < 4; i++) {
            float hiddenError = outputError * sigmoid_derivative(outputLayer[0]) * weights_hidden_output[i];
            weights_input_hidden[i * 2] += learningRate * hiddenError * input1;
            weights_input_hidden[i * 2 + 1] += learningRate * hiddenError * input2;
        }
    }

    void train(int epochs) {
        for (int epoch = 0; epoch < epochs; epoch++) {
            for (int i = 0; i < 4; i++) {
                feedForward(XOR_inputs[i][0], XOR_inputs[i][1]);
                backpropagate(XOR_inputs[i][0], XOR_inputs[i][1], XOR_outputs[i]);
            }
        }
    }
};

// Visualization function
void drawNeuralNetwork(NeuralNetwork& nn) {
    int inputX = 200, hiddenX = 400, outputX = 600;
    int inputY[2] = {300, 400}, hiddenY[4] = {200, 300, 400, 500}, outputY = 350;

    // Draw input layer
    for (int i = 0; i < 2; i++) {
        DrawCircle(inputX, inputY[i], 20, BLUE);
    }

    // Draw hidden layer
    for (int i = 0; i < 4; i++) {
        DrawCircle(hiddenX, hiddenY[i], 20, GREEN);
        // Draw connections from input to hidden
        DrawLine(inputX, inputY[0], hiddenX, hiddenY[i], LIGHTGRAY);
        DrawLine(inputX, inputY[1], hiddenX, hiddenY[i], LIGHTGRAY);
    }

    // Draw output layer
    DrawCircle(outputX, outputY, 20, RED);
    // Draw connections from hidden to output
    for (int i = 0; i < 4; i++) {
        DrawLine(hiddenX, hiddenY[i], outputX, outputY, LIGHTGRAY);
    }
}

int main() {
    // Initialize Raylib
    InitWindow(800, 600, "Neural Network Visualization");

    NeuralNetwork nn;
    nn.train(10000);  // Train XOR neural network

    SetTargetFPS(60);

    // Main game loop
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw the neural network structure
        drawNeuralNetwork(nn);

        EndDrawing();
    }

    // De-initialization
    CloseWindow();

    return 0;
}
