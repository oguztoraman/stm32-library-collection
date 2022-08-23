/*
 * stm32_dac.hpp
 *
 * Copyright (c) 2022 Oğuz Toraman oguz.toraman@protonmail.com
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#ifndef STM32_DAC_HPP
#define STM32_DAC_HPP

#include <cstdint>
#include <stm32f4xx_hal.h>

#if !defined(HAL_DAC_MODULE_ENABLED) /* module check */
static_assert(false, "HAL DAC module is not enabled!");
#endif /* module check */

static_assert(__cplusplus >= 201703L, "C++17 required!");

namespace stm32 {

template <int MinOutput = 0, int MaxOutput = 100,
		unsigned int DacAlignment = DAC_ALIGN_12B_R>
class dac {
public:
	dac(DAC_HandleTypeDef& dac_handle,
		std::uint32_t dac_channel,
		double dac_resolution  = 4095.) noexcept
	: m_dac_handle{&dac_handle},
	  m_dac_channel{dac_channel},
	  m_dac_resolution{dac_resolution}
	{
		static_assert(
			0 <= MinOutput,
			"the minimum output cannot be negative!"
		);
		static_assert(
			MinOutput <= MaxOutput,
			"the minimum output cannot be greater than the maximum output!"
		);
		HAL_DAC_Start(m_dac_handle, m_dac_channel);
		set(MinOutput);
	}

	dac(const dac&) = delete;
	dac& operator=(const dac&) = delete;
	dac(dac&&) = delete;
	dac& operator=(dac&&) = delete;

	~dac()
	{
		HAL_DAC_Stop(m_dac_handle, m_dac_channel);
	}

	[[nodiscard]]
	DAC_HandleTypeDef* get_dac_handle() const noexcept
	{
		return m_dac_handle;
	}

	[[nodiscard]]
	std::uint32_t get_dac_channel() const noexcept
	{
		return m_dac_channel;
	}

	[[nodiscard]]
	unsigned int get_dac_alignment() const noexcept
	{
		return DacAlignment;
	}

	void set(double output) noexcept
	{
		if (output <= MinOutput){
			output = MinOutput;
		} else if (MaxOutput <= output){
			output = MaxOutput;
		}
		HAL_DAC_SetValue(
			m_dac_handle,
			m_dac_channel,
			DacAlignment,
			convert_to_dac(output)
		);
	}

private:
	DAC_HandleTypeDef* m_dac_handle;
	std::uint32_t m_dac_channel;
	double m_dac_resolution;

	static constexpr int convert_to_dac(double output) noexcept
	{
		return static_cast<int>(
			(output / (MaxOutput - MinOutput)) * m_dac_resolution
		);
	}
};

} /* namespace stm32 */

#endif /* STM32_DAC_HPP */
