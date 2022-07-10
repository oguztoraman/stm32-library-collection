# STM32

Some useful header-only C++ libraries for STM32 micro-controllers based on STM32 HAL drivers.

## Requirements

+ C++20
+ STM32CubeIDE

## How to use these libraries on STM32CubeIDE

1. Install STM32CubeIDE version 1.9 or later.
2. Open an STM32 project.
3. Select the programming language as C++.
4. Set up the C++ standard;
``` 
Project => Properties => C/C++ Build => Settings => 
MCU G++ Compiler => General => Tool Settings => 
Language Standard => ISO C++20 
```
5. Copy the header file you want to use, along with its dependencies, to the Core/Inc folder.
6. Rename the automatically generated main.c file to main.cpp.

## License

BSD 3-Clause License.