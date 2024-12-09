#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>

const char* vertexShaderSource = R"(
    #version 330 core
    layout(location = 0) in vec2 aPos;

    out vec2 fragCoord;

    void main()
    {
        fragCoord = aPos;
        gl_Position = vec4(aPos, 0.0, 1.0);
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core

    out vec4 FragColor;
    in vec2 fragCoord;

    // Структура для материала
    struct Material {
        vec3 ambient;
        vec3 diffuse;
        vec3 specular;
        float shininess;
        float reflection; 
    };

    // Структура для сферы
    struct Sphere {
        vec3 center;
        float radius;
        Material material;
    };

    // Структура для плоскости
    struct Plane {
        vec3 point;
        vec3 normal;
        Material material;
    };

    // Структура для источника света
    struct Light {
        vec3 position;
        vec3 color;
    };

    // Униформы
    uniform vec3 uCameraPos;
    uniform Light uLight;

    // Униформы для сфер
    uniform Sphere uSpheres[2];
    // Униформ для плоскости
    uniform Plane uPlane;

    // Параметры окна
    uniform float uAspectRatio;
    uniform float uFOV;

    // Максимальная глубина итераций для отражений
    const int MAX_DEPTH = 5;

    // Класс для луча
    struct Ray {
        vec3 origin;
        vec3 direction;
    };

    // Функция для генерации луча через пиксель
    Ray generateRay(vec2 fragCoord, vec3 cameraPos, vec3 cameraDir, vec3 cameraRight, vec3 cameraUp, float fov, float aspectRatio)
    {
        float x = fragCoord.x;
        float y = fragCoord.y;

        float angle = tan(radians(fov) / 2.0);
        vec3 rayDir = normalize(cameraDir + cameraRight * (x * angle * aspectRatio) + cameraUp * (y * angle));

        Ray r;
        r.origin = cameraPos;
        r.direction = rayDir;
        return r;
    }

    // Функция для пересечения луча со сферой
    bool intersectSphere(Ray ray, Sphere sphere, out float t)
    {
        vec3 oc = ray.origin - sphere.center;
        float a = dot(ray.direction, ray.direction);
        float b = 2.0 * dot(oc, ray.direction);
        float c = dot(oc, oc) - sphere.radius * sphere.radius;
        float discriminant = b * b - 4.0 * a * c;
        if (discriminant < 0.0)
            return false;
        else {
            float sqrtDisc = sqrt(discriminant);
            float t0 = (-b - sqrtDisc) / (2.0 * a);
            float t1 = (-b + sqrtDisc) / (2.0 * a);
            t = (t0 < t1) ? t0 : t1;
            if (t < 0.0) {
                t = (t0 > t1) ? t0 : t1;
                if (t < 0.0)
                    return false;
            }
            return true;
        }
    }

    // Функция для пересечения луча с плоскостью
    bool intersectPlane(Ray ray, Plane plane, out float t)
    {
        float denom = dot(plane.normal, ray.direction);
        if (abs(denom) > 1e-6) { // Не параллельно
            t = dot(plane.point - ray.origin, plane.normal) / denom;
            return (t >= 0.0);
        }
        return false;
    }

    // Функция для получения цвета из материала
    vec3 getColor(Material mat, vec3 hitPoint, vec3 normal, vec3 viewDir, Light light, bool inShadow)
    {
        // Ambient
        vec3 ambient = mat.ambient * light.color;

        // Diffuse
        vec3 diffuse = vec3(0.0);
        // Specular
        vec3 specular = vec3(0.0);

        if (!inShadow) {
            vec3 lightDir = normalize(light.position - hitPoint);
            float diff = max(dot(normal, lightDir), 0.0);
            diffuse = mat.diffuse * diff * light.color;

            // Specular
            vec3 reflectDir = reflect(-lightDir, normal);
            float spec = pow(max(dot(viewDir, reflectDir), 0.0), mat.shininess);
            specular = mat.specular * spec * light.color;
        }

        return ambient + diffuse + specular;
    }

    void main()
    {
        // Определение параметров камеры
        vec3 cameraDir = normalize(vec3(0.0, 1.0, -3.0) - uCameraPos);
        vec3 cameraRight = normalize(cross(cameraDir, vec3(0.0, 1.0, 0.0)));
        vec3 cameraUp = cross(cameraRight, cameraDir);

        // Генерация луча через пиксель
        Ray ray = generateRay(fragCoord, uCameraPos, cameraDir, cameraRight, cameraUp, uFOV, uAspectRatio);

        vec3 finalColor = vec3(0.0);
        Ray currentRay = ray;
        float currentReflection = 1.0;

        for (int depth = 0; depth < MAX_DEPTH; ++depth) {
            float tMin = 1e20;
            int hitObject = -1; // 0 или 1 для сфер, 2 для плоскости
            float t;

            // Проверка пересечения с сферами
            for (int i = 0; i < 2; ++i) {
                if (intersectSphere(currentRay, uSpheres[i], t)) {
                    if (t < tMin) {
                        tMin = t;
                        hitObject = i;
                    }
                }
            }

            // Проверка пересечения с плоскостью
            if (intersectPlane(currentRay, uPlane, t)) {
                if (t < tMin) {
                    tMin = t;
                    hitObject = 2;
                }
            }

            // Если ничего не пересекло, добавить цвет фона и выйти из цикла
            if (hitObject == -1) {
                finalColor += currentReflection * vec3(0.2, 0.7, 0.8); // Цвет неба
                break;
            }

            // Определение точки пересечения и нормали
            vec3 hitPoint;
            vec3 normal;
            Material material;

            if (hitObject == 0 || hitObject == 1) {
                Sphere sphere = uSpheres[hitObject];
                hitPoint = currentRay.origin + currentRay.direction * tMin;
                normal = normalize(hitPoint - sphere.center);
                material = sphere.material;
            }
            else {
                hitPoint = currentRay.origin + currentRay.direction * tMin;
                normal = uPlane.normal;
                material = uPlane.material;
            }

            vec3 viewDir = normalize(-currentRay.direction);

            // Проверка теней
            vec3 lightDir = normalize(uLight.position - hitPoint);
            Ray shadowRay;
            shadowRay.origin = hitPoint + normal * 1e-4;
            shadowRay.direction = lightDir;
            bool inShadow = false;

            // Проверка пересечений для теней
            for (int i = 0; i < 2; ++i) {
                if (i == hitObject)
                    continue;
                float tShadow;
                if (intersectSphere(shadowRay, uSpheres[i], tShadow)) {
                    inShadow = true;
                    break;
                }
            }

            if (!inShadow && hitObject != 2) {
                float tShadow;
                if (intersectPlane(shadowRay, uPlane, tShadow)) {
                    inShadow = true;
                }
            }

            vec3 color = getColor(material, hitPoint, normal, viewDir, uLight, inShadow);

            // Добавление цвета с учётом текущего отражения
            finalColor += currentReflection * color;

            // Обработка отражения
            if (material.reflection > 0.0) {
                vec3 reflectDir = reflect(currentRay.direction, normal);
                currentRay.origin = hitPoint + reflectDir * 1e-4;
                currentRay.direction = reflectDir;
                currentReflection *= material.reflection;
            }
            else {
                break; // Если материал не отражает, выйти из цикла
            }
        }

        // Применение гамма-коррекции
        finalColor = pow(finalColor, vec3(1.0 / 2.2));

        FragColor = vec4(finalColor, 1.0);
    }
)";

GLuint compileShader(GLenum type, const char* source) {
    GLuint shader = glCreateShader(type);
    if (shader == 0) {
        std::cerr << "Ошибка создания шейдера!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);

    GLint compiled;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);
    if (!compiled) {
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

GLuint createProgram(const char* vertexSource, const char* fragmentSource) {
    GLuint vertexShader = compileShader(GL_VERTEX_SHADER, vertexSource);
    GLuint fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentSource);

    GLuint program = glCreateProgram();
    if (program == 0) {
        std::cerr << "Ошибка создания программы шейдеров!" << std::endl;
        exit(EXIT_FAILURE);
    }

    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);

    GLint linked;
    glGetProgramiv(program, GL_LINK_STATUS, &linked);
    if (!linked) {
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

struct Material {
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;
    float reflection;
};

struct Sphere {
    glm::vec3 center;
    float radius;
    Material material;
};

struct Plane {
    glm::vec3 point;
    glm::vec3 normal;
    Material material;
};

struct Light {
    glm::vec3 position;
    glm::vec3 color;
};

int main() {
    if (!glfwInit()) {
        std::cerr << "Не удалось инициализировать GLFW" << std::endl;
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Ray Tracing", nullptr, nullptr);
    if (!window) {
        std::cerr << "Не удалось создать окно GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    // Установка текущего контекста
    glfwMakeContextCurrent(window);
    glfwSwapInterval(1); // Включение вертикальной синхронизации

    // Инициализация GLEW
    glewExperimental = GL_TRUE;
    GLenum glewInitResult = glewInit();
    if (glewInitResult != GLEW_OK) {
        std::cerr << "Не удалось инициализировать GLEW: " << glewGetErrorString(glewInitResult) << std::endl;
        glfwDestroyWindow(window);
        glfwTerminate();
        return -1;
    }

    // Установка размеров области просмотра
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);

    GLuint shaderProgram = createProgram(vertexShaderSource, fragmentShaderSource);

    // Создание полноэкранного квадрата (fullscreen quad)
    float quadVertices[] = {
        // Positions
        -1.0f,  1.0f, // Top-left
        -1.0f, -1.0f, // Bottom-left
         1.0f, -1.0f, // Bottom-right

        -1.0f,  1.0f, // Top-left
         1.0f, -1.0f, // Bottom-right
         1.0f,  1.0f  // Top-right
    };

    GLuint VBO, VAO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    // Привязка VAO и VBO
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

    // Настройка атрибута позиции
    glEnableVertexAttribArray(0); // location = 0
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);

    // Разрыв привязки VAO
    glBindVertexArray(0);

    // Получение местоположений униформов
    GLint cameraPosLoc = glGetUniformLocation(shaderProgram, "uCameraPos");
    GLint lightPosLoc = glGetUniformLocation(shaderProgram, "uLight.position");
    GLint lightColorLoc = glGetUniformLocation(shaderProgram, "uLight.color");
    GLint aspectRatioLoc = glGetUniformLocation(shaderProgram, "uAspectRatio");
    GLint fovLoc = glGetUniformLocation(shaderProgram, "uFOV");

    GLint sphereCenterLoc[2];
    GLint sphereRadiusLoc[2];
    GLint sphereAmbientLoc[2];
    GLint sphereDiffuseLoc[2];
    GLint sphereSpecularLoc[2];
    GLint sphereShininessLoc[2];
    GLint sphereReflectionLoc[2];

    for (int i = 0; i < 2; ++i) {
        std::string index = std::to_string(i);
        sphereCenterLoc[i] = glGetUniformLocation(shaderProgram, ("uSpheres[" + index + "].center").c_str());
        sphereRadiusLoc[i] = glGetUniformLocation(shaderProgram, ("uSpheres[" + index + "].radius").c_str());
        sphereAmbientLoc[i] = glGetUniformLocation(shaderProgram, ("uSpheres[" + index + "].material.ambient").c_str());
        sphereDiffuseLoc[i] = glGetUniformLocation(shaderProgram, ("uSpheres[" + index + "].material.diffuse").c_str());
        sphereSpecularLoc[i] = glGetUniformLocation(shaderProgram, ("uSpheres[" + index + "].material.specular").c_str());
        sphereShininessLoc[i] = glGetUniformLocation(shaderProgram, ("uSpheres[" + index + "].material.shininess").c_str());
        sphereReflectionLoc[i] = glGetUniformLocation(shaderProgram, ("uSpheres[" + index + "].material.reflection").c_str());
    }

    GLint planePointLoc = glGetUniformLocation(shaderProgram, "uPlane.point");
    GLint planeNormalLoc = glGetUniformLocation(shaderProgram, "uPlane.normal");
    GLint planeAmbientLoc = glGetUniformLocation(shaderProgram, "uPlane.material.ambient");
    GLint planeDiffuseLoc = glGetUniformLocation(shaderProgram, "uPlane.material.diffuse");
    GLint planeSpecularLoc = glGetUniformLocation(shaderProgram, "uPlane.material.specular");
    GLint planeShininessLoc = glGetUniformLocation(shaderProgram, "uPlane.material.shininess");
    GLint planeReflectionLoc = glGetUniformLocation(shaderProgram, "uPlane.material.reflection");

    // Параметры сцены
    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, 5.0f);
    Light light;
    light.position = glm::vec3(2.0f, 4.0f, 2.0f);
    light.color = glm::vec3(1.0f, 1.0f, 1.0f);
    Sphere spheres[2];

    // Первая сфера
    spheres[0].center = glm::vec3(-1.0f, 1.0f, -3.0f);
    spheres[0].radius = 1.0f;
    spheres[0].material.ambient = glm::vec3(0.1f, 0.0f, 0.0f);
    spheres[0].material.diffuse = glm::vec3(0.6f, 0.0f, 0.0f);
    spheres[0].material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    spheres[0].material.shininess = 32.0f;
    spheres[0].material.reflection = 0.5f;

    // Вторая сфера
    spheres[1].center = glm::vec3(1.0f, 1.0f, -4.0f);
    spheres[1].radius = 1.0f;
    spheres[1].material.ambient = glm::vec3(0.0f, 0.0f, 0.1f);
    spheres[1].material.diffuse = glm::vec3(0.0f, 0.0f, 0.6f);
    spheres[1].material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    spheres[1].material.shininess = 32.0f;
    spheres[1].material.reflection = 0.5f;

    // Создание плоскости
    Plane plane;
    plane.point = glm::vec3(0.0f, 0.0f, 0.0f);
    plane.normal = glm::vec3(0.0f, 1.0f, 0.0f);
    plane.material.ambient = glm::vec3(0.1f, 0.1f, 0.1f);
    plane.material.diffuse = glm::vec3(0.6f, 0.6f, 0.6f);
    plane.material.specular = glm::vec3(0.5f, 0.5f, 0.5f);
    plane.material.shininess = 32.0f;
    plane.material.reflection = 0.3f;

    glUseProgram(shaderProgram);
    glUniform3fv(cameraPosLoc, 1, glm::value_ptr(cameraPos));
    glUniform3fv(lightPosLoc, 1, glm::value_ptr(light.position));
    glUniform3fv(lightColorLoc, 1, glm::value_ptr(light.color));
    glUniform1f(aspectRatioLoc, static_cast<float>(width) / static_cast<float>(height));
    glUniform1f(fovLoc, 45.0f);

    for (int i = 0; i < 2; ++i) {
        glUniform3fv(sphereCenterLoc[i], 1, glm::value_ptr(spheres[i].center));
        glUniform1f(sphereRadiusLoc[i], spheres[i].radius);
        glUniform3fv(sphereAmbientLoc[i], 1, glm::value_ptr(spheres[i].material.ambient));
        glUniform3fv(sphereDiffuseLoc[i], 1, glm::value_ptr(spheres[i].material.diffuse));
        glUniform3fv(sphereSpecularLoc[i], 1, glm::value_ptr(spheres[i].material.specular));
        glUniform1f(sphereShininessLoc[i], spheres[i].material.shininess);
        glUniform1f(sphereReflectionLoc[i], spheres[i].material.reflection);
    }
    glUniform3fv(planePointLoc, 1, glm::value_ptr(plane.point));
    glUniform3fv(planeNormalLoc, 1, glm::value_ptr(plane.normal));
    glUniform3fv(planeAmbientLoc, 1, glm::value_ptr(plane.material.ambient));
    glUniform3fv(planeDiffuseLoc, 1, glm::value_ptr(plane.material.diffuse));
    glUniform3fv(planeSpecularLoc, 1, glm::value_ptr(plane.material.specular));
    glUniform1f(planeShininessLoc, plane.material.shininess);
    glUniform1f(planeReflectionLoc, plane.material.reflection);

    // Основной цикл рендеринга
    while (!glfwWindowShouldClose(window)) {
        // Обработка ввода с клавиатуры
        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
            glfwSetWindowShouldClose(window, true);

        // Изменение коэффициента отражения сфер
        if (glfwGetKey(window, GLFW_KEY_1) == GLFW_PRESS) {
            spheres[0].material.reflection += 0.5f * 0.016f;
            spheres[0].material.reflection = glm::clamp(spheres[0].material.reflection, 0.0f, 1.0f);
            spheres[1].material.reflection = spheres[0].material.reflection;
            for (int i = 0; i < 2; ++i) {
                glUniform1f(sphereReflectionLoc[i], spheres[i].material.reflection);
            }
            std::cout << "Коэффициент отражения сфер: " << spheres[0].material.reflection << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_2) == GLFW_PRESS) {
            spheres[0].material.reflection -= 0.5f * 0.016f;
            spheres[0].material.reflection = glm::clamp(spheres[0].material.reflection, 0.0f, 1.0f);
            spheres[1].material.reflection = spheres[0].material.reflection;
            for (int i = 0; i < 2; ++i) {
                glUniform1f(sphereReflectionLoc[i], spheres[i].material.reflection);
            }
            std::cout << "Коэффициент отражения сфер: " << spheres[0].material.reflection << std::endl;
        }

        // Изменение коэффициента отражения пола
        if (glfwGetKey(window, GLFW_KEY_3) == GLFW_PRESS) {
            plane.material.reflection += 0.3f * 0.016f;
            plane.material.reflection = glm::clamp(plane.material.reflection, 0.0f, 1.0f);
            glUniform1f(planeReflectionLoc, plane.material.reflection);
            std::cout << "Коэффициент отражения пола: " << plane.material.reflection << std::endl;
        }
        if (glfwGetKey(window, GLFW_KEY_4) == GLFW_PRESS) {
            plane.material.reflection -= 0.3f * 0.016f;
            plane.material.reflection = glm::clamp(plane.material.reflection, 0.0f, 1.0f);
            glUniform1f(planeReflectionLoc, plane.material.reflection);
            std::cout << "Коэффициент отражения пола: " << plane.material.reflection << std::endl;
        }

        glfwGetFramebufferSize(window, &width, &height);
        glViewport(0, 0, width, height);
        glUniform1f(aspectRatioLoc, static_cast<float>(width) / static_cast<float>(height));

        // Очистка буферов
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Отрисовка полноэкранного квадрата
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);

        // Обмен буферов и обработка событий
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Очистка ресурсов
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    // Завершение работы GLFW
    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}
