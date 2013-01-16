Hi!

Some steps:

1. Get the Android Development Environment ready.
2. Run and see some Android application running.
3. Jump and see some simple application in NDK.
4. Play with it!
5. Run OpenGL ES 1/2 applications.
6. See how toolchain for Android works

7. Revisit how to compile Panda using Ppremake.
8. Make and start to configure a Config.Android.pp
9. Try to cross-compile panda to Android.
10. Get a clean build.

11. Try to get the generated shared library into Android
12. Discover that panda generates too much shared libraries and android complains about that.
13. Make a script to bundle all shared libraries in one (get all *.o, delete unwanted ones and build a nice shared library)

14. See how Native-Activities works.
15. Get into trouble trying to put dependencies to work with Native Activities
16. Make a bootstrap-thing to load the libraries
17. Try to use some very simple feature of the panda shared library (I tested the class Filename)
18. See what happens correct mistakes

19. Study package egldisplay.
20. Discover it's X11 based.
21. Make a simple androiddisplay to start.
22. Make a simple AndroidGraphicsPipe.
23. Make a simple AndroidGraphicsWindows.
24. Make a simple AndroidGraphicsStateGuardian

25. Get something rendered!


Problems:
- No input handling
- No data handling (paths are hardcoded (hardwired?))
- Various scripts
