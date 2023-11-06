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


float fitness(const Square& square, const cv::Mat& targetImage) {
    float mse = 0.0;
    
    // Iterate over the pixels in the canvas and target image
    for (int y = 0; y < CANVAS_HEIGHT; y++) {
        for (int x = 0; x < CANVAS_WIDTH; x++) {
            cv::Vec3b canvasPixel = canvas.at<cv::Vec3b>(y, x);
            cv::Vec3b targetPixel = targetImage.at<cv::Vec3b>(y, x);
            
            // Calculate squared differences for each color channel (R, G, B)
            float squaredDiffR = static_cast<float>(canvasPixel[2]) - static_cast<float>(targetPixel[2]);
            float squaredDiffG = static_cast(float)(canvasPixel[1]) - static_cast<float>(targetPixel[1]);
            float squaredDiffB = static_cast<float>(canvasPixel[0]) - static_cast<float>(targetPixel[0]);
            
            mse += squaredDiffR * squaredDiffR + squaredDiffG * squaredDiffG + squaredDiffB * squaredDiffB;
        }
    }
    
    // Normalize MSE by the number of pixels
    mse /= (CANVAS_WIDTH * CANVAS_HEIGHT);
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
