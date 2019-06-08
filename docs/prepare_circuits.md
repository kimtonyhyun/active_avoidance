---
layout: default
title: 1. Prepare circuits
nav_order: 2
---

## Prepare "4pin-bus" circuit board

Link to OSH Park where the board can be ordered: [4pin-bus](https://oshpark.com/shared_projects/qv0rC8Yo)

Solder header pins on the "4pin-bus" circuit board, as shown below:
![Finished 4pin-bus circuit board](4pin_bus.jpg)

Note that one of the header sets protrudes bidirectionally.

This small circuit board lets us conveniently sample electrical signals (e.g. with Saleae). You can populate the other header slots as well, but the photo shows the minimal number of headers necessary.

We need 3 "4-pin bus" circuit boards _per_ active avoidance setup.

## Prepare WAV Trigger board

Solder three sets of header pins on the WAV Trigger board, as shown below:
![WAV Trigger board with header pins](wav_trigger.jpg)

Next, apply a solder jumper to the jumper location indicated by "5V", as below:
![Solder jumper for 5V](wav_trigger_solderjumper.jpg)

For more information, see [WAV Trigger user guide](https://robertsonics.com/wav-trigger-online-user-guide/#chapter1)
