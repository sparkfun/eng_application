#Code#

##Launchpad##
This code example is explained in more detail in the project writeup attached to my resume submission.The Launchpad Receiving Software is written in Processing and is designed to be the desktop host program for a hardware device. It receives, decodes, and processes serial data (sent in a custom bitwise protocol). It then sends MIDI commands based on this data.

It was part of my DJ Controller project, I have included additional examples if you wish to review them (they are in the "additional" directory).

##Additional##
The software in the additional directory is the associated primary/secondary firmware for the host program mentioned above.

I have also included an example program I wrote in C for the MSP430G2553 that acts as a keypad lock. It allows a user to type in a passcode, reset a passcode, and gives indications (I never finished the actual *lock* part). It's cool because it saves the passcode in the MCU's flash so it is preserved between power cycles.