#include "include/ThermistorNTC.h"

#define KELVIN_TEMPERATURE_AT_25_CELSIUS 298.15
#define KELVIN_TO_CELSIUS_VALUE 273.15

struct ThermistorNTC {
    ADC_IT *adc;
    double thermistorResistance;
    double powerSupplyVoltage;
    double bCoefficient;
    uint16_t adcResolution;
};

static inline uint16_t resolveADCResolution(ADC_TypeDef *ADCx);


ThermistorNTC initThermistor(ADC_TypeDef *ADCx, TIM_TypeDef *TIMx, uint32_t channel, double thermistorResistance, double powerSupplyVoltage, double bCoefficient) {
    ThermistorNTC thermistor = malloc(sizeof(struct ThermistorNTC));
    if (thermistor == NULL) return NULL;

    ADC_IT *adc = initRegularADC_IT(ADCx, LL_ADC_REG_RANK_1, channel);
    if (adc == NULL) {
        deleteThermistor(thermistor);
        return NULL;
    }
    thermistor->adc = adc;
    thermistor->thermistorResistance = thermistorResistance;
    thermistor->powerSupplyVoltage = powerSupplyVoltage;
    thermistor->bCoefficient = bCoefficient;
    thermistor->adcResolution = resolveADCResolution(ADCx);

    startADC_IT(adc->ADCx, ADC_REGULAR_CHANNEL);
    LL_TIM_EnableCounter(TIMx);    // start timer when timer trigger enabled
    return thermistor;
}

ThermistorNTC initPredefinedThermistor(ADC_TypeDef *ADCx, TIM_TypeDef *TIMx, uint32_t channel) {
    return initThermistor(ADCx, TIMx, channel, NTC_RESISTANCE, NTC_SUPPLY_VOLTAGE, NTC_B_COEFFICIENT);
}

void selectThermistorADC(ThermistorNTC thermistor) {
    selectChannelADC_IT(thermistor->adc);
}

double readThermistorVoltage(ThermistorNTC thermistor) {
    return thermistor->adc->value * (thermistor->powerSupplyVoltage / thermistor->adcResolution);
}

double readThermistorResistance(ThermistorNTC thermistor) {
    double voltageDividerOutput = readThermistorVoltage(thermistor);
    return (voltageDividerOutput * thermistor->thermistorResistance) / (thermistor->powerSupplyVoltage - voltageDividerOutput);
}

double readThermistorKelvin(ThermistorNTC thermistor) {
    double measuredResistance = readThermistorResistance(thermistor);
    return (KELVIN_TEMPERATURE_AT_25_CELSIUS * thermistor->bCoefficient) /
           (KELVIN_TEMPERATURE_AT_25_CELSIUS * log(measuredResistance / thermistor->thermistorResistance) + thermistor->bCoefficient);
}

double readThermistorCelsius(ThermistorNTC thermistor) {
    return readThermistorKelvin(thermistor) - KELVIN_TO_CELSIUS_VALUE;
}

int32_t readThermistorCelsiusAsInt(ThermistorNTC thermistor) {
    double temperature = readThermistorCelsius(thermistor);
    return (int32_t) round(temperature);
}

void deleteThermistor(ThermistorNTC thermistor) {
    deleteADC_IT();
    free(thermistor);
}

static inline uint16_t resolveADCResolution(ADC_TypeDef *ADCx) {
    uint16_t resolution;
    switch (LL_ADC_GetResolution(ADCx)) {
        case LL_ADC_RESOLUTION_12B:
            resolution = 4095;
            break;
        case LL_ADC_RESOLUTION_10B:
            resolution = 1023;
            break;
        case LL_ADC_RESOLUTION_8B:
            resolution = 255;
            break;
        case LL_ADC_RESOLUTION_6B:
            resolution = 63;
            break;
        default:
            resolution = 0;
    }
    return resolution;
}