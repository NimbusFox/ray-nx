#******************************************************************************
#
#  raylib makefile
#
#  Platforms supported:
#    PLATFORM_SWITCH:   Nintendo Switch
#
#  Many thanks to Milan Nikolic (@gen2brain) for implementing Android platform pipeline.
#  Many thanks to Emanuele Petriglia for his contribution on GNU/Linux pipeline.
#
#  Copyright (c) 2014-2019 Ramon Santamaria (@raysan5)
#
#  This software is provided "as-is", without any express or implied warranty.
#  In no event will the authors be held liable for any damages arising from
#  the use of this software.
#
#  Permission is granted to anyone to use this software for any purpose,
#  including commercial applications, and to alter it and redistribute it
#  freely, subject to the following restrictions:
#
#    1. The origin of this software must not be misrepresented; you must not
#    claim that you wrote the original software. If you use this software in a
#    product, an acknowledgment in the product documentation would be
#    appreciated but is not required.
#
#    2. Altered source versions must be plainly marked as such, and must not
#    be misrepresented as being the original software.
#
#    3. This notice may not be removed or altered from any source distribution.
#
#******************************************************************************

# Please read the wiki to know how to compile raylib, because there are different methods.
# https://github.com/raysan5/raylib/wiki

.PHONY: all clean install uninstall

# Define required raylib variables
RAYLIB_VERSION     = 3.0.0
RAYLIB_API_VERSION = 300

# See below for alternatives.
RAYLIB_PATH        = ..

# Define default options

# One of PLATFORM_DESKTOP, PLATFORM_RPI, PLATFORM_ANDROID, PLATFORM_WEB
PLATFORM             = PLATFORM_SWITCH

# Library type used for raylib: STATIC (.a) or SHARED (.so/.dll)
RAYLIB_LIBTYPE       = STATIC

# Build mode for library: DEBUG or RELEASE
RAYLIB_BUILD_MODE    ?= RELEASE

# Include raylib modules on compilation
# NOTE: Some programs like tools could not require those modules
RAYLIB_MODULE_AUDIO  = FALSE
RAYLIB_MODULE_MODELS ?= TRUE
RAYLIB_MODULE_RAYGUI ?= FALSE
RAYLIB_MODULE_PHYSAC ?= FALSE

RAYLIB_MODULE_RAYGUI_PATH ?= .
RAYLIB_MODULE_PHYSAC_PATH ?= .

# Use external GLFW library instead of rglfw module
# TODO: Review usage of examples on Linux.
USE_EXTERNAL_GLFW    = TRUE

# Use Wayland display server protocol on Linux desktop
# by default it uses X11 windowing system
USE_WAYLAND_DISPLAY  ?= FALSE

# See below for more GRAPHICS options.

# See below for RAYLIB_RELEASE_PATH.

# See install target for *_INSTALL_PATH locations.

# Determine if the file has root access (only for installing raylib)
# "whoami" prints the name of the user that calls him (so, if it is the root
# user, "whoami" prints "root").
ROOT = $(shell whoami)

# Define raylib source code path
RAYLIB_SRC_PATH ?= $(RAYLIB_PATH)/src

# Define output directory for compiled library, defaults to src directory
# NOTE: If externally provided, make sure directory exists
RAYLIB_RELEASE_PATH  ?= $(RAYLIB_PATH)/src

GRAPHICS = GRAPHICS_API_OPENGL_33

# Define default C compiler and archiver to pack library
CC = $(DEVKITPRO)/devkitA64/bin/aarch64-none-elf-gcc
AR = $(DEVKITPRO)/devkitA64/bin/aarch64-none-elf-ar


# Define compiler flags:
#  -O1                      defines optimization level
#  -g                       include debug information on compilation
#  -s                       strip unnecessary data from build
#  -Wall                    turns on most, but not all, compiler warnings
#  -std=c99                 defines C language mode (standard C from 1999 revision)
#  -std=gnu99               defines C language mode (GNU C from 1999 revision)
#  -Wno-missing-braces      ignore invalid warning (GCC bug 53119)
#  -D_DEFAULT_SOURCE        use with -std=c99 on Linux and PLATFORM_WEB, required for timespec
#  -Werror=pointer-arith    catch unportable code that does direct arithmetic on void pointers
#  -fno-strict-aliasing     jar_xm.h does shady stuff (breaks strict aliasing)
CFLAGS += -std=gnu99 -Wall -D_DEFAULT_SOURCE -DPLATFORM_DESKTOP -DPLATFORM_SWITCH -Wno-missing-braces -Werror=pointer-arith -fno-strict-aliasing

CFLAGS += -specs=${DEVKITPRO}/libnx/switch.specs -march=armv8-a -mtune=cortex-a57 -mtp=soft -fPIE -L${DEVKITPRO}/libnx/lib -L${DEVKITPRO}/portlibs/switch/lib

ifeq ($(RAYLIB_BUILD_MODE),DEBUG)
    CFLAGS += -g
endif

# Use Wayland display on Linux desktop
ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(PLATFORM_OS), LINUX)
        ifeq ($(USE_WAYLAND_DISPLAY),TRUE)
            CFLAGS += -D_GLFW_WAYLAND
        else
            LDLIBS = -lX11
        endif
    endif
endif

# Define include paths for required headers
# NOTE: Several external required libraries (stb and others)
INCLUDE_PATHS = -I. -I${DEVKITPRO}/libnx/include -I${DEVKITPRO}/portlibs/switch/include

LDFLAGS += -lglfw3 -lEGL -ldrm_nouveau -lglapi -lGLESv1_CM -lGLESv2 -lnxd -ldeko3dd -ldeko3d -lnx -lm -lpthread

# Define all object files required with a wildcard
# The wildcard takes all files that finish with ".c",
# and replaces extentions with ".o", that are the object files
# NOTE: Some objects depend on the PLATFORM to be added or not!
# OBJS = $(patsubst %.c, %.o, $(wildcard *.c))

# Define object required on compilation
OBJS = core.o \
       shapes.o \
       textures.o \
       text.o \
       utils.o

ifeq ($(PLATFORM),PLATFORM_DESKTOP)
    ifeq ($(USE_EXTERNAL_GLFW),FALSE)
        OBJS += rglfw.o
    endif
endif
ifeq ($(RAYLIB_MODULE_MODELS),TRUE)
    OBJS += models.o
endif
ifeq ($(RAYLIB_MODULE_AUDIO),TRUE)
    OBJS += raudio.o
endif
ifeq ($(RAYLIB_MODULE_RAYGUI),TRUE)
    OBJS += raygui.o
endif
ifeq ($(RAYLIB_MODULE_PHYSAC),TRUE)
    OBJS += physac.o
endif

ifeq ($(PLATFORM),PLATFORM_ANDROID)
    OBJS += $(NATIVE_APP_GLUE)/android_native_app_glue.o
endif

# Default target entry
all: raylib

# Compile raylib library
# NOTE: Release directory is created if not exist
raylib: $(OBJS)
ifeq ($(PLATFORM),PLATFORM_WEB)
    # Compile raylib for web.
	$(CC) -O1 $(OBJS) -o $(RAYLIB_RELEASE_PATH)/libraylib.bc
	@echo "raylib library generated (libraylib.bc)!"
else
        # Compile raylib static library version $(RAYLIB_VERSION)
        # WARNING: You should type "make clean" before doing this target.
		$(AR) rcs $(RAYLIB_RELEASE_PATH)/libraylibnx.a $(OBJS)
		@echo "raylib static library generated (libraylibnx.a) in $(RAYLIB_RELEASE_PATH)!"
endif

# Compile all modules with their prerequisites

# Compile core module
core.o : core.c raylib.h rlgl.h utils.h raymath.h camera.h gestures.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -D$(GRAPHICS)

# Compile rglfw module
rglfw.o : rglfw.c
	$(CC) $(CFLAGS) $(GLFW_CFLAGS) -c $<  $(INCLUDE_PATHS) -D$(PLATFORM) -D$(GRAPHICS)

# Compile shapes module
shapes.o : shapes.c raylib.h rlgl.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -D$(GRAPHICS)

# Compile textures module
textures.o : textures.c raylib.h rlgl.h utils.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -D$(GRAPHICS)

# Compile text module
text.o : text.c raylib.h utils.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -D$(GRAPHICS)

# Compile utils module
utils.o : utils.c utils.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

# Compile models module
models.o : models.c raylib.h rlgl.h raymath.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -D$(GRAPHICS)

# Compile audio module
raudio.o : raudio.c raylib.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM)

# Compile raygui module
# NOTE: raygui header should be distributed with raylib.h
raygui.o : raygui.c raygui.h gui_textbox_extended.h ricons.h
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -DRAYGUI_IMPLEMENTATION
raygui.c:
	echo '#define RAYGUI_IMPLEMENTATION' > raygui.c
	echo '#include "$(RAYLIB_MODULE_RAYGUI_PATH)/raygui.h"' >> raygui.c

# Compile physac module
# NOTE: physac header should be distributed with raylib.h
physac.o : physac.c physac.h
	@echo #define PHYSAC_IMPLEMENTATION > physac.c
	@echo #include "$(RAYLIB_MODULE_PHYSAC_PATH)/physac.h" > physac.c
	$(CC) -c $< $(CFLAGS) $(INCLUDE_PATHS) -D$(PLATFORM) -DPHYSAC_IMPLEMENTATION


# Install generated and needed files to desired directories.
# On GNU/Linux and BSDs, there are some standard directories that contain extra
# libraries and header files. These directories (often /usr/local/lib and
# /usr/local/include) are for libraries that are installed manually
# (without a package manager). We'll use /usr/local/lib/raysan5 and /usr/local/include/raysan5
# for our -L and -I specification to simplify management of the raylib source package.
# Customize these locations if you like but don't forget to pass them to make
# for compilation and enable runtime linking with -rpath, LD_LIBRARY_PATH, or ldconfig.
# Hint: add -L$(RAYLIB_INSTALL_PATH) -I$(RAYLIB_H_INSTALL_PATH) to your own makefiles.
# See below and ../examples/Makefile for more information.
# TODO: Add other platforms. Remove sudo requirement, i.e. add USER mode.

# RAYLIB_INSTALL_PATH should be the desired full path to libraylib. No relative paths.
DESTDIR ?= /usr/local
RAYLIB_INSTALL_PATH ?= $(DESTDIR)/lib
# RAYLIB_H_INSTALL_PATH locates the installed raylib header and associated source files.
RAYLIB_H_INSTALL_PATH ?= $(DESTDIR)/include

install :
ifeq ($(ROOT),root)
    ifeq ($(PLATFORM_OS),LINUX)
        # Attention! You are root, writing files to $(RAYLIB_INSTALL_PATH)
        # and $(RAYLIB_H_INSTALL_PATH). Consult this Makefile for more information.
        # Prepare the environment as needed.
		mkdir --parents --verbose $(RAYLIB_INSTALL_PATH)
		mkdir --parents --verbose $(RAYLIB_H_INSTALL_PATH)
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
            # Installing raylib to $(RAYLIB_INSTALL_PATH).
			cp --update --verbose $(RAYLIB_RELEASE_PATH)/libraylib.so.$(RAYLIB_VERSION) $(RAYLIB_INSTALL_PATH)/libraylib.so.$(RAYLIB_VERSION)
			cd $(RAYLIB_INSTALL_PATH); ln -fsv libraylib.so.$(RAYLIB_VERSION) libraylib.so.$(RAYLIB_API_VERSION)
			cd $(RAYLIB_INSTALL_PATH); ln -fsv libraylib.so.$(RAYLIB_API_VERSION) libraylib.so
            # Uncomment to update the runtime linker cache with RAYLIB_INSTALL_PATH.
            # Not necessary if later embedding RPATH in your executable. See examples/Makefile.
			ldconfig $(RAYLIB_INSTALL_PATH)
        else
            # Installing raylib to $(RAYLIB_INSTALL_PATH).
			cp --update --verbose $(RAYLIB_RELEASE_PATH)/libraylib.a $(RAYLIB_INSTALL_PATH)/libraylib.a
        endif
        # Copying raylib development files to $(RAYLIB_H_INSTALL_PATH).
		cp --update raylib.h $(RAYLIB_H_INSTALL_PATH)/raylib.h
		cp --update raymath.h $(RAYLIB_H_INSTALL_PATH)/raymath.h
		cp --update rlgl.h $(RAYLIB_H_INSTALL_PATH)/rlgl.h
		cp --update physac.h $(RAYLIB_H_INSTALL_PATH)/physac.h
		@echo "raylib development files installed/updated!"
    else
		@echo "This function currently works on GNU/Linux systems. Add yours today (^;"
    endif
else
	@echo "Error: Root permissions needed for installation. Try sudo make install"
endif

# Remove raylib dev files installed on the system
# TODO: see 'install' target.
uninstall :
ifeq ($(ROOT),root)
    # WARNING: You are root, about to delete items from $(RAYLIB_INSTALL_PATH).
    # and $(RAYLIB_H_INSTALL_PATH). Please confirm each item.
    ifeq ($(PLATFORM_OS),LINUX)
        ifeq ($(RAYLIB_LIBTYPE),SHARED)
		rm --force --interactive --verbose $(RAYLIB_INSTALL_PATH)/libraylib.so
		rm --force --interactive --verbose $(RAYLIB_INSTALL_PATH)/libraylib.so.$(RAYLIB_API_VERSION)
		rm --force --interactive --verbose $(RAYLIB_INSTALL_PATH)/libraylib.so.$(RAYLIB_VERSION)
        # Uncomment to clean up the runtime linker cache. See install target.
		ldconfig
        else
		rm --force --interactive --verbose $(RAYLIB_INSTALL_PATH)/libraylib.a
        endif
		rm --force --interactive --verbose $(RAYLIB_H_INSTALL_PATH)/raylib.h
		rm --force --interactive --verbose $(RAYLIB_H_INSTALL_PATH)/raymath.h
		rm --force --interactive --verbose $(RAYLIB_H_INSTALL_PATH)/rlgl.h
		rm --force --interactive --verbose $(RAYLIB_H_INSTALL_PATH)/physac.h
		@echo "raylib development files removed!"
        else
		@echo "This function currently works on GNU/Linux systems. Add yours today (^;"
    endif
else
	@echo "Error: Root permissions needed for uninstallation. Try sudo make uninstall"
endif

# Clean everything
clean:
ifeq ($(PLATFORM_OS),WINDOWS)
	del *.o $(RAYLIB_RELEASE_PATH)/libraylib.a $(RAYLIB_RELEASE_PATH)/libraylib.bc $(RAYLIB_RELEASE_PATH)/libraylib.so
else
	rm -fv *.o $(RAYLIB_RELEASE_PATH)/libraylib.a $(RAYLIB_RELEASE_PATH)/libraylib.bc $(RAYLIB_RELEASE_PATH)/libraylib.so*
endif
ifeq ($(PLATFORM),PLATFORM_ANDROID)
	rm -rf $(ANDROID_TOOLCHAIN) $(NATIVE_APP_GLUE)/android_native_app_glue.o
endif
	@echo "removed all generated files!"
