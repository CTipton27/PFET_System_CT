# 2025-01-08T19:21:58.334092900
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.create_platform_component(name = "pfet_standalone_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

comp = client.create_app_component(name="test_freertos_led",platform = "$COMPONENT_LOCATION/../pfet_freertos_platform/export/pfet_freertos_platform/pfet_freertos_platform.xpfm",domain = "freertos_ps7_cortexa9_0",template = "freertos_hello_world")

platform = client.get_component(name="pfet_freertos_platform")
domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.set_lib(lib_name="xilffs", path="C:\Xilinx\Vitis\2024.2\data\embeddedsw\lib\sw_services\xilffs_v5_3")

status = domain.set_lib(lib_name="xilrsa", path="C:\Xilinx\Vitis\2024.2\data\embeddedsw\lib\sw_services\xilrsa_v1_8")

status = domain.regenerate()

status = platform.build()

status = platform.build()

status = platform.build()

comp = client.get_component(name="test_freertos_led")
comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = domain.regenerate()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = domain.set_config(option = "os", param = "freertos_support_static_allocation", value = "true")

status = platform.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = platform.build()

comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = platform.build()

comp.build()

status = comp.clean()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = comp.clean()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.build()

comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

domain = platform.get_domain(name="zynq_fsbl")

status = domain.regenerate()

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.regenerate()

status = platform.build()

comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = domain.regenerate()

domain = platform.get_domain(name="zynq_fsbl")

status = domain.regenerate()

vitis.dispose()

