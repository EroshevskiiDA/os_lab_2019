#!/bin/bash

# Инициализация суммы и счетчика
sum=0
count=0

# Чтение чисел из файла numbers.txt
while read -r number; do
  # Проверка, является ли числом
  if [[ "$number" =~ ^-?[0-9]+([.][0-9]+)?$ ]]; then
    # Увеличиваем счетчик и добавляем число к сумме
    count=$((count + 1))
    sum=$((sum + number))
  else
    echo "Ошибка: '$number' не является числом."
  fi
done < numbers.txt

# Вывод результата
if [[ $count -gt 0 ]]; then
  average=$(echo "scale=2; $sum / $count" | bc)
  echo "Количество аргументов: $count"
  echo "Среднее арифметическое: $average"
fi
