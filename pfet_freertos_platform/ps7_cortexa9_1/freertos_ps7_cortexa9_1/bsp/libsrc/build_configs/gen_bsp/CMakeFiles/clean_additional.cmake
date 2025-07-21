# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "")
  file(REMOVE_RECURSE
  "C:\\Users\\ychna\\xilinx_vivado\\PFET_System_v1.0\\pfet_freertos_platform\\ps7_cortexa9_1\\freertos_ps7_cortexa9_1\\bsp\\include\\sleep.h"
  "C:\\Users\\ychna\\xilinx_vivado\\PFET_System_v1.0\\pfet_freertos_platform\\ps7_cortexa9_1\\freertos_ps7_cortexa9_1\\bsp\\include\\xiltimer.h"
  "C:\\Users\\ychna\\xilinx_vivado\\PFET_System_v1.0\\pfet_freertos_platform\\ps7_cortexa9_1\\freertos_ps7_cortexa9_1\\bsp\\include\\xtimer_config.h"
  "C:\\Users\\ychna\\xilinx_vivado\\PFET_System_v1.0\\pfet_freertos_platform\\ps7_cortexa9_1\\freertos_ps7_cortexa9_1\\bsp\\lib\\libxiltimer.a"
  )
endif()
