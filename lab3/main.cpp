#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

// Структура для вершины куба с позицией и цветом
struct CubeVertex {
    glm::vec3 Position;
    glm::vec4 Color;
};

// Вершины куба (36 вершин для 12 треугольников, 6 граней)
std::vector<CubeVertex> cubeVertices = {
    // Front face - Красный
    { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },

    { {-0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f, 1.0f} },

    // Back face - Зелёный
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },

    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f, 1.0f} },

    // Left face - Синий
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },

    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, 1.0f, 1.0f} },

    // Right face - Жёлтый
    { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },

    { { 0.5f, -0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 1.0f, 0.0f, 1.0f} },

    // Top face - Фиолетовый
    { {-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} },

    { {-0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 1.0f, 1.0f} },

    // Bottom face - Бирюзовый
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { {-0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} },

    { {-0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { { 0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 1.0f, 1.0f} },
};

const char* vertexShaderSource = R"(
    attribute vec3 aPos;
    attribute vec4 aColor;

    uniform mat4 uMVP;

    varying vec4 vColor;

    void main()
    {
        gl_Position = uMVP * vec4(aPos, 1.0);
        vColor = aColor;
    }
)";

const char* fragmentShaderSource = R"(
    precision mediump float;

    varying vec4 vColor;

    void main()
    {
        gl_FragColor = vColor;
    }
)";


// Функция для компиляции шейдера
GLuint compileShader(GLenum type, const char* source)
{
    GLuint shader = glCreateShader(type);
    if (shader == 0)
    {
        std::cerr << "Ошибка создания шейдера!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    // Проверка успешности компиляции
    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled)
    {
        GLint infoLen = 0;
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);
        std::vector<char> infoLog(infoLen);
        glGetShaderInfoLog(shader, infoLen, nullptr, infoLog.data());
        std::cerr << "Ошибка компиляции шейдера: " << infoLog.data() << std::endl;
        glDeleteShader(shader);
        exit(EXIT_FAILURE);
    }

    return shader;
}

// Функция для создания программы шейдеров
GLuint createProgram(const char* vertexSource, const char* fragmentSource)
{
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    if (program == 0)
    {
        std::cerr << "Ошибка создания программы шейдеров!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    // Проверка успешности линковки
    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked)
    {
        GLint infoLen = 0;
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLen);
        std::vector<char> infoLog(infoLen);
        glGetProgramInfoLog(program, infoLen, nullptr, infoLog.data());
        std::cerr << "Ошибка линковки программы: " << infoLog.data() << std::endl;
        glDeleteProgram(program);
        exit(EXIT_FAILURE);
    }

    // Удаление шейдеров после линковки
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    return program;
}

int main() {
    if (!glfwInit())
    {
        std::cerr << "Не удалось инициализировать GLFW" << std::endl;
        return -1;
    }

    // Запрос создания контекста OpenGL ES 2.0
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    glfwWindowHint(GLFW_DEPTH_BITS, 24);
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Куб с вращающейся камерой", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Не удалось создать окно GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    // Получение местоположений атрибутов и униформов
    GLint aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    GLint aColorLocation = glGetAttribLocation(shaderProgram, "aColor");
    GLint uMVPLocation = glGetUniformLocation(shaderProgram, "uMVP");

    // Создание VBO для куба
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(CubeVertex), cubeVertices.data(), GL_STATIC_DRAW);

    // Настройка атрибутов для куба
    glEnableVertexAttribArray(aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)0);
    glEnableVertexAttribArray(aColorLocation);
    glVertexAttribPointer(aColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(offsetof(CubeVertex, Color)));
    glEnable(GL_DEPTH_TEST);

    // Параметры камеры
    float cameraOrbitAngle = 0.0f;     // Угол вращения камеры вокруг куба (в градусах)
    float cameraOrbitSpeed = 20.0f;    // Скорость вращения камеры (градусов в секунду)
    float cameraRadius = 5.0f;         // Радиус вращения камеры
    float fov = 45.0f;                  // Угол обзора (field of view) в градусах

    // Основной цикл рендеринга
    double lastTime = glfwGetTime();
    while (!glfwWindowShouldClose(window))
    {
        // Вычисление времени для анимации
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Обработка ввода с клавиатуры
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        // Управление радиусом вращения камеры
        if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) // Уменьшение радиуса
        {
            cameraRadius -= 2.0f * deltaTime;
            if (cameraRadius < 2.0f) cameraRadius = 2.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) // Увеличение радиуса
        {
            cameraRadius += 2.0f * deltaTime;
            if (cameraRadius > 20.0f) cameraRadius = 20.0f;
        }

        // Управление углом обзора (FOV)
        if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) // Уменьшение FOV
        {
            fov -= 30.0f * deltaTime;
            if (fov < 30.0f) fov = 30.0f;
        }
        if (glfwGetKey(window, GLFW_KEY_V) == GLFW_PRESS) // Увеличение FOV
        {
            fov += 30.0f * deltaTime;
            if (fov > 90.0f) fov = 90.0f;
        }

        // Обновление угла вращения камеры
        cameraOrbitAngle += cameraOrbitSpeed * deltaTime;
        if (cameraOrbitAngle >= 360.0f)
            cameraOrbitAngle -= 360.0f;

        // Вычисление позиции камеры на основе угла вращения и радиуса
        float rad = glm::radians(cameraOrbitAngle);
        glm::vec3 cameraPos = glm::vec3(cameraRadius * cos(rad), 2.0f, cameraRadius * sin(rad));

        // Матрица проекции (перспективная) с обновлённым FOV
        glm::mat4 projection = glm::perspective(glm::radians(fov),
                                                800.0f / 600.0f,
                                                0.1f,
                                                100.0f);

        // Матрица вида
        glm::mat4 viewMat = glm::lookAt(cameraPos,
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));

        // Создание матрицы MVP
        glm::mat4 mvp = projection * viewMat;

        // Передача матрицы MVP в шейдер
        glUniformMatrix4fv(uMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));

        // Очистка буферов
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Отрисовка куба
        glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(cubeVertices.size()));

        // Обмен буферов и обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка ресурсов
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
