<p align="center"><img style="text-align:center" src="./media/rendersmall.png" alt="Banner"/></p>

# ![SuperHaxagon](./media/icon-3ds.png "SuperHaxagon Icon") SuperHaxagon - PSP Port

A port of [SuperHaxagon](https://github.com/RedTopper/Super-Haxagon) to the PSP.

SuperHaxagon, like the original game [Super Hexagon](http://superhexagon.com/) by Terry Cavanagh, has only one goal.
Survive as long as possible by avoiding the falling walls in a trippy, spinny frenzy!

Original [SuperHaxagon](https://github.com/RedTopper/Super-Haxagon) is a cross-platform, open source _Super Hexagon_ clone for the Nintendo 3DS, Nintendo Switch, Windows,
Linux, Portmaster, Miyoo Mini, and the [TI-Nspire](https://education.ti.com/en/products/calculators/graphing-calculators/ti-nspire-cx-cas) 
(yes, the calculator). It is developed in C++ and uses no additional libraries for the core logic, making it easy (in theory) 
to port to new platforms. It also has a GUI based level editor called [Haxa Editor](https://github.com/RedTopper/Haxa-Editor).

## Download

The latest release can always be found here:

https://github.com/dntrnk/Super-Haxagon-PSP/releases

## Building

The first step is always to clone this repository with:

```bash
git clone https://github.com/dntrnk/Super-Haxagon-PSP.git
```

Then, open a terminal and run

```bash
make clean && make
```

## Credits

The original game [Super Hexagon](http://superhexagon.com/) by Terry Cavanagh

The people on the [contributors page](https://github.com/RedTopper/Super-Haxagon/graphs/contributors) of original repo (seriously, you guys rock!)

### Assets

Licenses for assets provided by the following people can now be found in [the CREDITS.md file](./CREDITS.md):

 * [TwistBit](https://github.com/TwistBit)
 * [aaronamar](http://fontstruct.com/fontstructions/show/155156/bump_it_up)
 * [Kevin MacLeod](http://incompetech.com/)
 * [Dunderpatrullen](https://dunderpatrullen.nu/)
 * [Bossfight](https://www.bossfightmusic.com/)

### Libraries

 * [PSP Driver](./driver/PSP/LICENSE.md) for PSP

## License

SuperHaxagon is now licensed under GPLv3 (or later) to ensure that any implementation across any platform will
remain open for as long as possible!

This license does not apply to assets or iconography, which have various licenses listed out in 
[the CREDITS.md file](./CREDITS.md).

## Beatmaps

If you are looking to swap out the existing music tracks with your own, you may be wondering what's up with those
`.txt` files next to the tracks. These metadata files provide information to the game about when to time beats,
spins, tilts, zooms, and any other effects while the music is running.

While it _may_ be possible to automatically generate the "beats" from the music itself at runtime,
some low-power platforms don't have the spare CPU cycles to deal with decoding music,
so the best balance is to write out the events manually.

You can open these `.txt` files in [Audacity](https://www.audacityteam.org/) as a
[Label Track](https://manual.audacityteam.org/man/creating_and_selecting_labels.html). If they change, the
events that the game can recognize are found in the `Play::update()` function within `Play.cpp`, and are
currently as follows:

* **S**: A large, sudden spin
* **I**: Invert the background colors
* **BL**: Large beat, when the music is really thumping
* **BS**: Small beat
* **TL**: Tilt the level by moving the main camera to the left
* **TR**: Tilt the level by moving the main camera to the right
* **Z**: A huge, diving zoom.

_(Yes, deleting these files is cheating)_
