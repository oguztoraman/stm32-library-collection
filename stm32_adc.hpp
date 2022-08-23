/*
 * stm32_adc.hpp
 *
 * Copyright (c) 2022 Oğuz Toraman oguz.toraman@protonmail.com
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#ifndef STM32_ADC_HPP
#define STM32_ADC_HPP

#include <array>
#include <cmath>
#include <algorithm>

#include <stm32f4xx_hal.h>

#if !defined(HAL_ADC_MODULE_ENABLED) /* module check */
static_assert(false, "HAL ADC module is not enabled!");
#endif /* module check */

static_assert(__cplusplus >= 201703L, "C++17 required!");

namespace stm32 {

template <int MinOutput = 0, int MaxOutput = 100, std::uint32_t MedianFilterSize = 1>
class adc {
public:
	adc(ADC_HandleTypeDef& adc_handle,
		std::uint32_t adc_timeout_ms = 5000,
		double adc_resolution = 4095.) noexcept
	: m_adc_handle{&adc_handle},
	  m_adc_timeout_ms{adc_timeout_ms},
	  m_adc_resolution{adc_resolution}
	{
		static_assert(
			0 <= MinOutput,
			"the minimum output cannot be negative!"
		);
		static_assert(
			MinOutput <= MaxOutput,
			"the minimum output cannot be greater than the maximum output!"
		);
		static_assert(
			1 <= MedianFilterSize,
			"the median filter size must be greater or equal to 1!"
		);
		static_assert(
			MedianFilterSize % 2 != 0,
			"the median filter size must be an odd number!"
		);
	}

	adc(const adc&) = delete;
	adc& operator=(const adc&) = delete;
	adc(adc&&) = delete;
	adc& operator=(adc&&) = delete;

	[[nodiscard]]
	ADC_HandleTypeDef* get_adc_handle() const noexcept
	{
		return m_adc_handle;
	}

	[[nodiscard]]
	double get() const noexcept
	{
		static std::array<double, MedianFilterSize> adc_values{};
		for (std::uint32_t i{}; i < MedianFilterSize; ++i){
			HAL_ADC_Start(m_adc_handle);
			HAL_ADC_PollForConversion(m_adc_handle , m_adc_timeout_ms);
			std::uint32_t input = HAL_ADC_GetValue(m_adc_handle);
			HAL_ADC_Stop(m_adc_handle);
			adc_values[i] = std::round(
				(input / m_adc_resolution) * (MaxOutput - MinOutput)
			) + MinOutput;
		}
		std::sort(std::begin(adc_values), std::end(adc_values));
		return adc_values[MedianFilterSize/2];
	}

private:
	ADC_HandleTypeDef* m_adc_handle;
	std::uint32_t m_adc_timeout_ms;
	double m_adc_resolution;
};

/**
  * Example;
  *
  * stm32::adc temperature_sensor(hadc1);
  * auto temperature = temperature_sensor.get();
  */

} /* namespace stm32 */

#endif /* STM32_ADC_HPP */
