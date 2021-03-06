# Workshop Slides

* [Part 1 Slides](./slides.pdf)
* [Part 2 Slides](./slides_part2.pdf)

# SFML Sample Code

* Sample sfml code to be used as a compilation test: [sfml_sample.cpp](https://raw.githubusercontent.com/Ohohcakester/gdg-sfml-workshop/master/compilation_tests/sample_game.cpp?token=AE989UeWcuWW9S6mQSNjnEadbe22VENZks5XUCg6wA%3D%3D)

Expected Result:

![sfml_sample](../images/images/sfml_sample.gif)

# Compiling with SFML

SFML Download Page (Download the binary corresponding to your compiler, else it won't work)

* [SFML Download](http://www.sfml-dev.org/download/sfml/2.3.2/)

The official setup instructions for SFML can be found here.

* [SFML Tutorial (Setup)](http://www.sfml-dev.org/tutorials/2.3/)

NOTE: Don't try to compile the SFML source code yourself, especially if you don't know what you are doing. Download a precompiled binary from the [downloads](http://www.sfml-dev.org/download/sfml/2.3.2/) page. You'll have a much, much easier time.


Below are some additional pointers for setting up SFML.

## Windows:
Either

**M1: Visual Studio [Recommended]**

Install Visual Studio with C++, download the SFML binary for your Visual Studio version, follow all the setup instructions in the [SFML tutorial](http://www.sfml-dev.org/tutorials/2.3/start-vc.php).

Or

**M2: MinGW / g++, from command line**

Download MinGW **4.9.2**, and download the SFML binary for MinGW from the downloads page. Make sure you add the `bin` folder for MinGW to your path so that you can use g++. Ask me if you don't know how to do that.
Place the sfml contents in a folder called "sfml" in the same directory as your game code, and run these two lines to build:

```
g++ -std=c++11 -c *.cpp -I sfml/include
g++ -std=c++11 *.o -o Game -L sfml/lib -lsfml-graphics -lsfml-window -lsfml-system
```

Note: The game won't run if you don't put the .dll files from the sfml/bin directory into the same directory as your Game.exe


## Mac OSX

Either

**M1: Brew**

Note: most convenient, but not recommended. Your compiled game will work on your computer only. (but okay if just for the workshop)
Make sure you are able to use g++ on your terminal. Then install SFML via `brew install sfml`.

To compile your code, run these two lines:
```
g++ -std=c++11 -c *.cpp
g++ -std=c++11 *.o -o Game -lsfml-graphics -lsfml-window -lsfml-system
```

Or

**M2: Xcode [Recommended]**

The recommended way for setting up SFML on a Mac. Follow the instructions on the [SFML tutorial](http://www.sfml-dev.org/tutorials/2.3/start-osx.php).


## Linux

**M1: apt-get**

Make sure you are able to use g++. Install SFML via `sudo apt-get install libsfml-dev`
then run these two lines to compile.
```
g++ -std=c++11 -c *.cpp
g++ -std=c++11 *.o -o Game -lsfml-graphics -lsfml-window -lsfml-system
```

## Versions

#### V0

![v0](../images/images/v0.png)

#### V1,V2

![v1v2](../images/images/v1v2.png)

#### V3

![v3](../images/images/v3.png)

#### V4

![v4](../images/images/v4.gif)

#### V5

![v5](../images/images/v5.gif)

#### V6

![v6](../images/images/v6.gif)

#### V7

![v7](../images/images/v7.gif)

#### V8

![v8](../images/images/v8.gif)

## Part 2 Versions

#### V9 - Camera

Add camera object - no visual change

#### V10 - Camera

![v10](../images/images/v10.gif)

#### V11 - Camera

![v11](../images/images/v11.gif)

#### V12 - Camera

Smooth camera movement

![v12](../images/images/v12.gif)

#### V13 - SFX

Create Sfx abstract class - no visual change

#### V14 - SFX
/
![v14](../images/images/v14.gif)

#### V15 - SFX

![v15](../images/images/v15.gif)

#### V16 - Array Deletion

Regularly clean up inactive objects in array - no visual change

#### V17 - Menu

![v17](../images/images/v17.gif)

#### V18 - Menu

![v18](../images/images/v18.gif)

#### VFinal - Splitting

Split the game into multiple files. Game is exactly the same.
