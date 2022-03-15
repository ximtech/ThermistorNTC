# ThermistorNTC

STM32 LL(Low-Layer) C library. Thermistor is short form of thermal resistor, whose resistance changes with change in temperature.

### Features

- Interrupt based implementation
- Auto ADC resolution detection
- Temperature to int value rounding
- No HAL dependencies

<img src="https://github.com/ximtech/ThermistorNTC/blob/main/example/view.PNG" alt="image" width="300"/>

### Add as CPM project dependency

How to add CPM to the project, check the [link](https://github.com/cpm-cmake/CPM.cmake)

```cmake
CPMAddPackage(
        NAME ThermistorNTC
        GITHUB_REPOSITORY ximtech/ThermistorNTC
        GIT_TAG origin/main)
```

### Project configuration

1. Start project with STM32CubeMX:
    * [ADC configuration](https://github.com/ximtech/ThermistorNTC/blob/main/example/config_1.PNG)
    * [ADC interrupt](https://github.com/ximtech/ThermistorNTC/blob/main/example/config_2.PNG)
    * [TIM configuration](https://github.com/ximtech/ThermistorNTC/blob/main/example/config_3.PNG)
2. Select: Project Manager -> Advanced Settings -> ADC -> LL
3. Generate Code
4. Add sources to project:
```cmake
add_subdirectory(${STM32_CORE_SOURCE_DIR}/ADC/IT)  # add I2C to project

include_directories(${THERMISTOR_NTC_DIRECTORY})   # source directories
file(GLOB_RECURSE SOURCES ${THERMISTOR_NTC_SOURCES})    # source files
```

3. Then Build -> Clean -> Rebuild Project

### Wiring

- <img src="https://github.com/ximtech/ThermistorNTC/blob/main/example/wiring.PNG" alt="image" width="300"/>

### Usage

***Provide interrupt handler in `stm32f4xx_it.c`***
```c
/**
  * @brief This function handles ADC1 global interrupt.
  */
void ADC_IRQHandler(void) {
   conventionCompleteCallbackADC(ADC1, ADC_REGULAR_CHANNEL);
}
```

***Define values at `main.h` or use defaults(see `ThermistorNTC.h`)***
```c
#define NTC_SUPPLY_VOLTAGE 3.275
#define NTC_B_COEFFICIENT 3950
#define NTC_RESISTANCE 9840
```

***Then predefined NTC instance can be created***
```c
#include "ThermistorNTC.h"

int main() {

   ThermistorNTC thermistor = initPredefinedThermistor(ADC1, TIM3, LL_ADC_CHANNEL_0);
      if (thermistor == NULL) {
         Error_Handler();
      }
      
      while(true) {
          
        int32_t temperature = readThermistorCelsiusAsInt(thermistor);
        printf("Temperature: %d\n", temperature);
      }
}
```

***Or create instance passing parameters***
```c
ThermistorNTC thermistor = initThermistor(ADC1, TIM3, LL_ADC_CHANNEL_0, NTC_RESISTANCE, NTC_SUPPLY_VOLTAGE, NTC_B_COEFFICIENT);
```
