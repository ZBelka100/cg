#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

// Структура для вершины всего
struct Vertex {
    glm::vec3 Position;
    glm::vec4 Color;
};

// Структура для линии нормали
struct NormalLine {
    glm::vec3 Start;
    glm::vec3 End;
    glm::vec4 Color;
};


// Вершины куба (36 вершин для 12 треугольников, 6 граней)
std::vector<Vertex> cubeVertices = {
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

int main()
{
    // Инициализация GLFW
    if (!glfwInit())
    {
        std::cerr << "Не удалось инициализировать GLFW" << std::endl;
        return -1;
    }

    // Запрос создания контекста OpenGL ES 2.0
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);

    // Запрос буфера глубины
    glfwWindowHint(GLFW_DEPTH_BITS, 24);

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "Лаба 2", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Не удалось создать окно GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Установка текущего контекста
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Компиляция и линковка шейдерных программ
    GLuint ShaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);

    // Получение местоположений атрибутов и униформов для куба
    GLint aPosLocation = glGetAttribLocation(ShaderProgram, "aPos");
    GLint aColorLocation = glGetAttribLocation(ShaderProgram, "aColor");
    GLint uMVPLocation = glGetUniformLocation(ShaderProgram, "uMVP");

    // Получение местоположений униформа матрицы MVP для нормалей
    GLint normal_uMVPLocation = glGetUniformLocation(ShaderProgram, "uMVP");

    // Создание VBO для куба
    GLuint cubeVBO;
    glGenBuffers(1, &cubeVBO);
    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(Vertex), cubeVertices.data(), GL_STATIC_DRAW);

    // Настройка атрибутов для куба
    glUseProgram(ShaderProgram);
    glEnableVertexAttribArray(aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(aColorLocation);
    glVertexAttribPointer(aColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));

    // Создание линий нормалей
    std::vector<Vertex> normalVertices;

    auto calculateNormals = [&](const std::vector<Vertex>& vertices, std::vector<Vertex>& normals) {
        for (size_t i = 0; i < vertices.size(); i += 6) {
            // Вычисление центра грани
            glm::vec3 sum(0.0f);
            for (size_t j = i; j < i + 6; ++j)
                sum += vertices[j].Position;
            glm::vec3 center = sum / 6.0f;

            // Вычисление нормали для грани
            glm::vec3 edge1 = vertices[i + 1].Position - vertices[i].Position;
            glm::vec3 edge2 = vertices[i + 2].Position - vertices[i].Position;
            glm::vec3 normal = glm::normalize(glm::cross(edge2, edge1));

            // Определение двух конечных точек нормали
            glm::vec3 normalEndForward = center + normal * 1.0f;
            glm::vec3 normalEndBackward = center - normal * 1.0f;

            // Цвет нормали соответствует цвету грани
            glm::vec4 faceColor = vertices[i].Color;

            // Добавление двух вершин нормали в массив
            normals.push_back({center, faceColor});
            normals.push_back({normalEndForward, faceColor});
            normals.push_back({center, faceColor});
            normals.push_back({normalEndBackward, faceColor});
        }
    };

    // Заполнение normalVertices
    calculateNormals(cubeVertices, normalVertices);


    // Создание VBO для нормалей
    GLuint normalVBO;
    glGenBuffers(1, &normalVBO);
    glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
    glBufferData(GL_ARRAY_BUFFER, normalVertices.size() * sizeof(Vertex), normalVertices.data(), GL_STATIC_DRAW);

    // Настройка атрибутов для нормалей
    glEnableVertexAttribArray(aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));

    glEnableVertexAttribArray(aColorLocation);
    glVertexAttribPointer(aColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));

    // Создание VBO для координатных осей
    std::vector<Vertex> axisVertices = {
        // X ось - Красный
        { {0.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
        { {2.0f, 0.0f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
        
        // Y ось - Зеленый
        { {0.0f, 0.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
        { {0.0f, 2.0f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
        
        // Z ось - Синий
        { {0.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
        { {0.0f, 0.0f, 2.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    };

    GLuint axisVBO;
    glGenBuffers(1, &axisVBO);
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glBufferData(GL_ARRAY_BUFFER, axisVertices.size() * sizeof(Vertex), axisVertices.data(), GL_STATIC_DRAW);

    // Компиляция и настройка шейдеров для координатных осей
    glBindBuffer(GL_ARRAY_BUFFER, axisVBO);
    glEnableVertexAttribArray(aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(aColorLocation);
    glVertexAttribPointer(aColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));


    // Включение теста глубины
    glEnable(GL_DEPTH_TEST);

    // Параметры камеры
    glm::vec3 cameraPos = glm::vec3(10.0f, 0.0f, 5.0f);
    glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

    // Матрица проекции (перспективная)
    glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                            800.0f / 600.0f,
                                            0.1f,
                                            100.0f);

    // Матрица вида
    glm::mat4 viewMat = glm::lookAt(cameraPos,
                                   cameraTarget,
                                   cameraUp);

    // Переменные для вращения куба
    float rotationX = 0.0f;
    float rotationY = 0.0f;
    float rotationZ = 0.0f; 

    // Позиции для двух кубов
    std::vector<glm::vec3> cubePositions = {
        glm::vec3(-1.5f, 0.0f, 0.0f),
        glm::vec3(1.5f, 0.0f, 0.0f)
    };

    // Основной цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Время между кадрами
        
        static double lastTime = glfwGetTime();
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Обработка ввода
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, GLFW_TRUE);

        // Управление вращением куба
        float rotateSpeed = 50.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            rotationY -= rotateSpeed;
        if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            rotationY += rotateSpeed;
        if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            rotationX -= rotateSpeed;
        if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            rotationX += rotateSpeed;
        if (glfwGetKey(window, GLFW_KEY_Q) == GLFW_PRESS) 
            rotationZ -= rotateSpeed;
        if (glfwGetKey(window, GLFW_KEY_E) == GLFW_PRESS)
            rotationZ += rotateSpeed;

        // Очистка буферов
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glUseProgram(ShaderProgram);

        // Матрица проекции и вида
        glm::mat4 mvp = projection * viewMat;

        // Отрисовка кубов и нормалей
        for (const auto& pos : cubePositions)
        {
            glm::mat4 model = glm::mat4(1.0f);
            model = glm::translate(model, pos);
            model = glm::rotate(model, glm::radians(rotationX), glm::vec3(1.0f, 0.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotationY), glm::vec3(0.0f, 1.0f, 0.0f));
            model = glm::rotate(model, glm::radians(rotationZ), glm::vec3(0.0f, 0.0f, 1.0f)); 

            glm::mat4 mvp_current = projection * viewMat * model;

            // Установка матрицы MVP один раз
            glUniformMatrix4fv(uMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp_current));

            // Установка общих атрибутов
            glEnableVertexAttribArray(aPosLocation);
            glEnableVertexAttribArray(aColorLocation);

            // Отрисовка куба
            glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
            glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
            glVertexAttribPointer(aColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(offsetof(Vertex, Color)));
            glDrawArrays(GL_TRIANGLES, 0, cubeVertices.size());

            // Отрисовка нормалей
            glBindBuffer(GL_ARRAY_BUFFER, normalVBO);
            glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
            glVertexAttribPointer(aColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
            glDrawArrays(GL_LINES, 0, normalVertices.size());

        }

        glUniformMatrix4fv(uMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
        glDrawArrays(GL_LINES, 0, axisVertices.size());

        // Обмен буферов и обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка ресурсов
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &normalVBO);
    glDeleteBuffers(1, &axisVBO);
    glDeleteProgram(ShaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
