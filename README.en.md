<div align="center">

<p>
  <a href="README.md"><img src="docs/langues/fr-off.png" alt="Lire cette page en français" width="150" /></a>
  <img src="docs/langues/en-on.png" alt="English, page shown" width="150" />
</p>

<img src="docs/en/banniere.png" alt="Lift Hill, hauling the train up" width="100%">

</div>

The lift hill is the classic way of giving the train the energy it will live on for the rest of the layout. A chain takes it at the bottom, hauls it to the crest, then lets go. The module drives the chain motor, watches the real speed and detects the arrival at the top.

Like the other modules, it is configured on first boot through a captive portal, then joins the server over WebSocket.

**Version 0.1.0**

<img src="docs/en/sections/s01.png" alt="01 How it works" width="100%">

A Hall effect sensor on the shaft gives one pulse per turn. That measurement serves twice: it slaves the speed to the command, and it detects a stall. A chain straining without moving stops producing pulses, and the module cuts out.

<img src="docs/en/schemas/etats.png" alt="ENGAGED: train detected at the foot of the hill, the chain can take it. LIFTING: climbing, speed slaved to the Hall sensor pulses. CREST: crest reached, slow down then release the chain, before the next train. FAULT: the Hall sensor stays silent for longer than STALL_TIMEOUT_MS, the chain is straining without moving, and the motor cuts out." width="100%">

The `RAMP_MS` ramps avoid a hard start: without them, the chain snaps taut and the train is jolted at the bottom as much as at the top.

<img src="docs/en/sections/s02.png" alt="02 Safety" width="100%">

**The anti-rollback is mechanical.** The firmware watches it, it does not replace it. A pawl stops the train running back down, and no line of code should ever stand in for that part.

**Losing the link does not cut the motor mid-climb.** This is the one module where the safe state is not stopping: a train left standing on the slope rolls back down. If the WebSocket drops during a `LIFTING`, the climb finishes, then the module goes back to waiting.

**Two timeouts bound the operation.** `STALL_TIMEOUT_MS` declares a stall if the Hall sensor goes quiet. `LIFT_TIMEOUT_MS` declares a fault if the crest is never reached.

<img src="docs/en/sections/s03.png" alt="03 Hardware" width="100%">

<img src="docs/en/schemas/brochage.png" alt="Outputs: GPIO 25 motor PWM for chain speed, GPIO 26 direction of rotation, GPIO 27 motor enable, GPIO 2 running LED for the LIFTING state, GPIO 4 fault LED for the FAULT state. Inputs: GPIO 34 Hall sensor, one pulse per turn, GPIO 35 bottom sensor for presence at the foot of the hill, GPIO 32 crest sensor for the arrival at the top." width="100%">

<img src="docs/en/sections/s04.png" alt="04 Settings" width="100%">

<img src="docs/en/schemas/reglages.png" alt="LIFT_SPEED_PERCENT: target climbing speed, too low and the chain slips under load. RAMP_MS: how gently it starts and reaches the crest. STALL_TIMEOUT_MS: Hall sensor silence tolerated before calling it a stall. LIFT_TIMEOUT_MS: maximum duration of a climb before going to fault." width="100%">

Too low a speed makes the chain slip under load, too high a speed makes the arrival at the crest harsh. `RAMP_MS` fixes the second, not the first.

<img src="docs/en/sections/s05.png" alt="05 Bringing it up" width="100%">

First copy [`include/env.h.example`](include/env.h.example) to `include/env.h` and fill it in: fallback portal credentials, the module identity and its secret. That file is not in git, and without it the firmware does not compile.

Requires [PlatformIO](https://platformio.org/) inside Visual Studio Code.

```bash
pio run                  # build
pio run -t upload        # upload the firmware
pio run -t uploadfs      # upload the portal to LittleFS
pio device monitor       # serial console, 115200 baud
```

1. Power the module. It creates a WiFi access point.
2. Connect to it and open `http://192.168.4.1`.
3. Enter the target network.
4. The module reboots, joins the network and announces itself to the server.

The credentials stay in the module's memory, never in the repository.

<img src="docs/en/sections/s06.png" alt="06 Ecosystem" width="100%">

The skeleton, the pinout and the state machine are laid down in `src/main.cpp`. What remains to be written is the speed loop, stall detection on interrupt and reporting telemetry.

The common base for every module is the [WiFi Manager](https://github.com/Microcoaster/MicroCoaster_WifiManager). The other way of giving the train its energy is the [Launch Track](https://github.com/Microcoaster/Launch-Track). The driving is done from the [WebApp](https://github.com/Microcoaster/MicroCoasterWebApp).

---

<sub>MicroCoaster · Author: Cybertrist</sub>
