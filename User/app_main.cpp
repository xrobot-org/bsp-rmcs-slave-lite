// LibXR objects and hardware registration of the RMCS Slave Lite BSP.
// Pins and clocks are set up in main.cpp. Put application code between the
// "User Code Begin" and "User Code End" markers; the layout follows bsp_stm32f103.
#include "app_main.h"

#include <cstdint>

#include "board.h"
#include "hpm_gpio.hpp"
#include "hpm_timebase.hpp"
#include "libxr.hpp"
#include "xrobot_main.hpp"

using namespace LibXR;

/* User Code Begin 1 */
/* User Code End 1 */

// DMA buffers: HPMGPIO needs none. The UART, SPI and CAN buffers are added together with
// their drivers.

extern "C" void app_main(void)
{
  /* User Code Begin 2 */
  /* User Code End 2 */

  // Timebase and platform
  static HPMTimebase timebase;
  PlatformInit();

  // GPIO: the pins and their initial state come from init_bsp_pins(); SetConfig() sets the
  // same direction and pull again for the LibXR object.
  // BMI088 chip selects, active low, idle high
  static HPMGPIO imu_cs_gyro(BOARD_IMU_CS_GYRO_GPIO_CTRL, BOARD_IMU_CS_GYRO_GPIO_INDEX,
                             BOARD_IMU_CS_GYRO_GPIO_PIN);
  static HPMGPIO imu_cs_accel(BOARD_IMU_CS_ACCEL_GPIO_CTRL, BOARD_IMU_CS_ACCEL_GPIO_INDEX,
                              BOARD_IMU_CS_ACCEL_GPIO_PIN);
  imu_cs_gyro.Write(true);
  imu_cs_accel.Write(true);
  imu_cs_gyro.SetConfig({GPIO::Direction::OUTPUT_PUSH_PULL, GPIO::Pull::NONE});
  imu_cs_accel.SetConfig({GPIO::Direction::OUTPUT_PUSH_PULL, GPIO::Pull::NONE});

  // BMI088 interrupt lines and the USB HS/FS switch, inputs without interrupts
  static HPMGPIO imu_int_gyro(BOARD_IMU_INT_GYRO_GPIO_CTRL, BOARD_IMU_INT_GYRO_GPIO_INDEX,
                              BOARD_IMU_INT_GYRO_GPIO_PIN);
  static HPMGPIO imu_int_accel(BOARD_IMU_INT_ACCEL_GPIO_CTRL,
                               BOARD_IMU_INT_ACCEL_GPIO_INDEX,
                               BOARD_IMU_INT_ACCEL_GPIO_PIN);
  static HPMGPIO usb_sw(BOARD_USB_SW_GPIO_CTRL, BOARD_USB_SW_GPIO_INDEX,
                        BOARD_USB_SW_GPIO_PIN);
  imu_int_gyro.SetConfig({GPIO::Direction::INPUT, GPIO::Pull::NONE});
  imu_int_accel.SetConfig({GPIO::Direction::INPUT, GPIO::Pull::NONE});
  usb_sw.SetConfig({GPIO::Direction::INPUT, GPIO::Pull::DOWN});

  // KEY on PA07, pull-up, low when pressed. PA07 is the JTAG TMS pin: the object is
  // always created and registered (xrobot's generator does not accept XR_REGISTER inside
  // #if), but the pin is configured only without JTAG. With RMCS_KEEP_JTAG on, do not
  // use `key` from a Module: its SetConfig() would take PA07 away from JTAG.
  static HPMGPIO key(BOARD_KEY_GPIO_CTRL, BOARD_KEY_GPIO_INDEX, BOARD_KEY_GPIO_PIN);
#if !RMCS_KEEP_JTAG
  key.SetConfig({GPIO::Direction::INPUT, GPIO::Pull::UP});
#endif

  // PWM: the buzzer (GPTMR0 COMP2) and the IMU heater (GPTMR2 COMP2) have no LibXR object
  // yet. On the HPM5361 HPMPWM drives the PWM peripheral (PWM0/PWM1) only; its GPTMR path
  // is built for SoCs without PWM. Their pins and the GPTMR clocks are set up by
  // init_bsp_pins() and init_clocks().

  // Hardware registration
  XR_REGISTER(imu_cs_gyro, LibXR::GPIO);
  XR_REGISTER(imu_cs_accel, LibXR::GPIO);
  XR_REGISTER(imu_int_gyro, LibXR::GPIO);
  XR_REGISTER(imu_int_accel, LibXR::GPIO);
  XR_REGISTER(usb_sw, LibXR::GPIO);
  XR_REGISTER(key, LibXR::GPIO);

  /* User Code Begin 3 */
  /* User Code End 3 */
  XROBOT_MAIN();
}
