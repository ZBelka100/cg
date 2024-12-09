#include <GLES2/gl2.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp> 
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <cmath>
#include <cstdlib>
#include <iostream>
#include <vector>

// Вершины многоугольника (используется TRIANGLE_FAN)
std::vector<float> vertices;

// Параметры трансформации
glm::vec2 translation(0.0f, 0.0f);
float rotation = 0.0f;
float scale = 1.0f;

// Параметры для анимации по кругу
float animationRadius = 0.5f;
float animationSpeed = 50.0f; // Градусы в секунду
float currentAngle = 0.0f;

// Вершинный шейдер
const char* vertexShaderSource = R"(
    attribute vec2 aPos;
    uniform mat4 uTransform;

    void main()
    {
        gl_Position = uTransform * vec4(aPos, 0.0, 1.0);
    }
)";

// Фрагментный шейдер
const char* fragmentShaderSource = R"(
    precision mediump float;
    uniform vec4 uColor;

    void main()
    {
        gl_FragColor = uColor;
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

    // Проверка линковки
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
    int numSides;
    std::cout << "Введите количество сторон многоугольника (>=3): ";
    std::cin >> numSides;
    if(numSides < 3)
    {
        std::cout << "Будет 3." << std::endl;
        numSides = 3;
    }

    int animateOption;
    std::cout << "Хочешь, чтобы многоугольник двигался по кругу? (1 - Да, 0 - Нет): ";
    std::cin >> animateOption;
    bool animate = (animateOption == 1);

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

    // Создание окна
    GLFWwindow* window = glfwCreateWindow(800, 600, "Многоугольник с фокусами", nullptr, nullptr);
    if (!window)
    {
        std::cerr << "Не удалось создать окно GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Установка контекста текущим
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);

    // Компиляция и линковка шейдерной программы
    GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);
    glUseProgram(shaderProgram);

    // Получение местоположений атрибутов и униформов
    GLint aPosLocation = glGetAttribLocation(shaderProgram, "aPos");
    GLint uColorLocation = glGetUniformLocation(shaderProgram, "uColor");
    GLint uTransformLocation = glGetUniformLocation(shaderProgram, "uTransform");

    // Создание вершин многоугольника
    vertices.push_back(0.0f); // Центр
    vertices.push_back(0.0f);

    float angleStep = 2.0f * M_PI / numSides;
    for(int i = 0; i <= numSides; ++i) // Закрытие фигуры
    {
        float angle = angleStep * i;
        float x = cos(angle);
        float y = sin(angle);
        vertices.push_back(x);
        vertices.push_back(y);
    }

    // Создание VBO
    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

    // Настройка атрибута позиции
    glEnableVertexAttribArray(aPosLocation);
    glVertexAttribPointer(aPosLocation, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Установка цвета многоугольника (белый)
    glUniform4f(uColorLocation, 1.0f, 1.0f, 1.0f, 1.0f);

    // Параметры для анимации по кругу
    double lastTime = glfwGetTime();

    // Основной цикл рендеринга
    while (!glfwWindowShouldClose(window))
    {
        // Вычисление времени для анимации
        double currentTime = glfwGetTime();
        double deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Обработка ввода с клавиатуры
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }

        // Перемещение вручную (если анимация отключена)
        float moveSpeed = 0.5f * deltaTime;
        if (!animate)
        {
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS)
            {
                translation.x -= moveSpeed;
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS)
            {
                translation.x += moveSpeed;
            }
            if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
            {
                translation.y += moveSpeed;
            }
            if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
            {
                translation.y -= moveSpeed;
            }
        }

        // Вращение
        float rotateSpeed = 90.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) // Вращение влево
        {
            rotation += rotateSpeed;
        }
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) // Вращение вправо
        {
            rotation -= rotateSpeed;
        }

        // Масштабирование
        float scaleSpeed = 1.0f * deltaTime;
        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) // Увеличение
        {
            scale += scaleSpeed;
            if (scale > 3.0f) scale = 3.0f; // Ограничение максимального масштаба
        }
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) // Уменьшение
        {
            scale -= scaleSpeed;
            // Минимальное ограничение отсутствует
        }

        // Обновление позиции для анимации по кругу
        if (animate)
        {
            currentAngle += animationSpeed * deltaTime;
            if (currentAngle >= 360.0f)
                currentAngle -= 360.0f;

            float rad = glm::radians(currentAngle);
            translation.x = animationRadius * cos(rad);
            translation.y = animationRadius * sin(rad);
        }

        // Создание матрицы трансформации
        glm::mat4 transform = glm::mat4(1.0f);
        transform = glm::translate(transform, glm::vec3(translation, 0.0f));
        transform = glm::rotate(transform, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        transform = glm::scale(transform, glm::vec3(scale, scale, 1.0f));

        // Передача матрицы в шейдер
        glUniformMatrix4fv(uTransformLocation, 1, GL_FALSE, glm::value_ptr(transform));

        // Очистка экрана (черный фон)
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Рисование многоугольника
        glDrawArrays(GL_TRIANGLE_FAN, 0, numSides + 2); // +2: центр + numSides + повтор первой вершины

        // Обмен буферов и обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
