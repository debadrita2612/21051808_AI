#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <opencv2/opencv.hpp>

#define CANVAS_WIDTH 400
#define CANVAS_HEIGHT 400
#define NUM_SQUARES 10
#define MAX_OPACITY 255
#define NUM_GENERATIONS 100

typedef struct {
    int x;
    int y;
    cv::Scalar color;
    int opacity;
} Square;

// Function to draw a square on the canvas
void drawSquare(cv::Mat& canvas, const Square& square) {
    cv::rectangle(canvas, cv::Point(square.x, square.y), cv::Point(square.x + 20, square.y + 20), square.color, -1);
    cv::addWeighted(canvas, square.opacity / MAX_OPACITY, canvas, 1 - square.opacity / MAX_OPACITY, 0, canvas);
}

int main() {
    srand(time(NULL));
    
    cv::Mat canvas(CANVAS_HEIGHT, CANVAS_WIDTH, CV_8UC3, cv::Scalar(255, 255, 255));  // Create a white canvas
    
    for (int generation = 0; generation < NUM_GENERATIONS; generation++) {
        // Generate and draw random squares
        for (int i = 0; i < NUM_SQUARES; i++) {
            Square square;
            square.x = rand() % CANVAS_WIDTH;
            square.y = rand() % CANVAS_HEIGHT;
            square.color = cv::Scalar(rand() % 256, rand() % 256, rand() % 256);
            square.opacity = rand() % (MAX_OPACITY + 1);
            
            drawSquare(canvas, square);
        }
        
        // Display the canvas
        cv::imshow("Generated Image", canvas);
        cv::waitKey(1);
    }
    
    cv::imshow("Final Image", canvas);
    cv::waitKey(0);
    
    return 0;
}

      # Runs a set of commands using the runners shell
      - name: Run a multi-line script
        run: |
          echo Add other actions to build,
          echo test, and deploy your project.
