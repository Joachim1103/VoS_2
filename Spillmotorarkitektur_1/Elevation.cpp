#include <GLFW/glfw3.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

// Structure to store the XYZ coordinates
struct Point {
    float x, y, z;
};

// Camera control variables
float cameraAngleX = 0.0f, cameraAngleY = 0.0f;
float cameraDistance = 2.0f; // Starting distance from the point cloud
float cameraOffsetX = 0.0f, cameraOffsetY = 0.0f;
bool isLeftMousePressed = false, isRightMousePressed = false;
double lastMouseX = 0.0, lastMouseY = 0.0;

// Function to load terrain data from the text file
std::vector<Point> loadTerrainData(const std::string& filename) {
    std::vector<Point> points;
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "Failed to open file: " << filename << std::endl;
        return points;
    }

    int numPoints;
    file >> numPoints; // First line is the total number of points

    float x, y, z;
    while (file >> x >> y >> z) {
        points.push_back({ x, y, z });
    }

    file.close();
    std::cout << "Loaded " << points.size() << " points." << std::endl;
    return points;
}

// Adjust the points for better visibility
void adjustPoints(std::vector<Point>& points) {
    if (points.empty()) return;

    float minX = points[0].x, maxX = points[0].x;
    float minY = points[0].y, maxY = points[0].y;
    float minZ = points[0].z, maxZ = points[0].z;

    for (const auto& p : points) {
        if (p.x < minX) minX = p.x;
        if (p.x > maxX) maxX = p.x;
        if (p.y < minY) minY = p.y;
        if (p.y > maxY) maxY = p.y;
        if (p.z < minZ) minZ = p.z;
        if (p.z > maxZ) maxZ = p.z;
    }

    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    float scale = std::max({ maxX - minX, maxY - minY, maxZ - minZ }) / 2.0f;
    if (scale == 0.0f) scale = 1.0f;

    for (auto& p : points) {
        p.x = (p.x - centerX) / scale;
        p.y = (p.y - centerY) / scale;
        p.z = (p.z - centerZ) / scale;
    }
}

// Render the terrain point cloud
void renderTerrain(const std::vector<Point>& points) {
    glColor3f(1.0f, 1.0f, 1.0f);
    glBegin(GL_POINTS);
    for (const auto& p : points) {
        glVertex3f(p.x, p.y, p.z);
    }
    glEnd();
}

// Setup the camera based on controls
void setupCamera() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.5, 1.5, -1.5, 1.5, -10.0, 10.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glTranslatef(cameraOffsetX, cameraOffsetY, -cameraDistance);
    glRotatef(cameraAngleX, 1.0f, 0.0f, 0.0f);
    glRotatef(cameraAngleY, 0.0f, 1.0f, 0.0f);
}

// Mouse callback for camera interaction
void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        isLeftMousePressed = (action == GLFW_PRESS);
        glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        isRightMousePressed = (action == GLFW_PRESS);
        glfwGetCursorPos(window, &lastMouseX, &lastMouseY);
    }
}

// Mouse motion callback
void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
    if (isLeftMousePressed) {
        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;
        cameraAngleY += deltaX * 0.1f;
        cameraAngleX -= deltaY * 0.1f;
    }
    else if (isRightMousePressed) {
        double deltaX = xpos - lastMouseX;
        double deltaY = ypos - lastMouseY;
        cameraOffsetX += deltaX * 0.01f;
        cameraOffsetY -= deltaY * 0.01f;
    }

    lastMouseX = xpos;
    lastMouseY = ypos;
}

// Scroll callback for zooming
void scrollCallback(GLFWwindow* window, double xoffset, double yoffset) {
    cameraDistance -= yoffset * 0.1f;
    if (cameraDistance < 0.1f) cameraDistance = 0.1f; // Prevent zooming too close
}

// Setup OpenGL/GLFW
void setupOpenGL(const std::vector<Point>& points) {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return;
    }

    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Terrain Visualization", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    glfwMakeContextCurrent(window);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glPointSize(3.0f);
    glEnable(GL_DEPTH_TEST);

    setupCamera();

    // Register callbacks
    glfwSetMouseButtonCallback(window, mouseButtonCallback);
    glfwSetCursorPosCallback(window, cursorPositionCallback);
    glfwSetScrollCallback(window, scrollCallback);

    // Main loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        setupCamera();
        renderTerrain(points);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
}

int main() {
    std::string filename = "Elevation Data.txt";
    std::vector<Point> points = loadTerrainData(filename);

    if (points.empty()) {
        std::cerr << "No points loaded. Check the input file." << std::endl;
        return -1;
    }

    adjustPoints(points);
    setupOpenGL(points);
    return 0;
}
