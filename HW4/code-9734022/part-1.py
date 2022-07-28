#!/usr/bin/env python3

import time
from datetime import datetime
# from Crypto.Cipher import AES
import base64

from mia_py.I1820.app import I1820App
from mia_py.I1820.domain.notif import I1820Notification

# from things.models.light import Light

tenant_id = "mohammad-home"

app = I1820App(tenant_id, "127.0.0.1")

# msg_text = b'test1:4'.rjust(32)
# secret_key = b'mohammad'  # create new & store somewhere safe
# cipher = AES.new(secret_key, AES.MODE_ECB)  # never use ECB in strong systems obviously
# encoded = base64.b64encode(cipher.encrypt(msg_text))


@app.notification("lamp", "alarm", "smartLamp")
def lamp_notification(data: I1820Notification):
    print(f'we have a notification for {data.device} of {data.type}  {data}')
    return True


if __name__ == "__main__":
    app.add_thing("lamp", "1111")
    app.add_thing("alarm", "86986")
    app.add_thing("smartLamp", "4986")
    # app.add_thing("multisensor", encoded)
    app.add_thing("multisensor", "967097")

    app.run()

    TEMPERATURE = 10
    LIGHT = 1024
    MOTION = 1
    while True:
        TEMPERATURE = (TEMPERATURE + 10) % 100
        LIGHT = LIGHT / 2 if LIGHT >= 128 else LIGHT * 2
        MOTION = 0 if MOTION == 1 else 1

        # print(encoded)
        # ...
        # decoded = cipher.decrypt(base64.b64decode(encoded))
        # print(decoded.strip())

        states = []
        states.append({"name": "temperature", "value": str(TEMPERATURE)})
        states.append({"name": "light", "value": str(LIGHT)})
        states.append({"name": "motion", "value": str(MOTION)})
        states.append({"name": "humidity", "value": "24"})

        app.log("multisensor", "09707", states)
        print(f'sending information to mia {datetime.now()}')
        time.sleep(5)
