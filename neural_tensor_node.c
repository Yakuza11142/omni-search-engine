/**
 * @file neural_tensor_node.c
 * @brief Deterministic Neural Core & Tensor Inference Engine - Pure Standard C, Zero External APIs
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

// Structure representing a 1D/2D Tensor for neural inference
typedef struct {
    int rows;
    int cols;
    float *data;
} Tensor;

// Allocate a tensor dynamically
Tensor* allocate_tensor(int rows, int cols) {
    Tensor *t = (Tensor *)malloc(sizeof(Tensor));
    if (!t) return NULL;
    t->rows = rows;
    t->cols = cols;
    t->data = (float *)calloc((size_t)rows * cols, sizeof(float));
    if (!t->data) {
        free(t);
        return NULL;
    }
    return t;
}

// Free tensor memory securely
void free_tensor(Tensor *t) {
    if (t) {
        if (t->data) {
            free(t->data);
        }
        free(t);
    }
}

// Execute deterministic matrix multiplication (Dense layer forward pass: Output = Input * Weights + Bias)
Tensor* tensor_dense_forward(const Tensor *input, const Tensor *weights, const Tensor *bias) {
    if (!input || !weights || !bias || input->cols != weights->rows) {
        return NULL;
    }

    Tensor *output = allocate_tensor(input->rows, weights->cols);
    if (!output) return NULL;

    // Standard matrix multiplication loop with zero external dependencies
    for (int i = 0; i < input->rows; i++) {
        for (int j = 0; j < weights->cols; j++) {
            float sum = 0.0f;
            for (int k = 0; k < input->cols; k++) {
                sum += input->data[i * input->cols + k] * weights->data[k * weights->cols + j];
            }
            // Add bias term
            sum += bias->data[j];
            
            // Apply ReLU activation inline for deterministic neural response
            output->data[i * output->cols + j] = (sum > 0.0f) ? sum : 0.0f;
        }
    }

    return output;
}

int main(int argc, char *argv[]) {
    printf("Initializing Neural Tensor Inference Core (Domain 7)...\n");

    // Define a sample 1x2 input tensor
    Tensor *input = allocate_tensor(1, 2);
    input->data[0] = 0.5f;
    input->data[1] = -1.2f;

    // Define a sample 2x2 weight matrix
    Tensor *weights = allocate_tensor(2, 2);
    weights->data[0] = 0.8f; weights->data[1] = -0.4f;
    weights->data[2] = 0.2f; weights->data[3] =  0.9f;

    // Define a 1x2 bias vector
    Tensor *bias = allocate_tensor(1, 2);
    bias->data[0] = 0.1f;
    bias->data[1] = -0.2f;

    // Execute forward pass
    Tensor *output = tensor_dense_forward(input, weights, bias);
    if (output) {
        printf("Neural Forward Pass Completed Successfully.\n");
        printf("Output Tensor Result -> [ %.4f, %.4f ]\n", output->data[0], output->data[1]);
        free_tensor(output);
    } else {
        fprintf(stderr, "Neural forward pass execution failed.\n");
    }

    free_tensor(input);
    free_tensor(weights);
    free_tensor(bias);
    return 0;
}
