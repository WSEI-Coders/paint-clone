#include <iostream>
#include <stack>
#include <string>
#include <vector>
#include <opencv2/opencv.hpp>

// User Data Structure
struct UserData {
    cv::Mat image;

    // Circle
    cv::Point center;
    int radius;
    bool drawingCircle;
    bool isDoneCircle;

    // Rectagle
    cv::Point tl;
    cv::Point br;
    bool drawingRect;
    bool isDoneRect;
    bool exist;

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
        ud->drawingCircle = true;
    } else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON) && ud->drawingCircle) {
        // Calculating the circle radius based on mouse movement
        int dx = x - ud->center.x;
        int dy = y - ud->center.y;
        ud->radius = cv::sqrt(dx*dx + dy*dy);
    } else if (event == cv::EVENT_LBUTTONUP) {
        // Stop drawing
        ud->drawingCircle = false;

        // Set to true, because we have a final circle
        ud->isDoneCircle = true;
    }
}

void drawRectangle(int event, int x, int y, int flags, void* userdata) {
    UserData* ud = static_cast<UserData*>(userdata);

    if (event == cv::EVENT_LBUTTONDOWN) {
        ud->tl = cv::Point(x,y);
        ud->drawingRect = true;
    }
    else if (event == cv::EVENT_MOUSEMOVE && (flags & cv::EVENT_FLAG_LBUTTON) && ud->drawingRect) {
        ud->br = cv::Point(x,y);
        ud->exist = true;
    }
    else if (event == cv::EVENT_LBUTTONUP) {
        ud->drawingRect = false;
        ud->isDoneRect = true;
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
    ud.drawingCircle = false;
    ud.isDoneCircle = false;
    ud.drawingRect = false;
    ud.isDoneRect = false;
    ud.exist = false;

    // Declare variables to store the last drawn circle
    cv::Point lastCenter;
    int lastRadius = 0;

    cv::Point lastTL;
    cv::Point lastBR;

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

        if (key == 'c') {
            cv::setMouseCallback(namedWindow, drawCircle, &ud);
        }
        if (key == 'r') {
            cv::setMouseCallback(namedWindow, drawRectangle, &ud);
        }
        
        // If a circle is being drawn, draw it on the image
        if (ud.drawingCircle) {
            lastCenter = ud.center;
            lastRadius = ud.radius;
        }
        // If there is no radius, don't call cv::circle
        if (lastRadius != 0) {
            cv::circle(display, lastCenter, lastRadius, cv::Scalar(0, 255, 0), 2);
        }
        
        // If the circle is full created add current image to the stack
        if (ud.isDoneCircle) {
            ud.s.emplace(display.clone());
            ud.isDoneCircle = false;
            lastRadius = 0;
            lastCenter = {};
        }
        
        if (ud.drawingRect) {
            lastTL = ud.tl;
            lastBR = ud.br;
        }

        if (ud.exist) {
            cv::rectangle(display, {lastTL, lastBR}, cv::Scalar(255, 0, 255), 2);
        }

        if (ud.isDoneRect) {
            ud.s.emplace(display.clone());
            ud.isDoneRect = false;
            lastTL = {};
            lastBR = {};
            ud.exist = false;
        }

        // Show the image
        cv::imshow(namedWindow, display);
        key = cv::waitKey(1);
    }

    cv::destroyAllWindows();
    return 0;
}

