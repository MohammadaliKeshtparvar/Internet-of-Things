#!/usr/bin/env python3

import time
from datetime import datetime

from mia_py.I1820.app import I1820App
from mia_py.I1820.domain.notif import I1820Notification
from light import Light
from lamp import Lamp

tenant_id = "mohammad-home"

app = I1820App(tenant_id, "127.0.0.1")

light_1 = Light(name="light", device_id="test2:1")

lamps = [Lamp(device_id="test2:2"), Lamp(device_id="test2:3"),
         Lamp(device_id="test2:4"), Lamp(device_id="test2:5")]


def find_lamp(device_id: str) -> Lamp:
    for led in lamps:
        if led.device_id == device_id:
            return led


@app.notification("lamp", "alarm", "smartLamp")
def lamp_notification(data: I1820Notification):
    print(get_state_from_notif(data.settings))
    find_lamp(data.device).state = get_state_from_notif(data.settings)
    print(f'we have a notification for {data.device} of {data.type}, {data.settings}')
    return True


def get_state_from_notif(settings: list[dict]) -> str:
    state = ""
    for setting in settings:
        if setting['name'].lower() == 'on':
            state = "on" if setting['value'] else "off"
        if setting['name'].lower() == 'off':
            state = "off" if setting['value'] else "on"
    return state


if __name__ == "__main__":
    app.add_thing(light_1.name, light_1.device_id)

    for lamp in lamps:
        app.add_thing(lamp.name, lamp.device_id)

    app.run()

    while True:
        app.log("light", "test2:1", [light_1.to_dict()])
        for lamp in lamps:
            app.log("lamp", lamp.device_id, [lamp.to_dict()])
        print(f'sending information to mia {datetime.now()}')
        time.sleep(10)
