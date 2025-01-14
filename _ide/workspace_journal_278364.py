# 2025-01-10T11:03:27.838427400
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.get_component(name="pfet_freertos_platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = platform.build()

client.delete_component(name="pfet_standalone_platform")

client.delete_component(name="pfet_freertos_platform")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.set_lib(lib_name="xilffs", path="C:\Xilinx\Vitis\2024.2\data\embeddedsw\lib\sw_services\xilffs_v5_3")

status = domain.set_lib(lib_name="xilrsa", path="C:\Xilinx\Vitis\2024.2\data\embeddedsw\lib\sw_services\xilrsa_v1_8")

status = platform.build()

status = platform.build()

comp = client.get_component(name="test_freertos_led")
comp.build()

client.delete_component(name="pfet_freertos_platform")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

status = platform.build()

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

status = platform.build()

comp.build()

vitis.dispose()

