# Hackweek powered by BWM Group Werk Regensburg

![hack week car](/img/hack-week-car.jpg)

This repository contains the source code for the car we're planning to use at the [MINT-Labs Hackweek][event]. The goal of the event will be to replace the preinstalled RC car receiver and speed controller with an ESP32 and custom speed controller, allowing for further customization (as well as allowing for the car to be controlled by a smartphone via bluetooth).

## Repository structure

This repository contains both the Arduino project that is required for the ESP32 to receive commands, as well as the App Inventor application files that build the application which is needed to control the car from a mobile device. Both files can be found under their respectively named folders.

> There are currently three app inventor files: `slider1.aia`, `slider2.aia` and `hack-week-car.aia`. `slider1.aia` and `slider2.aia` attempt to implement multi-touch sliders for speed and steering control, but are still in alpha-stages of development.

[event]: https://www.mint-labs-regensburg.de/course/10059/hack-week-sponsored-by-bmw.html
