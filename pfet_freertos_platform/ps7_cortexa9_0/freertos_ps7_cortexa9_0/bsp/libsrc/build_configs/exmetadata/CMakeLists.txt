
cmake_minimum_required(VERSION 3.15)
project(bsp)
        
set(DRIVER_TARGETS axidma;gpio;intc;coresightps_dcc;devcfg;dmaps;emacps;gpiops;scugic;qspips;scutimer;scuwdt;sdps;ttcps;uartps;usbps;xadcps;sysmon)
set(DRIVER_LOCATIONS C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/axidma_v9_19;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/gpio_v4_12;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/intc_v3_19;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/coresightps_dcc_v1_11;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/devcfg_v3_8;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/dmaps_v2_11;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/emacps_v3_21;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/gpiops_v3_13;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/scugic_v5_4;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/qspips_v3_13;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/scutimer_v2_6;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/scuwdt_v2_6;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/sdps_v4_4;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/ttcps_v3_20;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/uartps_v3_15;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/usbps_v2_9;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/xadcps_v2_7;C:/Xilinx/Vitis/2024.2/data/embeddedsw/XilinxProcessorIPLib/drivers/sysmon_v7_10)

list(LENGTH DRIVER_TARGETS no_of_drivers)
set(index 0)

while(${index} LESS ${no_of_drivers})
    list(GET DRIVER_TARGETS ${index} drv)
    list(GET DRIVER_LOCATIONS ${index} drv_dir)
    set(exp_dir "${drv_dir}/examples")
    add_custom_target(
        ${drv}_example ALL
        COMMAND lopper -f -O C:/Users/ychna/xilinx_vivado/PFET_System_v1.0/pfet_freertos_platform/ps7_cortexa9_0/freertos_ps7_cortexa9_0/bsp/libsrc/${drv} C:/Users/ychna/xilinx_vivado/PFET_System_v1.0/pfet_freertos_platform/ps7_cortexa9_0/freertos_ps7_cortexa9_0/bsp/hw_artifacts/ps7_cortexa9_0_baremetal.dts -- bmcmake_metadata_xlnx ps7_cortexa9_0 ${exp_dir} drvcmake_metadata
        BYPRODUCTS x${drv}_exlist.yaml)
    MATH(EXPR index "${index}+1")
endwhile()
