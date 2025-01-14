# 2025-01-13T21:04:05.207981400
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.get_component(name="pfet_freertos_platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = platform.build()

comp = client.get_component(name="test_freertos_led")
comp.build()

domain = platform.get_domain(name="zynq_fsbl")

status = domain.set_config(option = "lib", param = "XILTIMER_tick_timer", value = "ps7_scutimer_0", lib_name="xiltimer")

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = domain.regenerate()

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.regenerate()

status = domain.set_lib(lib_name="xilrsa", path="C:\Xilinx\Vitis\2024.2\data\embeddedsw\lib\sw_services\xilrsa_v1_8")

platform = client.create_platform_component(name = "pfet_freertos_platform_2",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

domain = platform.get_domain(name="zynq_fsbl")

status = domain.regenerate()

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.regenerate()

status = platform.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = domain.regenerate()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

platform = client.get_component(name="pfet_standalone_platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

client.delete_component(name="pfet_freertos_platform")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

platform = client.get_component(name="pfet_freertos_platform")
status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = domain.set_config(option = "os", param = "freertos_support_static_allocation", value = "true")

status = platform.build()

comp.build()

status = platform.build()

comp.build()

client.delete_component(name="pfet_freertos_platform")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

client.delete_component(name="pfet_freertos_platform")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

client.delete_component(name="pfet_freertos_platform")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

status = platform.build()

comp.build()

status = domain.set_config(option = "os", param = "freertos_support_static_allocation", value = "true")

status = platform.build()

comp.build()

