/*
 * stm32_us_timer.hpp
 *
 * Copyright (c) 2022 Oğuz Toraman oguz.toraman@protonmail.com
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#ifndef STM32_US_TIMER_HPP
#define STM32_US_TIMER_HPP

#include <cstdint>
#include <stm32f4xx_hal_tim.h>

#if !defined(HAL_TIM_MODULE_ENABLED) /* module check */
static_assert(false, "HAL TIM module is not enabled!");
#endif /* module check */

static_assert(__cplusplus >= 201103L, "C++11 required!");

namespace stm32 {

class us_timer {
public:
	us_timer(TIM_HandleTypeDef& timer) noexcept
	: m_timer{&timer}
	{
		HAL_TIM_Base_Start(m_timer);
	}

	us_timer(const us_timer&) = delete;
	us_timer& operator=(const us_timer&) = delete;
	us_timer(us_timer&&) = delete;
	us_timer& operator=(us_timer&&) = delete;

	~us_timer()
	{
		HAL_TIM_Base_Stop(m_timer);
	}

	void set_us(std::uint32_t us) noexcept
	{
		__HAL_TIM_SET_COUNTER(m_timer, us);
	}

	[[nodiscard]]
	std::uint32_t get_us() const noexcept
	{
		return __HAL_TIM_GET_COUNTER(m_timer);
	}

	void sleep_for_us(std::uint32_t us) noexcept
	{
		reset();
		while(get_us() < us);
	}

	void reset() noexcept
	{
		set_us(0);
	}

private:
	TIM_HandleTypeDef* m_timer;
};

/*
 * TIM5 specs for STM32F4-DISC;
 * timer frequency must be 1'000'000 Hz;
 * peripheral_frequency     = 84'000'000;
 * prescaler                = 83;
 * timer_frequency          = 1'000'000;
 * counter_period = 4'294'967'295;
 */

} /* namespace stm32 */

#endif /* STM32_US_TIMER_HPP */
