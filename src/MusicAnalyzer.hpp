/**
 * @file MusicAnalyzer.hpp
 * @author your name (you@domain.com)
 * @brief
 * @version 0.1
 * @date 2019-05-13
 *
 * @copyright Copyright (c) 2019
 *
 */
#ifndef MUSICANALYZER_HPP
#define MUSICANALYZER_HPP

#include <memory>

#include <FreeRTOS.h>
#include <driver/i2s.h>

#include "GlobalController.hpp"

namespace EnlightingLetters
{
/**
 * @brief
 *
 */
class MusicAnalyzer
{
 public:
  MusicAnalyzer(std::shared_ptr<GlobalController> state);
  ~MusicAnalyzer();

  void UpdateState();

 private:
  std::shared_ptr<GlobalController> mGlobalController;
  bool mOk;

  const i2s_port_t kI2CPort = I2S_NUM_0;
  const i2s_config_t kI2CConfig = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 32000,
      .bits_per_sample = I2S_BITS_PER_SAMPLE_32BIT,
      .channel_format = I2S_CHANNEL_FMT_ONLY_RIGHT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_I2S | I2S_COMM_FORMAT_I2S_MSB),
      .intr_alloc_flags = ESP_INTR_FLAG_LEVEL1,
      .dma_buf_count = 8,
      .dma_buf_len = 16};
  const i2s_pin_config_t kPinConfig = {
      .bck_io_num = 26,  // BCKL
      .ws_io_num = 25,   // LRCL
      .data_out_num = I2S_PIN_NO_CHANGE,
      .data_in_num = 22  // DOUT
  };
};
}  // namespace EnlightingLetters

#endif