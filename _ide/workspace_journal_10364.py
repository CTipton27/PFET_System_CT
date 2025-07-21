# 2025-01-15T22:58:04.845321300
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.get_component(name="pfet_freertos_platform")
status = platform.update_hw(hw_design = "$COMPONENT_LOCATION/../pfet_system_wrapper.xsa")

domain = platform.get_domain(name="zynq_fsbl")

status = domain.regenerate()

domain = platform.get_domain(name="freertos_ps7_cortexa9_0")

status = domain.regenerate()

status = platform.build()

vitis.dispose()

