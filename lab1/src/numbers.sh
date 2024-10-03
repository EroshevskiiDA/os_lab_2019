#!/bin/bash

# Генерируем 150 случайных чисел и записываем в numbers.txt
for i in {1..150}; do
  # Получаем случайное число из /dev/random
  random_number=$(tr -dc 0-9 < /dev/random | head -c 1)
  # Записываем число в файл
  echo $random_number >> numbers.txt
done
