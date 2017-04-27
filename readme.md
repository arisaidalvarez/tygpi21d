# André LaMothe's "Teach Yourself Game Programming in 21 Days"
Here's my trek through this great book from 1996.

## Setup
I used Borland's Turbo C v2.01 to compile the examples. I also needed to set the memory model to 'Large' so it could see the far pointers.

André used Microsoft C/C++ 7.0; several things have to be changed for this to work in TC:

| MSCC7        | TC2         |
| ------------ | ----------- |
| `<memory.h>` | `<mem.h>`   |
| `_ffree`     | `farefree`  |
| `_fmalloc`   | `farmalloc` |
| `_fmemset`   | `memset`    |
| `_inp`       | `inp`       |
| `_int86`     | `int86`     |
| `_outp`      | `outp`      |

Also, TC doesn't support `// single line comments`, so it's `/* olde schoole time */`.

## TODO
* Chapter 3, Exercise 5: Try to make `Plot_Pixel_Fast()` even faster!
* Chapter 3, Exercise 6: Using the keyboard as an input device, try to mkae a crude drawing program that allows the user to change colors and move a pen on the screen.
* Chapter 4, Exercise 1: Write a function that scales a sprite.
* Chapter 4, Exercise 2: Write a program that places a PCX file on the screen and then fades to black when a key is pressed.
* Chapter 4, Exercise 3: Write a function named Sprite_Fizz() that makes a sprite fizzle away in 25 cycles.
* Chapter 4, Exercise 4: When the tanks collide in the Attank!!! demo, nothing happens. Make something spectacular happen when the tanks collide.
* Chapter 4, Exercise 5: Create a starfield that looks like the one in the shuttle.exe program (?); that is, one that swings around. Then, place a sprite of a spaceship with glowing engines in the center of the starfield. (Hint: use color-register animation to make the engines glow.)
