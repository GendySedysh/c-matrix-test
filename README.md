# Тестовое задание Робопро

## Описание
---
Реализовать клиент-серверное приложение на языке C со следующим функционалом:
- Клиент - по нажатию клавиши на клавиатуре или по таймеру высылает на сервер матрицу размером 6x6 со случайными значениями типа double.
- Сервер - при получении матрицы от клиента вычисляет и выводит в консоль следующие значения:
- Определитель принятой матрицы
- Среднее арифметическое значение определителя среди последних 5 полученных матриц (если принято менее 5 матриц - выводить "N/A")
- Определитель матрицы, полученной 5 матриц назад (если принято менее 5 матриц, выводить "N/A")

## Сборка
---
Тестирование происходило на WSL2 с Ubuntu.

Порядок сборки (из коренной папки репозитория):
```
mkdir build && cd build
cmake ..
cmake --build .
```

## Запуск
---

Старт сервера (из папки build):
`./Server <port>`, где `port` это номер порта на котором будем ожидать присоеденения.

Пример:
`./Server 4444`

Запуск клиента (из папки build):
`./Client <port> (time)`, где 
- `port` это номер порта на котором будем ожидать присоеденения.
- `(time)` необязательный числовой аргумент который указывает раз в какое время (в секундах), клиент будет отправлять матрицу серверу.

Пример:
`./Client 4444 1` или `./Client 4444`

## Использование
---

Сначала запускаем сервер, он работает в фоновом режиме и не требует никаких доболнительных действий

Далее запускаем клиент, тут у нас есть два варианта:
- Запуск клиента с отправкой матрицы по времени: `./Client <port> (time)`
- Запуск клиента с отправкой матрицы по нажатию клавиши `enter`: `./Client <port>`

Прервать работу клиента и сервера можно комбинацией клавиш `Ctrl + c`.
Если клиент запущен в режиме отправки по нажатию клавиши `enter`, то для прерывания работы после `Ctrl + c` нужно нажать `enter`.
