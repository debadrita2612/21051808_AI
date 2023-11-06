#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <opencv2/opencv.hpp>

// Constants
#define CANVAS_WIDTH 400
#define CANVAS_HEIGHT 400
#define NUM_SQUARES 10
#define MAX_OPACITY 255
#define NUM_GENERATIONS 100
#define MUTATION_RATE 0.1

// Struct to represent squares
typedef struct {
    int x;
    int y;
    cv::Scalar color;
    int opacity;
    float fitness;
} Square;

// Function to draw a square on the canvas
void drawSquare(cv::Mat& canvas, const Square& square) {
    cv::Scalar color = square.color;
    int opacity = square.opacity;

    // Create a rectangle with the specified color and opacity
    cv::Mat overlay(canvas.size(), canvas.type(), color);
    cv::addWeighted(overlay, opacity / (float)MAX_OPACITY, canvas, 1.0 - opacity / (float)MAX_OPACITY, 0, canvas);
}

// Function to perform a simple crossover between two squares
Square crossover(const Square& parent1, const Square& parent2) {
    Square child;

    // Crossover position: Choose x from parent1 and y from parent2
    child.x = parent1.x;
    child.y = parent2.y;

    // Crossover color: Choose color from parent1
    child.color = parent1.color;

    // Crossover opacity: Choose opacity from parent2
    child.opacity = parent2.opacity;

    return child;
}

// Function to mutate a square
void mutate(Square& square) {
    if (rand() / (float)RAND_MAX < MUTATION_RATE) {
        // Mutate the square's position (x, y) slightly
        square.x += rand() % 11 - 5;  // Random change between -5 and 5
        square.y += rand() % 11 - 5;

        // Ensure the square remains within the canvas bounds
        square.x = std::max(0, std::min(CANVAS_WIDTH - 20, square.x));
        square.y = std::max(0, std::min(CANVAS_HEIGHT - 20, square.y));

        // Mutate the square's color
        square.color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);

        // Mutate the square's opacity
        square.opacity = rand() % (MAX_OPACITY + 1);
    }
}

// Function to evaluate the fitness of a square
float fitness(const Square& square, const cv::Mat& testtImage) {
cv::Mat generatedImage(CANVAS_HEIGHT, CANVAS_WIDTH, CV_8UC3, cv::Scalar(255, 255, 255));
    
    // Draw the square on the generated image
    drawSquare(generatedImage, square);
    
    // Calculate the SSIM between the generated image and the target image
    cv::Mat testImageGray, generatedImageGray;
    cv::cvtColor(testtImage, testImageGray, cv::COLOR_BGR2GRAY);
    cv::cvtColor(generatedImage, generatedImageGray, cv::COLOR_BGR2GRAY);
    
    double ssim = cv::quality::QualityPSNR::compute(testImageGray, generatedImageGray);
    
    // Normalize the SSIM score to be within the range [0, 1]
    float normalizedSsim = static_cast<float>((ssim + 1.0) / 2.0);
    
    // Invert the score to be a fitness value (higher fitness is better)
    float fitnessValue = 1.0 - normalizedSsim;
    
    
}
void selection(Square population[NUM_SQUARES]) {
    Square tempPopulation[NUM_SQUARES];
    
    for (int i = 0; i < NUM_SQUARES; i++) {
        int tournamentSize = 5;  // Adjust the tournament size as needed
        
        // Create a tournament by selecting random individuals
        Square tournament[tournamentSize];
        for (int j = 0; j < tournamentSize; j++) {
            tournament[j] = population[rand() % NUM_SQUARES];
        }
        
        // Find the best-performing individual in the tournament
        Square bestIndividual = tournament[0];
        for (int j = 1; j < tournamentSize; j++) {
            if (tournament[j].fitness > bestIndividual.fitness) {
                bestIndividual = tournament[j];
            }
        }
        
        // Store the best individual in the temporary population
        tempPopulation[i] = bestIndividual;
    }
    
    // Replace the population with the selected individuals
    for (int i = 0; i < NUM_SQUARES; i++) {
        population[i] = tempPopulation[i];
    }

int main() {
    // Seed the random number generator with the current time
    srand(time(NULL));
    
    // Load the target image using OpenCV
    cv::Mat testtImage = cv::imread("testimage.jpg");
    cv::Mat canvas(CANVAS_HEIGHT, CANVAS_WIDTH, CV_8UC3, cv::Scalar(255, 255, 255));
    
    Square population[NUM_SQUARES];
    
    for (int generation = 0; generation < NUM_GENERATIONS; generation++) {
        // Evaluate fitness for each square in the population
        for (int i = 0; i < NUM_SQUARES; i++) {
            population[i].fitness = fitness(population[i], testImage);
        }
        
        // Select the top-performing squares as parents for crossover
        selection(population);
        
        // Generate new offspring through crossover and mutation
        for (int i = 0; i < NUM_SQUARES; i++) {
            int parent1 = rand() % NUM_SQUARES;
            int parent2 = rand() % NUM_SQUARES;
            population[i] = crossover(population[parent1], population[parent2]);
            mutate(population[i]);
        }
        
        // Display the canvas
        cv::imshow("Generated Image", canvas);
        cv::waitKey(1);  // Wait for a short period to update the display
    }
    
    // Display the final image
    cv::imshow("Final Image", canvas);
    cv::waitKey(0);  // Wait until a key is pressed
    
    return 0;
}
