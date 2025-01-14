# 2025-01-08T16:47:35.919834400
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

platform = client.create_platform_component(name = "pfet_standalone_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "standalone",cpu = "ps7_cortexa9_0",domain_name = "standalone_ps7_cortexa9_0")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_1",domain_name = "freertos_ps7_cortexa9_1")

platform = client.get_component(name="pfet_standalone_platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

comp = client.create_app_component(name="test_freertos_led",platform = "$COMPONENT_LOCATION/../pfet_freertos_platform/export/pfet_freertos_platform/pfet_freertos_platform.xpfm",domain = "freertos_ps7_cortexa9_0",template = "freertos_hello_world")

platform = client.get_component(name="pfet_freertos_platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

platform = client.get_component(name="pfet_standalone_platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

platform = client.get_component(name="pfet_freertos_platform")
domain = platform.get_domain(name="zynq_fsbl")

status = domain.regenerate()

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.regenerate()

platform = client.get_component(name="pfet_standalone_platform")
domain = platform.get_domain(name="zynq_fsbl")

status = domain.regenerate()

domain = platform.get_domain(name="standalone_ps7_cortexa9_0")

status = domain.regenerate()

client.delete_component(name="test_freertos_led")

client.delete_component(name="pfet_standalone_platform")

client.delete_component(name="pfet_freertos_platform")

vitis.dispose()

