# Midi Speaker Project

This repository was created and added to during RIT's Winter Swen-340 class during the 24-25 year. It acts as an intro to embedded development and very basic operating systems for Software Engineering BS Students. The majority of the contents in this repository is boilerplate for the microcontroller. If you want to see my code, follow the path of SWEN-340/Core/Src/Project. 

## Features

The code centers on manipulating an STM32L0467 to do a couple various functions. In specific, I learned about interrupts, timing, analog to digital conversion, and how to design a closed system that only works with the stack. It includes function for a speaker to play music, a LED to show state, and a button to change the state, but the state can also be changed via a serial terminal. Finally, it plays music that is preloaded to the system and in my specific case, those files are read on the fly.

## What did not work

- Polyphony, I just did not have time and did not understand how it was supposed to work so i created this weird syncopation kind of sound instead. It is not that hard to understand though so i will get it working while I ta for the class
- On the fly file introduction. I want to be able to input brand new files to the program without interrupting other function. I expect this to be quite challenging, but I can do it

