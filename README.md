#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <opencv2/opencv.hpp>

#define CANVAS_WIDTH 400
#define CANVAS_HEIGHT 400
#define NUM_SQUARES 10
#define MAX_OPACITY 255
#define NUM_GENERATIONS 100
#define MUTATION_RATE 0.1


typedef struct {
    int x;
    int y;
    cv::Scalar color;
    int opacity;
} Square;


void drawSquare(cv::Mat& canvas, const Square& square) {
    cv::rectangle(canvas, cv::Point(square.x, square.y), cv::Point(square.x + 20, square.y + 20), square.color, -1);
    cv::addWeighted(canvas, square.opacity / (float)MAX_OPACITY, canvas, 1.0 - square.opacity / (float)MAX_OPACITY, 0, canvas);
}


Square crossover(const Square& parent1, const Square& parent2) {
    Square child;
    child.x = parent1.x;
    child.y = parent2.y;
    child.color = parent1.color;
    child.opacity = parent2.opacity;
    return child;
}


void mutate(Square& square) {
    if (rand() / (float)RAND_MAX < MUTATION_RATE) {
        square.x = rand() % CANVAS_WIDTH;
        square.y = rand() % CANVAS_HEIGHT;
        square.color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
        square.opacity = rand() % (MAX_OPACITY + 1);
    }
}

// Function to evaluate the fitness of a square
float fitness(const Square& square, const cv::Mat& targetImage) {
    // Implement your fitness function here
    // Compare the generated square with the target image
}

int main() {
    Seed the random number generator with the current time
    srand(time(NULL));
    
  cv::Mat canvas(CANVAS_HEIGHT, CANVAS_WIDTH, CV_8UC3, cv::Scalar(255, 255, 255));
    
  Square population[NUM_SQUARES];
    
   for (int generation = 0; generation < NUM_GENERATIONS; generation++) {
        
  cv::imshow("Generated Image", canvas);
        cv::waitKey(1);  // Wait for a short period to update the display
    }
    
    // Display the final image
   cv::imshow("Final Image", canvas);
    cv::waitKey(0);  // Wait until a key is pressed
    
  return 0;
}
