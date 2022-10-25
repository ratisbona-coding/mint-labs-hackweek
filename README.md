# Hackweek powered by BWM Group Werk Regensburg

![hack week car](/img/hack-week-car.jpg)

This repository contains the source code for the car we're planning to use at the [MINT-Labs Hackweek][event]. The goal of the event will be to replace the preinstalled RC car receiver and speed controller with an ESP32 and custom speed controller, allowing for further customization (as well as allowing for the car to be controlled by a smartphone via bluetooth).

## Repository structure

This repository contains both the Arduino project that is required for the ESP32 to receive commands, as well as the App Inventor application files that build the application which is needed to control the car from a mobile device. Both files can be found under their respectively named folders.

> There are currently three app inventor files: `slider1.aia`, `slider2.aia` and `hack-week-car.aia`. `slider1.aia` and `slider2.aia` attempt to implement multi-touch sliders for speed and steering control, but are still in alpha-stages of development.

[event]: https://www.mint-labs-regensburg.de/course/10059/hack-week-sponsored-by-bmw.html

## App Inventor - How to get started?

MIT App Inventor is an intuitive, visual programming environment that allows everyone to build fully functional apps for Android phones, iPhones, and Android/iOS tablets in less than 30 minutes. We used the tool to control the car via a tablet. 

1. Go to the MIT App Inventor - use the link below to log in without an account: 

    http://code.appinventor.mit.edu/

    ![App Inventor](/img/AppInventor-0.png)


2. When logging in, a return code will be displayed for the user - make sure to copy and save the return code in a safe place, because projects created cannot be retrieved without the code, after you log out once:

    ![App Inventor](/img/AppInventor-1.png)


3. Create a new project and import the .aia files from the GitHub repsitory:

    ![App Inventor](/img/AppInventor-2.png)

    ![App Inventor](/img/AppInventor-3.png)

4. Connect to an Android Tablet or Phone, click *Connect - AI Companion* and scan the appeared QR code with your device:

    ![App Inventor](/img/AppInventor-4.png)

    ![App Inventor](/img/AppInventor-5.png)

    Now you are connected to the project via your device and can steer the car as well as preview the changes you do in your project. If you close the window on your device your connection to your project will be lost. If you lose the connection, repeat the steps to generate a new QR code and connect again.

5. If you log out and want to continue the next day, use your initial code to login again:

    ![App Inventor](/img/AppInventor-6.png)





