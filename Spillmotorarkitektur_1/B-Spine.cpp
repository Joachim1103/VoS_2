//#include <GLFW/glfw3.h>
//#include <iostream>
//#include <vector>
//#include <cmath>
//
//// Define control points as specified in the provided document setup
//std::vector<std::vector<GLfloat>> controlPoints = {
//    { 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 2.0f, 0.0f, 0.0f, 3.0f, 0.0f, 0.0f }, // First row
//    { 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, 2.0f, 2.0f, 1.0f, 2.0f, 3.0f, 1.0f, 0.0f }, // Second row
//    { 0.0f, 2.0f, 0.0f, 1.0f, 2.0f, 0.0f, 2.0f, 2.0f, 0.0f, 3.0f, 2.0f, 0.0f }  // Third row
//};
//
//// Knot vector for u and v directions
//GLfloat mu[] = { 0.0f, 0.0f, 0.0f, 1.0f, 2.0f, 2.0f, 2.0f };
//GLfloat mv[] = { 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f };
//
//// Fixed rotation angles
//float rotationAngleX = -607.0f;
//float rotationAngleY = 187.0f;
//float cameraDistance = 6.0f; // Maintain standard distance
//
//// B-spline basis function (quadratic)
//float B(int i, int degree, float t, GLfloat* knots) {
//    if (degree == 0) {
//        return (t >= knots[i] && t < knots[i + 1]) ? 1.0f : 0.0f;
//    }
//    float leftDenom = knots[i + degree] - knots[i];
//    float rightDenom = knots[i + degree + 1] - knots[i + 1];
//
//    float left = 0.0f;
//    float right = 0.0f;
//
//    if (leftDenom != 0.0f) {
//        left = (t - knots[i]) / leftDenom * B(i, degree - 1, t, knots);
//    }
//
//    if (rightDenom != 0.0f) {
//        right = (knots[i + degree + 1] - t) / rightDenom * B(i + 1, degree - 1, t, knots);
//    }
//
//    return left + right;
//}
//
//// Evaluate point on the B-spline surface
//void evaluateBSplineSurface(float u, float v, float& x, float& y, float& z) {
//    int du = 2, dv = 2;
//    x = y = z = 0.0f;
//
//    for (int i = 0; i < 4; i++) {
//        for (int j = 0; j < 3; j++) {
//            float Bu = B(i, du, u, mu);
//            float Bv = B(j, dv, v, mv);
//            x += controlPoints[j][i * 3] * Bu * Bv;
//            y += controlPoints[j][i * 3 + 1] * Bu * Bv;
//            z += controlPoints[j][i * 3 + 2] * Bu * Bv;
//        }
//    }
//}
//
//// Render the B-Spline wireframe surface
//void renderBSplineWireframe() {
//    float step = 0.05f;
//    glBegin(GL_LINES);
//    for (float u = 0.0f; u <= 2.0f; u += step) {
//        for (float v = 0.0f; v <= 1.0f; v += step) {
//            float x1, y1, z1, x2, y2, z2, x3, y3, z3;
//            evaluateBSplineSurface(u, v, x1, y1, z1);
//            evaluateBSplineSurface(u + step, v, x2, y2, z2);
//            evaluateBSplineSurface(u, v + step, x3, y3, z3);
//
//            glVertex3f(x1, y1, z1);
//            glVertex3f(x2, y2, z2);
//
//            glVertex3f(x1, y1, z1);
//            glVertex3f(x3, y3, z3);
//        }
//    }
//    glEnd();
//}
//
//// Setup orthographic projection for isometric view
//void setupProjection(int width, int height) {
//    glViewport(0, 0, width, height);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    float aspect = (float)width / (float)height;
//    float scale = 4.0f;
//    glOrtho(-scale * aspect, scale * aspect, -scale, scale, -10.0f, 10.0f);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//}
//
//// Static camera setup with fixed rotation angles
//void setupCamera() {
//    glLoadIdentity();
//    glTranslatef(0.0f, 0.0f, -cameraDistance);
//    glRotatef(rotationAngleX, 1.0f, 0.0f, 0.0f);
//    glRotatef(rotationAngleY, 0.0f, 1.0f, 0.0f);
//    glTranslatef(-1.5f, -1.0f, 0.0f);
//}
//
//// OpenGL/GLFW setup
//void setupOpenGL() {
//    if (!glfwInit()) {
//        std::cerr << "Failed to initialize GLFW" << std::endl;
//        return;
//    }
//
//    GLFWwindow* window = glfwCreateWindow(1920, 1080, "Static Wireframe View", NULL, NULL);
//    if (!window) {
//        std::cerr << "Failed to create GLFW window" << std::endl;
//        glfwTerminate();
//        return;
//    }
//
//    glfwMakeContextCurrent(window);
//    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); // Wireframe mode
//
//    setupProjection(1920, 1080);
//
//    while (!glfwWindowShouldClose(window)) {
//        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//        setupCamera();
//        renderBSplineWireframe();
//        glfwSwapBuffers(window);
//        glfwPollEvents();
//    }
//
//    glfwTerminate();
//}
//
//int main() {
//    setupOpenGL();
//    return 0;
//}
