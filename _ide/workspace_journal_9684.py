# 2025-01-13T16:38:47.679372600
import vitis

client = vitis.create_client()
client.set_workspace(path="PFET_System_v1.0")

platform = client.get_component(name="pfet_freertos_platform")
status = platform.build()

comp = client.get_component(name="test_freertos_led")
comp.build()

status = platform.build()

comp.build()

