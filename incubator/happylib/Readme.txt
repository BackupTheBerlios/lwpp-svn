
Welcome to HappyLib!
--------------------

HappyLib contains, among other things,
GLYPH's C++ wrapper for the LightWave 6.5 / 7.5 SDK.


Why
---

The main reason to write a library of code such as this is code re-use.
Code re-use is a Good Thing.  The more a piece of code is used, the
sooner any existing bugs will be found and squashed.  And when a bug is
squashed, it is squashed everywhere the buggy code was used, which is
lots of places.  This is in stark contrast to "cut-and-paste" coding.
Second, code re-use increases the pay-off for optimizing a piece of code
that's used a lot.  Optimizations can be made that would be too annoying
to code over and over in the "cut-and-paste" model.  Third, code re-use
reduces program size and development time, on average.

The next biggest reason is to take the tedious tasks required to use the
LightWave SDK and hide them in a bunch of easy-to-use classes. The whole wrapper is lightweight and mostly compiles away to nothing.


Status
------

Release date:  Jan/10/2005

HappyLib Classes to date:
    Array, Array2, Bitfield, BitWrapper, Cache, Color, ColorSpace, List,
    Matrix, Matrix3x3, Matrix4x4, Monitor, MultiMutex, Mutex, Point,
    Quaternion, Queue, String, Stack, Thread, Timer, Vector, WeeFloat

LightWaveClass Classes to date:
    (none)

LightWavePlugin Classes to date:
    LWPChannel
    LWPCustomObject
    LWPDisplacement
    LWPGeneric
    LWPImageFilter
    LWPMaster
    LWPMeshTool
    LWPPixelFilter
    LWPShader
    LWPTexture
    LWPVolume

LightWavePlugin Helper Classes to date:
    LWPGradient
    LWPLoadState
    LWPSaveState
    LWPPanel
    LWPControl
    LWPValue (and derived classes)
    LWPControlDesc (and derived classes)
    LWPItem, LWPObject, LWPLight, LWPBone, LWPCamera, etc.
    LWPScene
    LWPSerialization - a really easy to use serialization system
    LWPSurface
    LWPSurfaceData
    LWPChanGroup
    LWPEnvelope
    LWPMicroPolygon
    LWPVParm
    LWPTextureSpace
    (and many more)

Compilers Tested to date:
Intel:
    Visual C++ 6.0 - works great


News
----

Jan/10/2005

Wow, been a while since an update.  Lots of new goodies.

May/16/2003

Many little tweaks and fixes in the code.

Jan/16/2003

Officially moved to LW7.5 SDK wrapping.  Fortunately, the wrapper
automatically detects and functions with the LW6.5 SDK too.
This is the first HappyLib to support two separate SDK versions!  Woo!

More updates.  Added LWPChannel, LWPCustomObject, LWPMaster, LWPMeshTool.
Some minor bugfixes.

Feb/20/2002

Some updates.  Thread code somewhat rewritten.
Added LWPVolume, LWPPixelFilter.

Nov/13/2001

It's been a while.  Many improvements have been made.  Some new files
are in here, like HLUtils.h, HLIOUtils.h, HLTrig.h, HLColorSpace.h.

Variant parameters, panels and panel controls are wrapped all
pretty-like (see LWPPanel.h) though no XPanel wrapper yet :(

Texture stuff is wrapped all pretty-like (see LWPTextureUtils.h)

Dec/27/2000

Officially dropped LW5.6 SDK wrapping.  Old wrapper is still kicking
around, but I won't be developing it much anymore.  The 6.5 SDK is
far too different to have a joint wrapper, and demand for 5.6 plug-ins
seems to be less than 15%, so it's not really worth adding to it.

On a brighter note, the 6.5 SDK is pretty sweet, so the wrapper is
super-sweet!


Usage
-----

Make sure you read the disclaimer at the bottom of this document!

Seriously, have you read it yet?

Okay, here it is again:

----------
DISCLAIMER
----------
THERE IS NO WARRANTEE, EXPRESSED OR IMPLIED, ON THIS SOFTWARE.  USE IT
AT YOUR OWN RISK.  NO OFFICIAL SUPPORT IS PROVIDED WITH THIS PRODUCT.
Report bugs or problems to graham@happy-digital.com, but I can't make
any promises.

Now for the documentation.

HappyLib is really three sets of source files:

1) HappyLib files (with HL prefix)
   This is a set of classes and utilities for general C++ programming.
   It includes classes specifically for 3D programming (Point, Color,
   Vector, Matrix, etc.) as well as more generic classes (Stack, Queue,
   Array, etc.)

2) LightWave Class files (with LWC prefix)
   This is a set of classes and utilities to make it easier to handle
   LightWave data in external programs.  It is not dependant on the
   LW SDK.  (Currently there are no files with this prefix)

3) LightWave Plug-in files (with LWP prefix)
   This is a very cool C++ wrapper for the LW SDK.  It uses the
   equally cool classes from the HappyLib files.  It has very low
   overhead, since most of the class stuff gets compiled away.  Its
   job is to make my life easier, but it may also make yours easier.

I've included some demos with Visual C++ 6.0 workspaces, so some of you
can get started right away!  These workspaces may also work under VC 5.
This stuff should, in theory, compile under other C++ compilers and on
other platforms.  If you get it to do so, e-mail your project file or
settings or whatever to graham@happy-digital.com.  I'll be sure and add
them to future releases.

You'll need the LightWave SDK to use the LWP stuff.  The LightWave SDK
source files will have to be in your Includes path for your compiler.
Also, you may need to remove and re-add the source files from the demo
projects, since they may be in a different directory on your machine.

Have a look at the demo projects to see how the wrapper is used.  Go
ahead and base your plug-ins off the examples.  That's why they're there.

e-mail any questions to graham@happy-digital.com

See yah!

- GLYPH


History
-------
05/16/2003 - Many little tweaks and fixes in the code.
01/16/2003 - Officially moved to LW7.5 SDK wrapping.  Still works with LW6.5 SDK as well.
             Added LWPChannel, LWPCustomObject, LWPMaster, LWPMeshTool.
             Some minor bugfixes.
02/15/2002 - Thread code somewhat rewritten.
             Added classes LWPVolume, LWPPixelFilter.
11/13/2001 - Many, many improvements and bug-fixes (It's been ten months!)
             Added / improved HLUtils.h, HLIOUtils.h, HLTrig.h, HLColorSpace.h.
             Wrapped variant parameters, panels and panel controls (LWPPanel.h)
             Wrapped texture stuff (see LWPTextureUtils.h)
             Added classes LWPGeneric, LWPImageFilter, LWPTexture.
01/05/2001 - Fixed bug in Serialization that caused int values to save incorrectly.
01/04/2001 - Improved Serialization (transparently) for better modularity.
             Some files have been renamed / removed, so you should delete the old HappyLib
             files before copying the new one over it.
12/27/2000 - Switched over to LW 6.5 SDK.  5.6 support dropped.
             Wrapper is leaner and meaner.  Less overhead.
             Basically starting plug-ins over, currently supports Shader and Displacement.
10/04/2000 - Added LWPItemMotion, LWPObjectReplacement, LWPLayoutGeneric.
             Fixed some bugs in VectorT.
             Added more control classes to Interface.
05/28/2000 - Now expects SDK to be found in a "lwsdk5_6" directory.
             Some internal changes, getting ready to support L6 SDK.
             Added noise.h and noise.cpp, mostly borrowed from the Shades project.
             Removed a lot of overhead in Shader.
05/10/2000 - Fixed up the Interface classes a lot.
04/14/2000 - Added quaternion.h.
             Added a lot of code to matrix.h, including determinant() and adjoint().
             Added documentation of conventions for point.h and matrix.h.
04/13/2000 - Cleaned up VectorT, PointT, ColorT.  Added TripleT superclass for code reuse.
             More prominent use of enums for array indeces to reduce confusion.
             Split point.h into point.h, matrix.h, weefloat.h.
             Cleaned up defs.h; moved some code around.
04/09/2000 - Fixed a memory leak in LWPObject.
04/08/2000 - Fixed version checking.  Now asks for LW5.5 handlers.
04/07/2000 - Fixed 3D random function.  Now much more random, and a wee bit faster!
04/04/2000 - Fixed several bugs in the LWPShader::shade function, mainly ambient handling.
03/23/2000 - Added lots of (undocumented) handy code to MeshEdit.
02/29/2000 - Made code somewhat more ANSI-compliant.
02/07/2000 - Added automagic CUT / PASTE functionality to Interfaces,
             using ctrl-c / ctrl-v.
02/06/2000 - Starting to integrate Bryan Ewert's LWClass code.
             Thanks Bryan!!!
02/03/2000 - Fixed bug that caused interfaces to sometimes be extra wide.
01/27/2000 - Fixed templates to work with VisualC++5.x
01/21/2000 - Major bug fixed in Queue.
01/19/2000 - Changed server record registration to be platform independant.
01/18/2000 - Fixed some more bugs in Shader.
01/15/2000 - Templatized Point, Vector, Color, and Matrix.
             Fixed a bug in Queue.
01/06/2000 - Fixed more bugs wrt multithreading and rayShade.
01/04/2000 - Cleaned up a lot of garbage in the Shader code.
01/03/2000 - Added MeshEdit (modeler) wrapper.
12/28/1999 - Added Displacement map wrapper.
             Added Lwob, which wraps the ObjectAccess stuff.
12/20/1999 - Added ImageFilter wrapper.
           - Fixed Illumination / RayTracing recursion problems.
12/19/1999 - Added PixelFilter wrapper.
12/16/1999 - First public release.  Shader supported.


Known Bugs / Issues
-------------------

12/27/2000 - Some stuff only supported under WIN32 (e.g. HLMutex.cpp)
01/17/2000 - There is a problem compiling it under the SGI C++ compiler.
             It seems the only LightWave-compatible compiler on the SGI
             is not fully C++ compliant.  This will take some thought.
01/01/2000 - There is very little documentation :)


Credits
-------
Bryan Ewert - for letting me integrate his LWClass code into the project.

Graphics Gems - some of the quaternion code is based on Graphics Gems code.
                Specific routines are credited to the original authors.

The Shades Project - for the noise code that I stole :)

Graham Fyffe - That's me! :)

Everybody Who Sends Bug Reports - thanks!


----------------------------
DISTRIBUTION AND USAGE NOTES
----------------------------
You may use and / or modify this stuff for your own plug-ins, or for
commercial plug-ins, provided you thank me (Graham Fyffe) and Bryan
(Bryan Ewert) somewhere in the "about" menu or in the documentation.

You may redistribute this stuff as long as it's in the original,
unmodified zip file.  If you'd like something added, or something
changed, please e-mail me at graham@happy-digital.com with the details.
I'm trying to keep this thing more or less open-source, but I don't want
multiple versions of it floating around out there.

There is now a mailing list for discussing HappyLib!  You can make
suggestions, report bugs, etc. here.  Simply send a blank e-mail to the
following address to subscribe:

subscribe-happylib@egroups.com


----------
DISCLAIMER
----------
THERE IS NO WARRANTEE, EXPRESSED OR IMPLIED, ON THIS SOFTWARE.  USE IT
AT YOUR OWN RISK.  NO OFFICIAL SUPPORT IS PROVIDED WITH THIS PRODUCT.
Report bugs or problems to graham@happy-digital.com, but I can't make
any promises.

