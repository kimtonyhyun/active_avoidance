---
layout: default
title: 1. Prepare circuits
nav_order: 2
---

# 1. Prepare circuits
{: .no_toc }

## Table of contents
{: .no_toc .text-delta }

1. TOC
{:toc}

---

## Prepare "4pin-bus" circuit board

Link to OSH Park where the board can be ordered: [4pin-bus](https://oshpark.com/shared_projects/qv0rC8Yo)

Solder header pins on the "4pin-bus" circuit board, as shown below:
![Finished 4pin-bus circuit board](4pin_bus.jpg)

Note that one of the header sets (left one, in above picture) protrudes bidirectionally.

This small circuit board lets us conveniently sample electrical signals (e.g. with Saleae). You can populate the other header slots as well, but the photo shows the minimal number of headers necessary. You can also use right angle headers (as shown) or straight ones.

We need 3 "4-pin bus" circuit boards per active avoidance setup.

---

## Prepare WAV Trigger board

Solder three sets of header pins on the WAV Trigger board, as shown below:
![WAV Trigger board with header pins](wav_trigger.jpg)

Next, apply a solder jumper to the jumper location indicated by "5V", as below:
![Solder jumper for 5V](wav_trigger_solderjumper.jpg)

For more information on the jumper, see [WAV Trigger user guide](https://robertsonics.com/wav-trigger-online-user-guide/#chapter1)

---

## Load the SD card

I use:
- [1 GB SD card from Sparkfun](https://www.sparkfun.com/products/15107), and
- [USB card reader from Amazon](https://www.amazon.com/IOGEAR-MicroSD-Reader-Writer-GFR204SD/dp/B0046TJG1U)

Using the above hardware, copy over the two files located [here](https://github.com/kimtonyhyun/active_avoidance/tree/master/sd_card) onto the SD card.

The WAV file is the auditory (linear) sweep, from 2 kHz to 6 kHz over 5 seconds. I used [this Matlab script](https://github.com/kimtonyhyun/active_avoidance/blob/master/scripts/wavegen.m) to generate the WAV file. While the standard active avoidance protocol uses 4 second trial (and thus CS) periods, I made the WAV file to be 5 seconds long, as to avoid any odd auditory effects at the end of the file.

The INI file provides settings for the WAV Trigger board. (For more info see [WAV Trigger user guide](http://robertsonics.com/wav-trigger-online-user-guide/#chapter8).)

After the two files have been loaded onto the SD card, insert the card into the WAV Trigger board until it clicks once.

__Make sure that the Load/Run slider switch is in the Run position!__

---

## Prepare the BNC panel

Populate the BNC panel as follows:
![Front view of the BNC panel](bnc_panel_front.jpg)

I use the following components from Digikey:
- Toggle switch: [EG2350-ND](https://www.digikey.com/products/en?keywords=eg2350-nd)
- Non-isolating BNC bulkhead: [ACX1046-ND](https://www.digikey.com/products/en?keywords=acx1046-nd)

Viewed from the back, the components should be wired up as follows:
![Rear view of the BNC panel](bnc_panel_rear.jpg)

Here, the color code is:
- Green/yellow: Electrical ground
- Gray: Middle pole of the switch
- Red: Scope enable
- Orange: US

We will refer to these colors later in the instructions.

---

## Prepare the speaker (optional)

Solder wires onto a standalone speaker, as shown below:
![Soldered speakers](speakers.jpg)
