# STM32 Library Collection

Useful, header-only C++ library collection for STM32 microcontrollers.

## Requirements

+ Modern C++ (C++17, C++20)
+ STM32 HAL drivers
+ STM32CubeIDE

## How to use these libraries on STM32CubeIDE

1. Install STM32CubeIDE version 1.9 or later.
2. Open an STM32 project.
3. Select the programming language as C++.
4. Set up the C++ standard;
``` 
Project => Properties => C/C++ Build => Settings => 
Tool Settings => MCU G++ Compiler => General =>
Language Standard => ISO C++20 
```
5. Set up the runtime library;
``` 
Project => Properties => C/C++ Build => Settings => 
Tool Settings => MCU Settings => Runtime library => 
Standard C, standard C++
```
6. Copy the header file you want to use, along with its dependencies, to the *Core/Inc* folder of your STM32 project.
7. Rename the automatically generated *main.c* file to *main.cpp*.

## Notes

+ To prevent your old code from being deleted when generating new code, write your code between the *USER CODE BEGIN* and *USER CODE END* sections.
+ Rename the *main.cpp* file to *main.c* before making any changes to the *.ioc* file and generating new code. After generating new code, rename the automatically generated *main.c* file to *main.cpp*.

## License

Licensed under the GNU LGPL version 3. See the COPYING.LESSER file for details.

### To Do
 
+ [ ] I2C
+ [ ] MPU6050
+ [ ] SPI
+ [ ] I2S
+ [ ] CANBUS
+ [ ] 7-Segment Display
+ [ ] LCD
+ [ ] Multi-mode gpio
+ [ ] DS18B20
+ [ ] LM35
+ [ ] ADXL345
+ [ ] PID control
+ [ ] Test code
+ [ ] Use double as a non-type template parameter when supported by the compiler