/*
 * stm32_hcsr04_ultrasonic_sensor.hpp
 *
 * Copyright (c) 2022 Oğuz Toraman oguz.toraman@protonmail.com
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#ifndef STM32_HCSR04_ULTRASONIC_SENSOR_HPP
#define STM32_HCSR04_ULTRASONIC_SENSOR_HPP

#include <stm32_gpio.hpp>
#include <stm32_timer.hpp>

namespace stm32 {

class hcsr04_ultrasonic_sensor {
public:
	hcsr04_ultrasonic_sensor(
		timer& us_timer,
		gpio_input& input_pin,
		gpio_output& output_pin) noexcept
	: m_us_timer{&us_timer},
	  m_input_pin{&input_pin},
	  m_output_pin{&output_pin}
	{ }

	hcsr04_ultrasonic_sensor(const hcsr04_ultrasonic_sensor&) = delete;
	hcsr04_ultrasonic_sensor& operator=(const hcsr04_ultrasonic_sensor&) = delete;
	hcsr04_ultrasonic_sensor(hcsr04_ultrasonic_sensor&&) = delete;
	hcsr04_ultrasonic_sensor& operator=(hcsr04_ultrasonic_sensor&&) = delete;

	[[nodiscard]]
	int get_distance() const noexcept
	{
		m_output_pin->write(High);
		m_us_timer->sleep_for(initial_delay);
		m_output_pin->write(Low);
		m_us_timer->reset();
		while (m_input_pin->read() != High){
			if (m_us_timer->get() >= max_counter_value){
				return max_distance;
			}
		}
		m_us_timer->reset();
		while (m_input_pin->read() != Low){
			if (m_us_timer->get() >= max_counter_value){
				return max_distance;
			}
		}
		int distance = static_cast<int>(m_us_timer->get() / coefficient);
		m_us_timer->sleep_for(new_measurement_delay);
		return distance;
	}

private:
	timer* m_us_timer;
	gpio_input* m_input_pin;
	gpio_output* m_output_pin;

	static constexpr auto coefficient          = 58.;
	static constexpr int max_distance          = 400; 		/* cm */
	static constexpr int max_counter_value     = 30'000;
	static constexpr int initial_delay         = 10;		/* us */
	static constexpr int new_measurement_delay = 100'000;	/* us */
};

} /* namespace stm32 */

#endif /* STM32_HCSR04_ULTRASONIC_SENSOR_HPP */
