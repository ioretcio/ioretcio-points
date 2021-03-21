# Пошук особливих точок на базі мікрокомп'ютера RaspberryPi4
```
git clone https://github.com/yuriy-goncharuk/ioretcio-points/
cd ioretcio-points
```
Для встановлення opencv v4 використовується скрипт користувача  spmallick [link]( https://github.com/spmallick). 
[script origin](https://github.com/spmallick/learnopencv/blob/master/InstallScripts/installOpenCV-4-raspberry-pi.sh)
```
cd Setup
chmod +x installOpenCV-4-raspberry-pi.sh
./installOpenCV-4-raspberry-pi.sh
```
Попередня процедура займає близько 90 хвилин
Для запуску додатку необхідно відрадагувати ваш фалй



```
cd $HOME
cd ioretcio-points
cmake .
make
./program.out
```
