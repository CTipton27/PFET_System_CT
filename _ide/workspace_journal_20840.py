# 2025-01-31T15:47:02.945945200
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

comp = client.clone_component(name="pfet_freertos_platform",new_name="pfet_freertos_platform_core0")

client.delete_component(name="pfet_freertos_platform_core0")

platform = client.create_platform_component(name = "pfet_freertos_core1_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_1",domain_name = "freertos_ps7_cortexa9_1")

client.delete_component(name="pfet_freertos_core1_platform")

platform = client.get_component(name="pfet_freertos_platform")
domain = platform.add_domain(cpu = "ps7_cortexa9_1",os = "freertos",name = "freertos_ps7_cortexa9_1",display_name = "freertos_ps7_cortexa9_1")

