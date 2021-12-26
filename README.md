# Connect-four-3D
Connect Four Game using Wemos D1 Mini
4x4x4 LED Cube with encoder to select game pieces.

Source Code, Schematic, Project Box STL Files, and BOM provided.
by RCI

Requires the following arduino library files:
Adafruit_NeoPixel.h, 
Adafruit_GFX.h, 
Adafruit_NeoMatrix.h, 
ESPRotarySet.h

![connect4x4x4cubeschematic](https://user-images.githubusercontent.com/71176984/147419218-a3ebf983-3edf-4e18-a610-854017baef62.jpg)

![connect4x4x4ledwiring](https://user-images.githubusercontent.com/71176984/147418770-167b0c93-b8c0-42e3-a541-dccc980b1bad.jpg)


A few notes.....
1.  The WS2811 series LED's do not work well with WEMOS D1 GPIO outputs.  The digital output to the WS2811 is too low
    to reliably set the ws2811 shift register.  Remove the surface mount on-board rectifier diode (black diode near USB connector).
    Solder a jumper wire in its place.  The WEMOS D1 will now have enough voltage to drive the D-IN for the WS2811 LED's.
2.  Battery used in the project is a 3.7V 1000 mA lithium polymer, but any 3.7V 1000 mA battery will work. https://www.amazon.com/YTKavq-1000mAh-Battery-Rechargeable-Connector/dp/B08TTTL5BV/ref=sr_1_13?crid=12OLW2WLXVMN4&keywords=3.7+v+lithium+polymer&qid=1640551164&sprefix=3.7+v+lithium+polymer%2Caps%2C136&sr=8-13 
3.  Charger is a 1A USB Lithium Polymer Charger module, but any USB 1000ma charger will work.  https://www.amazon.com/CHENBO-Lithium-Battery-Charging-Protection/dp/B06XCXPY86/ref=sr_1_18?crid=3GTAPOLXMUMRX&keywords=usb+3.7v+battery+charger+board&qid=1640551113&sprefix=usb+3.7v+battery+charger+board%2Caps%2C84&sr=8-18
4.  Proto board that fits in STL Case was found at Amazon, https://www.amazon.com/ElectroCookie-Prototype-Snappable-Electronics-Gold-Plated/dp/B081QYPHHP/ref=sr_1_83?crid=BRBBRTF4HOI1&keywords=prototype+circuit+board&qid=1640551006&sprefix=prototype+circuit+board%2Caps%2C85&sr=8-83
