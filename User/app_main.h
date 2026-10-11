#pragma once

// RMCS_KEEP_JTAG comes from the CMake option of the same name; JTAG is kept unless it is
// switched off explicitly.
#ifndef RMCS_KEEP_JTAG
#define RMCS_KEEP_JTAG 1
#endif

#ifdef __cplusplus
extern "C"
{
#endif

  void app_main(void);

#ifdef __cplusplus
}
#endif
