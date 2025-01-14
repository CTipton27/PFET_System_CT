# 2025-01-09T17:48:05.597632900
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.get_component(name="pfet_freertos_platform")
status = platform.build()

comp = client.get_component(name="test_freertos_led")
comp.build()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.regenerate()

domain = platform.get_domain(name="zynq_fsbl")

status = domain.regenerate()

status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

status = domain.regenerate()

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.regenerate()

vitis.dispose()

