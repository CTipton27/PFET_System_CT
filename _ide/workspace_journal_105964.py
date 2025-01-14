# 2025-01-08T16:39:57.568690800
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

vitis.dispose()

