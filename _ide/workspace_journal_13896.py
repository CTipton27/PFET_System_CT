# 2025-01-12T08:28:08.463457100
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.get_component(name="pfet_standalone_platform")
status = platform.build()

comp = client.get_component(name="test_axi_timer_interrupt")
comp.build()

status = platform.build()

comp.build()

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

vitis.dispose()

