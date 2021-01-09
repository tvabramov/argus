# ARGUS

## Описание проекта
ARGUS является и интерактивным консольным приложением для работы с изображениями.

## Технологии, библиотеки и зависимости
Проект написан на C++14 с использованием OpenCV и headers-only части библиотеки Boost.

## Сборка под Linux
```bash
# Скачиваем ARGUS из репозитория
user@user:~$ git clone https://github.com/tvabramov/argus.git
user@user:~$ cd argus
# Собираем
user@user:~/argus$ mkdir build && cd build
user@user:~/argus/build$ cmake -D CMAKE_INSTALL_PREFIX=./ .. 
user@user:~/argus/build$ make install
```

## Сборка под Windows
Происходит аналогично сборке под Linux. Рекомендуется использовать тулчейн [mingw-w64](http://mingw-w64.org/doku.php), выбирать сборку с posix потоками, например "x86_64-8.1.0-posix-seh-rt_v6-rev0".
```cmd
C:\Users\User>git clone https://github.com/tvabramov/argus.git
C:\Users\User>cd argus
C:\Users\User\argus>mkdir build && cd build
C:\Users\User\argus\build>cmake -G "MinGW Makefiles" ^
  -D "OpenCV_DIR"="C:/OpenCV/install" ^
  -DCMAKE_INSTALL_PREFIX=./ ^
  ..
C:\Users\User\argus\build>mingw32-make install
```

## Использование
Просто запустите исполняемый файл:
```bash
user@user:~/argus/build/bin$ ./argus
```
После этого Вы окажетесь в командной оболочке ARGUS, где сможете вводить команды, напрмер "h" для помощи.

## Автор
* **Тимофей Абрамов** - *[timohamail@inbox.ru](mailto://timohamail@inbox.ru)*.
