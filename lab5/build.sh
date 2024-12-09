#!/bin/bash

# Имя исполняемого файла
OUTPUT="raytrace_app"

# Путь к исходному файлу
SOURCE="main.cpp"

# Компилятор
CXX=g++

# Флаги компиляции
CXXFLAGS="-std=c++11 -Wall -O2 `pkg-config --cflags glew glfw3`"

# Линковка библиотек
LIBS="`pkg-config --libs glew glfw3` -lGL -lm"

# Компиляция
echo "Компилируем $SOURCE..."
g++ $CXXFLAGS $SOURCE -o $OUTPUT $LIBS

# Проверяем успешность компиляции
if [ $? -eq 0 ]; then
    echo "Успешно скомпилировано: $OUTPUT"
    echo "Запуск приложения..."
    ./$OUTPUT
else
    echo "Ошибка компиляции!"
fi
