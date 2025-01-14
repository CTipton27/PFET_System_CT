# 2025-01-13T11:39:11.586641200
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.create_platform_component(name = "pfet_freertos_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

platform = client.create_platform_component(name = "pfet_freerots_platform",hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa",os = "freertos",cpu = "ps7_cortexa9_0",domain_name = "freertos_ps7_cortexa9_0")

comp = client.get_component(name="test_freertos_led")
comp.build()

comp.build()

status = comp.clean()

comp.build()

platform = client.get_component(name="pfet_freerots_platform")
status = platform.build()

comp.build()

comp.build()

comp.build()

client.delete_component(name="pfet_freerots_platform")

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

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.set_config(option = "os", param = "freertos_support_static_allocation", value = "true")

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

status = domain.set_config(option = "lib", param = "XILTIMER_tick_timer", value = "ps7_scutimer_0", lib_name="xiltimer")

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

status = platform.build()

comp.build()

status = domain.set_config(option = "lib", param = "XILTIMER_tick_timer", value = "ps7_ttc_0", lib_name="xiltimer")

status = platform.build()

status = platform.build()

comp.build()

status = domain.set_config(option = "lib", param = "XILTIMER_tick_timer", value = "axi_timer_led", lib_name="xiltimer")

status = platform.build()

comp.build()

status = domain.set_config(option = "lib", param = "XILTIMER_tick_timer", value = "ps7_scutimer_0", lib_name="xiltimer")

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

