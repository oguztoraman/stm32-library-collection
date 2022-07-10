/*
 * stm32_servo.hpp
 *
 * Copyright (c) 2022 Oğuz Toraman oguz.toraman@protonmail.com
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#ifndef STM32_SERVO_HPP
#define STM32_SERVO_HPP

#include <cmath>
#include <cstdint>
#include <stm32f4xx_hal.h>
#include <stm32f4xx_hal_tim.h>

#if !defined(HAL_TIM_MODULE_ENABLED) /* module check */
static_assert(false, "HAL TIM module is not enabled!");
#endif /* module check */

static_assert(__cplusplus > 201703L, "C++20 required!");

namespace stm32 {
#if __cpp_nontype_template_args < 201911L
/* the compiler does not support double as a non-type template parameter */

/**
  * @brief A helper class to use double as a non-type template parameter
  *
  */
struct stm32_double {
    constexpr stm32_double(double value) noexcept
        : m_power_of_ten{
            [](double d){
                if (d < 0){
                	d *= -1;
                }
                std::int32_t exponent{};
                while (true){
                    auto difference = d - static_cast<int>(d);
                    if (difference == 0.0){
                        break;
                    }
                    d *= ten;
                    ++exponent;
                }
                return exponent;
            }(value)
        },
		m_value{static_cast<std::int32_t>(
			value * power(ten, m_power_of_ten)
        )}
    { }

    constexpr operator double() const noexcept
    {
        return m_value/power(ten, m_power_of_ten);
    }

    static constexpr double power(double base, double exponent) noexcept
    {
        while(exponent--){
            base *= base;
        }
        return base;
    }

    std::int32_t m_power_of_ten;
    std::int32_t m_value;
    static constexpr int ten = 10;
};

/**
  * @brief A servo class to facilitate the use of
  * 	   servo motors with STM32 development boards
  *
  * Note: A servo class object must be defined
  * 	  after all the initializing operations,
  * 	  for example, in the USER CODE 2 section
  */
template <int MinDegree, int MaxDegree, int DefaultDegree,
		  stm32_double PWMDutyMin, stm32_double PWMDutyMax>
#else
/* the compiler supports double as a non-type template parameter  */

template <int MinDegree, int MaxDegree, int DefaultDegree,
		  double PWMDutyMin, double PWMDutyMax>

#endif /* non-type template parameter check */
class servo {
public:
	static_assert(
		0 <= MinDegree,
		"the minimum degree cannot be negative!"
	);
	static_assert(
		MaxDegree <= 360,
		"the maximum degree cannot be greater than 360 degrees!"
	);
	static_assert(
		MinDegree <= DefaultDegree && DefaultDegree <= MaxDegree,
		"the default degree must be in the range between the minimum and the maximum degrees!"
	);
	static_assert(
		0 <= PWMDutyMin,
		"the minimum pwm duty cycle percentage cannot be negative!"
	);
	static_assert(
		PWMDutyMax <= 100,
		"the maximum pwm duty cycle percentage cannot be greater than 100!"
	);

	servo(TIM_HandleTypeDef& timer, std::uint32_t channel) noexcept
	: m_timer{&timer},
	  m_channel{channel},
	  m_pwm_resolution{m_timer->Init.Period + 1}
	{
		HAL_TIM_PWM_Start(m_timer, m_channel);
		set_position(DefaultDegree);
	}

	servo(const servo&) = delete;
	servo& operator=(const servo&) = delete;
	servo(servo&&) = delete;
	servo& operator=(servo&&) = delete;

	~servo()
	{
		HAL_TIM_PWM_Stop(m_timer, m_channel);
	}

	void set_position(double degree) noexcept
	{
		if (degree <= MinDegree){
			degree = MinDegree;
		} else if (MaxDegree <= degree){
			degree = MaxDegree;
		}
		__HAL_TIM_SET_COMPARE(m_timer, m_channel, convert_pwm_value(degree));
	}

	[[nodiscard]]
	int get_position() const noexcept
	{
		return convert_pwm_degree(__HAL_TIM_GET_COMPARE(m_timer, m_channel));
	}

private:
	TIM_HandleTypeDef* m_timer;
	std::uint32_t m_channel;
	std::uint32_t m_pwm_resolution;
	double m_pwm_value_min{
		(m_pwm_resolution * PWMDutyMin) / 100.
	};
	double m_pwm_value_max{
		(m_pwm_resolution * PWMDutyMax) / 100.
	};
	double m_pwm_value_resolution{
		m_pwm_value_max - m_pwm_value_min
	};

	constexpr int convert_pwm_value(double degree) const noexcept
	{
		return static_cast<int>(
			m_pwm_value_min + (degree / MaxDegree) * m_pwm_value_resolution
		);
	}

	constexpr int convert_pwm_degree(int pwm_value) const noexcept
	{
		return static_cast<int>(std::round(
			MaxDegree * ((pwm_value - m_pwm_value_min) / m_pwm_value_resolution)
		));
	}
};

/**
  * @brief type alias for sg90 servo motors
  *
  */
using sg90_servo = servo<0, 180, 90, 2.5, 12.>;

/**
  * Sample usage (defined in the USER CODE 2 section of the main.cpp file);
  * stm32::sg90_servo vertical_servo(htim2, TIM_CHANNEL_1);
  */

/**
  * TIM2 specs for STM32F4-DISC at 168 MHz for the sg90_servo;
  * peripheral_frequency = 84'000'000;
  * prescaler            = 27;
  * counter_period		 = 59'999;
  */
} /* namespace stm32 */

#endif /* STM32_SERVO_HPP */
