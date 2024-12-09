#!/bin/bash

# Имя исполняемого файла
OUTPUT="camera_app"

# Путь к исходному файлу
SOURCE="main.cpp"

# Линковка библиотек
LIBS="-lGLESv2 -lglfw -lm"

# Компиляция
echo "Компилируем $SOURCE..."
g++ $SOURCE $LIBS -o $OUTPUT

# Проверяем успешность компиляции
if [ $? -eq 0 ]; then
    echo "Успешно скомпилировано: $OUTPUT"
    echo "Запуск приложения..."
    ./$OUTPUT
else
    echo "Ошибка компиляции!"
fi
