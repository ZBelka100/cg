#!/bin/bash

# Устанавливаем флаг для остановки при ошибках
set -e

# Имя выходного исполняемого файла
OUTPUT="colorful_cube_with_normals"

# Файл исходного кода
SOURCE="main.cpp"

# Проверяем, существует ли исходный файл
if [ ! -f "$SOURCE" ]; then
    echo "Исходный файл $SOURCE не найден!"
    exit 1
fi

# Компиляция с использованием g++
echo "Компиляция $SOURCE..."
g++ -o $OUTPUT $SOURCE $(pkg-config --cflags --libs glfw3) -lGLESv2 -lm

# Проверка успешности компиляции
if [ $? -eq 0 ]; then
    echo "Компиляция прошла успешно. Запустите программу с помощью ./${OUTPUT}"
else
    echo "Компиляция завершилась с ошибками."
    exit 1
fi
