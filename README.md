# Пошук особливих точок на базі мікрокомп'ютера RaspberryPi4
```
git clone https://github.com/ioretcio/ioretcio-points
cd ioretcio-points
```
Для встановлення opencv v4 використовується скрипт користувача  [spmallick]( https://github.com/spmallick)  
[script origin](https://github.com/spmallick/learnopencv/blob/master/InstallScripts/installOpenCV-4-raspberry-pi.sh)

*(Попередження- процедура займає близько 90 хвилин)*
```
cd Setup
chmod +x installOpenCV-4-raspberry-pi.sh
./installOpenCV-4-raspberry-pi.sh
```
Для запуску самого додатку необхідно відредагувати ваш файл **CMakeLists.txt**

Так щоб у ньому був вказаний коректний шлях до бібліотеки OpenCV, збудованої на попередньому етапі.
```
cd $HOME
cd ioretcio-points
nano CMakeLists.txt
```
Далі перейдемо до побудови застосунку та тестування:
```
cmake .
make
./program.out
```
