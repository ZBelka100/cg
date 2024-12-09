#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

// Структура для вершины куба с позицией, нормалью и цветом
struct CubeVertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec4 Color;
};

// Вершины куба (36 вершин для 12 треугольников, 6 граней)
std::vector<CubeVertex> cubeVertices = {
    // Front face
    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },

    { {-0.5f, -0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f, 0.0f, 1.0f} },

    // Back face
    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },

    { {-0.5f, -0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },
    { {-0.5f,  0.5f, -0.5f}, {0.0f, 0.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f} },

    // Left face
    { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f, -0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },

    { {-0.5f, -0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f, -0.5f}, {-1.0f, 0.0f, 0.0f}, {0.0f, 0.0f, 1.0f, 1.0f} },

    // Right face
    { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },

    { { 0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f, 0.0f, 1.0f} },

    // Top face
    { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { {-0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },

    { {-0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { { 0.5f,  0.5f,  0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },
    { { 0.5f,  0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f, 1.0f, 1.0f} },

    // Bottom face
    { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { {-0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },

    { {-0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { { 0.5f, -0.5f,  0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
    { { 0.5f, -0.5f, -0.5f}, {0.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 1.0f, 1.0f} },
};


// Вершинный шейдер с моделью Фонга
const char* vertexShaderSource = R"(
    attribute vec3 aPos;
    attribute vec3 aNormal;
    attribute vec4 aColor;

    uniform mat4 uMVP;
    uniform mat4 uModel;
    uniform mat4 uNormalMatrix;

    varying vec3 FragPos;
    varying vec3 Normal;
    varying vec4 VertexColor;

    void main()
    {
        gl_Position = uMVP * vec4(aPos, 1.0);
        FragPos = vec3(uModel * vec4(aPos, 1.0));
        Normal = mat3(uNormalMatrix) * aNormal;
        VertexColor = aColor;
    }
)";

// Фрагментный шейдер с моделью Фонга
const char* fragmentShaderSource = R"(
    precision mediump float;

    struct Light {
        vec3 Position;
        vec3 Direction;
        vec3 Color;
        float CutOff;
        float OuterCutOff;
    };

    uniform Light uLight;
    uniform vec3 uViewPos;
    uniform int uLightType; // 0 - точечный свет, 1 - прожектор

    uniform vec3 uMaterialAmbient;
    uniform vec3 uMaterialDiffuse;
    uniform vec3 uMaterialSpecular;
    uniform float uMaterialShininess;

    varying vec3 FragPos;
    varying vec3 Normal;
    varying vec4 VertexColor;

    void main() {
        vec3 ambient = uMaterialAmbient * uLight.Color;

        vec3 norm = normalize(Normal);
        vec3 lightDir = normalize(uLight.Position - FragPos);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = uMaterialDiffuse * diff * uLight.Color;

        vec3 viewDir = normalize(uViewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), uMaterialShininess);
        vec3 specular = uMaterialSpecular * spec * uLight.Color;

        if (uLightType == 1) {
            // Прожектор
            float theta = dot(lightDir, normalize(-uLight.Direction));
            float epsilon = uLight.CutOff - uLight.OuterCutOff;
            float intensity = clamp((theta - uLight.OuterCutOff) / epsilon, 0.0, 1.0);

            diffuse *= intensity;
            specular *= intensity;
        }

        vec3 result = ambient + diffuse + specular;
        gl_FragColor = vec4(result, VertexColor.a);
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
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return program;
}

int main()
{
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "3D Куб с освещением Фонга", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Не удалось создать окно GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Установка текущего контекста
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); 

    GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    // Получение местоположений атрибутов и униформов
    GLint aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    GLint aNormalLocation = glGetAttribLocation(shaderProgram, "aNormal");
    GLint aColorLocation = glGetAttribLocation(shaderProgram, "aColor");
    GLint uMVPLocation = glGetUniformLocation(shaderProgram, "uMVP");
    GLint uModelLocation = glGetUniformLocation(shaderProgram, "uModel");
    GLint uNormalMatrixLocation = glGetUniformLocation(shaderProgram, "uNormalMatrix");
    GLint uLightPosLocation = glGetUniformLocation(shaderProgram, "uLight.Position");
    GLint uLightDirLocation = glGetUniformLocation(shaderProgram, "uLight.Direction");
    GLint uLightColorLocation = glGetUniformLocation(shaderProgram, "uLight.Color");
    GLint uLightCutOffLocation = glGetUniformLocation(shaderProgram, "uLight.CutOff");
    GLint uLightOuterCutOffLocation = glGetUniformLocation(shaderProgram, "uLight.OuterCutOff");
    GLint uLightTypeLocation = glGetUniformLocation(shaderProgram, "uLightType");
    GLint uViewPosLocation = glGetUniformLocation(shaderProgram, "uViewPos");
    GLint uMaterialAmbientLocation = glGetUniformLocation(shaderProgram, "uMaterialAmbient");
    GLint uMaterialDiffuseLocation = glGetUniformLocation(shaderProgram, "uMaterialDiffuse");
    GLint uMaterialSpecularLocation = glGetUniformLocation(shaderProgram, "uMaterialSpecular");
    GLint uMaterialShininessLocation = glGetUniformLocation(shaderProgram, "uMaterialShininess");

    // Создание VBO для куба
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, cubeVertices.size() * sizeof(CubeVertex), cubeVertices.data(), GL_STATIC_DRAW);

    // Настройка атрибутов для куба
    glEnableVertexAttribArray(aPosLocation);
    glVertexAttribPointer(aPosLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)0);

    glEnableVertexAttribArray(aNormalLocation);
    glVertexAttribPointer(aNormalLocation, 3, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(offsetof(CubeVertex, Normal)));

    glEnableVertexAttribArray(aColorLocation);
    glVertexAttribPointer(aColorLocation, 4, GL_FLOAT, GL_FALSE, sizeof(CubeVertex), (void*)(offsetof(CubeVertex, Color)));
    glEnable(GL_DEPTH_TEST);

    // Параметры освещения
    glm::vec3 lightPos(2.0f, 2.0f, 2.0f);
    glm::vec3 lightDir(2.0f, -1.0f, 1.0f); // Направление прожектора
    glm::vec3 lightColor(1.0f, 1.0f, 1.0f); // Белый свет
    float lightCutOff = glm::cos(glm::radians(4.5f));
    float lightOuterCutOff = glm::cos(glm::radians(5.5f));

    // Параметры материала
    glm::vec3 materialAmbient(0.2f, 0.2f, 0.2f);
    glm::vec3 materialDiffuse(0.5f, 0.5f, 0.5f);
    glm::vec3 materialSpecular(1.0f, 1.0f, 1.0f);
    float materialShininess = 32.0f;

    // Передача начальных значений освещения и материала в шейдер
    glUniform3fv(uLightPosLocation, 1, glm::value_ptr(lightPos));
    glUniform3fv(uLightDirLocation, 1, glm::value_ptr(lightDir));
    glUniform3fv(uLightColorLocation, 1, glm::value_ptr(lightColor));
    glUniform1f(uLightCutOffLocation, lightCutOff);
    glUniform1f(uLightOuterCutOffLocation, lightOuterCutOff);
    glUniform1i(uLightTypeLocation, 1);
    glUniform3fv(uMaterialAmbientLocation, 1, glm::value_ptr(materialAmbient));
    glUniform3fv(uMaterialDiffuseLocation, 1, glm::value_ptr(materialDiffuse));
    glUniform3fv(uMaterialSpecularLocation, 1, glm::value_ptr(materialSpecular));
    glUniform1f(uMaterialShininessLocation, materialShininess);
    int lightType = 1; // Начальное значение - прожектор

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

        if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
        {
            lightType = 0; // Точечный свет
        }
        if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
        {
            lightType = 1; // Прожектор
        }

        // Передача типа освещения в шейдер
        glUniform1i(uLightTypeLocation, lightType);

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

        // Управление цветом источника света
        if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) // Увеличение красного
        {
            lightColor.r += 1.0f * deltaTime;
            if (lightColor.r > 1.0f) lightColor.r = 1.0f;
            glUniform3fv(uLightColorLocation, 1, glm::value_ptr(lightColor));
        }
        if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS) // Уменьшение красного
        {
            lightColor.r -= 1.0f * deltaTime;
            if (lightColor.r < 0.0f) lightColor.r = 0.0f;
            glUniform3fv(uLightColorLocation, 1, glm::value_ptr(lightColor));
        }
        if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS) // Увеличение зелёного
        {
            lightColor.g += 1.0f * deltaTime;
            if (lightColor.g > 1.0f) lightColor.g = 1.0f;
            glUniform3fv(uLightColorLocation, 1, glm::value_ptr(lightColor));
        }
        if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS) // Уменьшение зелёного
        {
            lightColor.g -= 1.0f * deltaTime;
            if (lightColor.g < 0.0f) lightColor.g = 0.0f;
            glUniform3fv(uLightColorLocation, 1, glm::value_ptr(lightColor));
        }
        if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS) // Увеличение синего
        {
            lightColor.b += 1.0f * deltaTime;
            if (lightColor.b > 1.0f) lightColor.b = 1.0f;
            glUniform3fv(uLightColorLocation, 1, glm::value_ptr(lightColor));
        }
        if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) // Уменьшение синего
        {
            lightColor.b -= 1.0f * deltaTime;
            if (lightColor.b < 0.0f) lightColor.b = 0.0f;
            glUniform3fv(uLightColorLocation, 1, glm::value_ptr(lightColor));
        }

        // Управление материалом: коэффициент бликов (shininess)
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) // Увеличение shininess
        {
            materialShininess += 50.0f * deltaTime;
            if (materialShininess > 256.0f) materialShininess = 256.0f;
            glUniform1f(uMaterialShininessLocation, materialShininess);
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) // Уменьшение shininess
        {
            materialShininess -= 50.0f * deltaTime;
            if (materialShininess < 1.0f) materialShininess = 1.0f;
            glUniform1f(uMaterialShininessLocation, materialShininess);
        }

        // Обновление угла вращения камеры
        cameraOrbitAngle += cameraOrbitSpeed * deltaTime;
        if (cameraOrbitAngle >= 360.0f)
            cameraOrbitAngle -= 360.0f;

        // Вычисление позиции камеры на основе угла вращения и радиуса
        float rad = glm::radians(cameraOrbitAngle);
        glm::vec3 cameraPos = glm::vec3(cameraRadius * cos(rad), 2.0f, cameraRadius * sin(rad)); // Смещение по Y для лучшей видимости

        // Обновление позиции источника света для точечного света
        lightPos = cameraPos;
        lightDir = glm::normalize(glm::vec3(0.0f) - cameraPos); // Направление к центру
        glUniform3fv(uLightPosLocation, 1, glm::value_ptr(lightPos));
        glUniform3fv(uLightDirLocation, 1, glm::value_ptr(lightDir));

        // Матрица проекции (перспективная) с обновлённым FOV
        glm::mat4 projection = glm::perspective(glm::radians(fov),
                                                800.0f / 600.0f,
                                                0.1f,
                                                100.0f);

        // Матрица вида
        glm::mat4 viewMat = glm::lookAt(cameraPos,
                                       glm::vec3(0.0f, 0.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f));

        // Матрица модели (в данном случае единичная, так как куб статичен)
        glm::mat4 model = glm::mat4(1.0f);

        // Матрица нормалей (обратная транспонированная матрица модели)
        glm::mat4 normalMatrix = glm::transpose(glm::inverse(model));

        // Создание матрицы MVP
        glm::mat4 mvp = projection * viewMat * model;

        // Передача матриц в шейдер
        glUniformMatrix4fv(uMVPLocation, 1, GL_FALSE, glm::value_ptr(mvp));
        glUniformMatrix4fv(uModelLocation, 1, GL_FALSE, glm::value_ptr(model));
        glUniformMatrix4fv(uNormalMatrixLocation, 1, GL_FALSE, glm::value_ptr(normalMatrix));

        // Передача позиции камеры в шейдер
        glUniform3fv(uViewPosLocation, 1, glm::value_ptr(cameraPos));

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
