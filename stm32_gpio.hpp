/*
 * stm32_gpio.hpp
 *
 * Copyright (c) 2022 Oğuz Toraman oguz.toraman@protonmail.com
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#ifndef STM32_GPIO_HPP
#define STM32_GPIO_HPP

#include <cstdint>
#include <stm32f4xx_hal.h>

#if !defined(HAL_GPIO_MODULE_ENABLED) /* module check */
static_assert(false, "HAL GPIO module is not enabled!");
#endif /* module check */

static_assert(__cplusplus >= 201703L, "C++17 required!");

namespace stm32 {

inline constexpr auto Low = GPIO_PinState::GPIO_PIN_RESET;
inline constexpr auto High = GPIO_PinState::GPIO_PIN_SET;

class gpio_base {
public:
	gpio_base(GPIO_TypeDef* gpio, std::uint16_t pin) noexcept
	: m_gpio{gpio},
	  m_pin{pin}
	{ }

	gpio_base(const gpio_base&) = delete;
	gpio_base& operator=(const gpio_base&) = delete;
	gpio_base(gpio_base&&) = delete;
	gpio_base& operator=(gpio_base&&) = delete;

	[[nodiscard]]
	GPIO_TypeDef* get_gpio() const noexcept
	{
		return m_gpio;
	}

	[[nodiscard]]
	std::uint16_t get_pin() const noexcept
	{
		return m_pin;
	}

protected:
	GPIO_TypeDef* m_gpio;
	std::uint16_t m_pin;
};

struct gpio_input : gpio_base {
	gpio_input(GPIO_TypeDef* gpio, std::uint16_t pin) noexcept
	: gpio_base(gpio, pin)
	{ }

	[[nodiscard]]
	GPIO_PinState read() const noexcept
	{
		return HAL_GPIO_ReadPin(gpio_base::m_gpio, gpio_base::m_pin);
	}
};

struct gpio_output : gpio_base {
	gpio_output(GPIO_TypeDef* gpio, std::uint16_t pin) noexcept
	: gpio_base(gpio, pin)
	{ }

	void write(GPIO_PinState state) noexcept
	{
		HAL_GPIO_WritePin(gpio_base::m_gpio, gpio_base::m_pin, state);
	}

	void toggle() noexcept
	{
		HAL_GPIO_TogglePin(gpio_base::m_gpio, gpio_base::m_pin);
	}
};

#if defined(STM32F407xx)
inline gpio_output green_led{GPIOD, GPIO_PIN_12};
inline gpio_output orange_led{GPIOD, GPIO_PIN_13};
inline gpio_output red_led{GPIOD, GPIO_PIN_14};
inline gpio_output blue_led{GPIOD, GPIO_PIN_15};
#endif /* STM32F407xx */

/*
 * Examples;
 *
 * stm32::gpio_output led(GPIOA, GPIO_PIN_9);
 * led.write(stm32::High);
 *
 */

} /* namespace stm32 */

#endif /* STM32_GPIO_HPP */
