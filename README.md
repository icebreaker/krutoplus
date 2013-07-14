Kruto+
======
Kruto+ is an OpenGL (fixed function) based 2D micro-framework.

You should consider it *alpha quality*; take everything with a grain of salt and 
pepper for that matter.

> No shaders were harmed in the making of Kruto+.

Kruto+ has been extracted from the
[Kruto.js](https://github.com/icebreaker/kruto.js) code base.

Getting Started
---------------
In order to get started you need to clone this repository and fetch the
bundled submodules by opening up a Terminal window and issuing the following
commands.

```bash
git clone git://github.com/icebreaker/krutoplus kruto+
cd kruto+
```

Before you start make sure that you installed all the required dependencies.

**Note**: Windows and Mac OSX users you are on your for now, but everything
**should** work just fine without *any* changes.

#### Requirements
* [premake4](http://industriousone.com/premake)
* [SDL 1.2](http://www.libsdl.org/)
* [SDL Image 1.2](http://www.libsdl.org/projects/SDL_image/)
* [SDL Mixer 1.2](http://www.libsdl.org/projects/SDL_mixer/)

##### Premake
Download premake from [industriousone.com](http://industriousone.com/premake/download) 
and copy it to a location in your `PATH`.

```bash
sudo cp premake4 /usr/bin
```

##### SDL
You can install SDL from *source* or via your GNU/Linux distribution's favorite
package manager. Odds are you already have it installed.

Fedora

```bash
sudo yum install SDL-devel SDL_image-devel SDL_mixer-devel SDL_ttf-devel
```

Debian

```bash
sudo apt-get install libsdl1.2-dev libsdl-image1.2-dev libsdl-mixer1.2-dev
```

openSUSE

```bash
sudo zypper install libSDL-devel libSDL_image-devel libSDL_mixer-devel
```

#### Build
Now that you have all the dependencies run the following commands in the
`kruto+` directory.

```bash
premake4 gmake
make config=release
```

If all goes well and the birds sing in your yard you should have some *pretty*
executables in the `build` directory.

Basic Example
-------------
![](https://raw.github.com/icebreaker/krutoplus/master/screenshots/basic.png)

```bash
cd demos/basic
../../build/basic
```

That's all. Enjoy :)

Contribute
----------
* Fork the project.
* Make your feature addition or bug fix.
* Do **not** bump the version number.
* Send me a pull request. Bonus points for topic branches.

License
-------
Copyright (c) 2013, Mihail Szabolcs

Kruto+ is provided **as-is** under the **MIT** license. For more information see
LICENSE.
