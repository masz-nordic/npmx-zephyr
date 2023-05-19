.. _npmx_ldo_sample:

LDO
###

Overview
********

This sample demonstrates the usage of nPM1300 PMIC LDOs.
For testing purposes, connect a voltage multimeter to LSOUT1 and LSOUT2.
In this sample, output voltage ranges are as follows:

* From 1.0 V to 1.7 V for LDO1.
* From 1.0 V to 2.9 V for LDO2.

Requirements
************

To run this sample, do the following:

* Connect the nRF5340 or nRF52840 DK to a host device.
* Open a COM port.
* Connect nPM1300 EK with the chosen DK.

Connections
-----------

.. list-table:: nPM1300 EK connections.
   :widths: 25 25 25
   :header-rows: 1

   * - nPM1300 EK
     - nRF5340 DK
     - nRF52840 DK
   * - GPIO0
     - P1.10
     - P1.10
   * - SDA
     - P1.02
     - P0.26
   * - SCL
     - P1.03
     - P0.27
   * - VOUT2 + GND
     - External supply (P21)
     - External supply (P21)

On nPM1300 EK do the following:

* Connect USB to power supply (J3).
* On **P18** connect with the VOUT2 - VDDIO jumper.
* On **P2** connect with the VBAT - VBATIN jumper.
* On **P17** connect all LEDs with jumpers.
* On **P13** connect with the RSET1 - GND jumper.
* On **P14** connect with the RSET2 - VSET2 jumper.
* On **P16** connect bypass capacitors with two jumpers.

On nRF5340 DK do the following:

* Connect USB to host device (J2).
* Switch nRF power source (SW9) to VDD.
* Switch VEXT -> nRF (SW10) to ON.

On nRF52840 DK do the following:

* Connect USB to host device (J2).
* Switch nRF power source (SW9) to VDD.
* Switch VEXT -> nRF (SW10) to ON.
