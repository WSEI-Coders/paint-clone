#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

// User Data Structure
struct UserData {
    cv::Mat image;
    cv::Point center;
    int radius;
    // Flag indicating whether a circle is currently being drawn.
    bool drawing;
    // Flag indicating whether a circle is fully created.
    bool isDone;
    std::stack<cv::Mat> s;
};

// onMouse function for drawing a circle
void drawCircle(int event, int x, int y, int flags, void* userdata) {
    UserData* ud = static_cast<UserData*>(userdata);

    if (event == cv::EVENT_LBUTTONDOWN) {
        // Setting centre of the circle
        ud->center = cv::Point(x, y);
        ud->radius = 0;

        // Start drawing
        ud->drawing = true;
    } else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON) && ud->drawing) {
        // Calculating the circle radius based on mouse movement
        int dx = x - ud->center.x;
        int dy = y - ud->center.y;
        ud->radius = cv::sqrt(dx*dx + dy*dy);
    } else if (event == cv::EVENT_LBUTTONUP) {
        // Stop drawing
        ud->drawing = false;

        // Set to true, because we have a final circle
        ud->isDone = true;
    }
}

int main() {
    // Create named window
    std::string namedWindow{"Drawing"};
    cv::namedWindow(namedWindow);

    // Object of user data
    UserData ud;

    // Create an empty black canvas
    ud.image = cv::Mat::zeros(500, 500, CV_8UC3);
    ud.s.emplace(ud.image);
    
    // Set starting values
    ud.radius = 0;
    ud.drawing = false;
    ud.isDone = false;

    // Declare variables to store the last drawn circle
    cv::Point lastCenter;
    int lastRadius = 0;

    // Set callback
    cv::setMouseCallback(namedWindow, drawCircle, &ud);

    cv::Mat display = ud.s.top().clone();
    char key = 0;
    while (key != 'q') {
        // Copy the original image to erase the previous frame
        display = ud.s.top().clone();

        // Undo last circle
        if (key == 'z' && ud.s.size() > 1) {
            ud.s.pop();
        }
        
        // If a circle is being drawn, draw it on the image
        if (ud.drawing) {
            lastCenter = ud.center;
            lastRadius = ud.radius;
            cv::circle(display, lastCenter, lastRadius, cv::Scalar(0, 255, 0), 2);
        }
        // If there is no radius, don't call cv::circle
        if (lastRadius != 0) {
            cv::circle(display, lastCenter, lastRadius, cv::Scalar(0, 255, 0), 2);
        }
        
        // If the circle is full created add current image to the stack
        if (ud.isDone) {
            ud.s.emplace(display.clone());
            ud.isDone = false;
            lastRadius = 0;
            lastCenter = {};
        } 
        
        // Show the image
        cv::imshow(namedWindow, display);
        key = cv::waitKey(10);
    }

    cv::destroyAllWindows();
    return 0;
}

