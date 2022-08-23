/*
 * stm32_uart.hpp
 *
 * Copyright (c) 2022 Oğuz Toraman oguz.toraman@protonmail.com
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 */

#ifndef STM32_UART_HPP
#define STM32_UART_HPP

#include <array>
#include <cstdint>
#include <stm32f4xx_hal.h>

#if !defined(HAL_UART_MODULE_ENABLED) /* module check */
static_assert(false, "HAL UART module is not enabled!");
#endif /* module check */

#if !defined(HAL_DMA_MODULE_ENABLED) /* module check */
static_assert(false, "HAL DMA module is not enabled!");
#endif /* module check */

static_assert(__cplusplus >= 201703L, "C++17 required!");

namespace  stm32 {

template <std::uint32_t TransmitTimeout = 100>
class uart {
public:
	uart(UART_HandleTypeDef& uart_handle) noexcept
	: m_uart_handle{&uart_handle}
	{ }

	uart(const uart&) = delete;
	uart& operator=(const uart&) = delete;
	uart(uart&&) = delete;
	uart& operator=(uart&&) = delete;

	[[nodiscard]]
	UART_HandleTypeDef* get_uart_handle() const noexcept
	{
		return m_uart_handle;
	}

	bool abort_receive() noexcept
	{
		return HAL_UART_AbortReceive(m_uart_handle) == HAL_OK;
	}

	bool abort_transmit() noexcept
	{
		return HAL_UART_AbortTransmit(m_uart_handle) == HAL_OK;
	}

	template <std::size_t MessageLength>
	bool receive_to_dma(std::array<char, MessageLength>& rx_message) noexcept
	{
		const auto result = HAL_UART_Receive_DMA(
			m_uart_handle,
			reinterpret_cast<std::uint8_t*>(rx_message.data()),
			MessageLength
		);
		return (result == HAL_OK);
	}

	template <std::size_t MessageLength>
	bool transmit(const std::array<char, MessageLength>& tx_message) noexcept
	{
		const auto result = HAL_UART_Transmit(
			m_uart_handle,
			reinterpret_cast<std::uint8_t*>(
				const_cast<char *>(
					tx_message.data()
				)
			),
			MessageLength,
			TransmitTimeout
		);
		return (result == HAL_OK);
	}

private:
	UART_HandleTypeDef* m_uart_handle;
};

template <std::uint32_t TransmitTimeout = 100>
using hc_05 = uart<TransmitTimeout>;

/**
  * Example;
  *
  * std::array<char, 5> rx_message{};
  * std::array<char, 5> tx_message{'1','3','3','2','1'};
  * stm32::uart raspberrypi(huart3);
  * raspberrypi.transmit(tx_message);
  * raspberrypi.receive_to_dma(rx_message);
  */

} /* namespace stm32 */

#endif /* STM32_UART_HPP */
