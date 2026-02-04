#ifndef GLFWAPPLICATION_H_
#define GLFWAPPLICATION_H_

#include <iostream>
#include <string>
#include <array>
#include "glad/glad.h"
#include "GLFW/glfw3.h"

class GLFWApplication
{
public:
    GLFWApplication(const std::string& name, const std::string& version)
        : m_name(name), m_version(version), WINDOW_HEIGHT(600), WINDOW_WIDTH(800)
    {
        initializeOpenGL();
    }
    GLFWApplication(const std::string& name, const std::string& version, const int windowHeight, const int windowWidth)
        : m_name(name), m_version(version), WINDOW_HEIGHT(windowHeight), WINDOW_WIDTH(windowWidth)
    {
        initializeOpenGL();
    }
    GLFWApplication() = default;
    virtual ~GLFWApplication() = default;

    const int WINDOW_HEIGHT;
    const int WINDOW_WIDTH;

    virtual unsigned init();
    virtual unsigned run();
    virtual void shutdown();

    bool getIsOpenGLInitialized() const { return m_isOpenGLInitialized; }
    std::string getName() const { return m_name; }
    std::string getVersion() const { return m_version; }
    GLFWwindow* getWindow() const { return m_window; }
    void checkGLError(const char* label);


private:
    std::string m_name;
    std::string m_version;
    GLFWwindow* m_window;
    bool m_isOpenGLInitialized = false;

    void initializeOpenGL();
};

#endif // !GLFWAPPLICATION_H_