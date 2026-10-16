TARGET = super-haxagon
PSPSDK = $(shell psp-config --pspsdk-path)
PSPDEV = $(shell psp-config --pspdev-path)

INCDIR = \
  $(PSPSDK)/include \
  $(PSPDEV)/psp/include \
  $(PSPDEV)/psp/include/SDL2 \
  driver/All/include \
  driver/PSP/source \
  source

OBJS = \
  driver/All/source/PlatformIsFullyRandom.cpp \
  driver/All/source/PlatformSupportsFilesystem.cpp \
  driver/PSP/source/FontSDL2.cpp \
  driver/PSP/source/KeybindsSDL2.cpp \
  driver/PSP/source/MusicSDL2.cpp \
  driver/PSP/source/PlatformSDL2.cpp \
  driver/PSP/source/ScreenSDL2.cpp \
  driver/PSP/source/SoundSDL2.cpp \
  driver/SDL2-PortMaster/source/PlatformPortMaster.cpp \
  source/Core/Camera.cpp \
  source/Core/Game.cpp \
  source/Core/Matrix.cpp \
  source/Core/Metadata.cpp \
  source/Core/Structs.cpp \
  source/Core/SurfaceGame.cpp \
  source/Core/SurfaceUI.cpp \
  source/Factories/LevelFactory.cpp \
  source/Factories/PatternFactory.cpp \
  source/Factories/WallFactory.cpp \
  source/Objects/Level.cpp \
  source/Objects/Pattern.cpp \
  source/Objects/Wall.cpp \
  source/States/Load.cpp \
  source/States/Menu.cpp \
  source/States/Over.cpp \
  source/States/Play.cpp \
  source/States/Quit.cpp \
  source/States/Title.cpp \
  source/States/Transition.cpp \
  source/States/Win.cpp \
  source/Main.cpp

CFLAGS = -G0 -O3 -fomit-frame-pointer -fsingle-precision-constant -fno-math-errno -ffast-math -Wall -Wno-trigraphs -ffunction-sections -fdata-sections
# CFLAGS = -g3 -Wl,-Map,EBOOT.map
CXXFLAGS = $(CFLAGS) -fno-rtti -fno-exceptions -fvisibility=hidden -fvisibility-inlines-hidden
ASFLAGS = $(CFLAGS) -c

BUILD_PRX = 1
PSP_FW_VERSION = 660

LIBS = -Wl,--gc-sections -Wl,--start-group -lpspgu -lpspge -lharfbuzz -lpspdisplay -lpsppower -lpspctrl -lpspaudio -lbz2 -lmodplug -lpspvram \
  -lz -lpng -lphysfs -lvorbis -lvorbisidec -lpspatrac3 -lstdc++ -ltinyxml2 -lfreetype -lSDL2main -lSDL2 -lGL -logg -lpsphprm -lpspvfpu -lSDL2_ttf -lSDL2_mixer -lm -lc -Wl,--end-group

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Super Haxagon
PSP_EBOOT_ICON = media/ICON0.PNG
# PSP_EBOOT_PIC0 = res/PIC0.PNG
# PSP_EBOOT_PIC1 = res/PIC1.PNG

SFOFLAGS = -d MEMSIZE=1

include $(PSPSDK)/lib/build.mak