#pragma once

#include <math.h>
#include <stdlib.h>
#include "ADC_IT.h"

#ifndef NTC_SUPPLY_VOLTAGE
#define NTC_SUPPLY_VOLTAGE 3.275
#endif

#ifndef NTC_B_COEFFICIENT
#define NTC_B_COEFFICIENT 3950
#endif

#ifndef NTC_RESISTANCE
#define NTC_RESISTANCE 9840
#endif

typedef struct ThermistorNTC *ThermistorNTC;

ThermistorNTC initThermistor(ADC_TypeDef *ADCx, TIM_TypeDef *TIMx, uint32_t channel, double thermistorResistance, double powerSupplyVoltage, double bCoefficient);
ThermistorNTC initPredefinedThermistor(ADC_TypeDef *ADCx, TIM_TypeDef *TIMx, uint32_t channel);

void selectThermistorADC(ThermistorNTC thermistor);
double readThermistorVoltage(ThermistorNTC thermistor);
double readThermistorResistance(ThermistorNTC thermistor);
double readThermistorKelvin(ThermistorNTC thermistor);
double readThermistorCelsius(ThermistorNTC thermistor);
int32_t readThermistorCelsiusAsInt(ThermistorNTC thermistor);

void deleteThermistor(ThermistorNTC thermistor);