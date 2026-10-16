TARGET = super-haxagon
PSPSDK = $(shell psp-config --pspsdk-path)
PSPDEV = $(shell psp-config --pspdev-path)

INCDIR = \
  $(PSPSDK)/include \
  $(PSPDEV)/psp/include \
  $(PSPDEV)/psp/include/SDL2 \
  driver/All/include \
  driver/PSP/glib2d \
  driver/PSP/aalib \
  driver/PSP/source \
  source

OBJS = \
  driver/All/source/PlatformIsFullyRandom.o \
  driver/All/source/PlatformSupportsFilesystem.o \
  driver/PSP/aalib/pspaalib.o \
  driver/PSP/aalib/pspaalibat3.o \
  driver/PSP/aalib/pspaalibcommon.o \
  driver/PSP/aalib/pspaalibeffects.o \
  driver/PSP/aalib/pspaalibwav.o \
  driver/PSP/source/FontSDL2.o \
  driver/PSP/source/KeybindsSDL2.o \
  driver/PSP/source/MusicSDL2.o \
  driver/PSP/source/PlatformSDL2.o \
  driver/PSP/source/ScreenSDL2.o \
  driver/PSP/source/SoundSDL2.o \
  driver/SDL2-PortMaster/source/PlatformPortMaster.o \
  source/Core/Camera.o \
  source/Core/Game.o \
  source/Core/Matrix.o \
  source/Core/Metadata.o \
  source/Core/Structs.o \
  source/Core/SurfaceGame.o \
  source/Core/SurfaceUI.o \
  source/Factories/LevelFactory.o \
  source/Factories/PatternFactory.o \
  source/Factories/WallFactory.o \
  source/Objects/Level.o \
  source/Objects/Pattern.o \
  source/Objects/Wall.o \
  source/States/Load.o \
  source/States/Menu.o \
  source/States/Over.o \
  source/States/Play.o \
  source/States/Quit.o \
  source/States/Title.o \
  source/States/Transition.o \
  source/States/Win.o \
  source/Main.o

CFLAGS = -G0 -O3 -fomit-frame-pointer -fsingle-precision-constant -fno-math-errno -ffast-math -Wall -Wno-trigraphs -ffunction-sections -fdata-sections
# CFLAGS = -g3 -Wl,-Map,EBOOT.map
CXXFLAGS = $(CFLAGS) -fno-rtti -fno-exceptions -fvisibility=hidden -fvisibility-inlines-hidden
ASFLAGS = $(CFLAGS) -c

BUILD_PRX = 1
PSP_FW_VERSION = 660

LIBS = -Wl,--gc-sections -Wl,--start-group -lpspgu -lpspge -lharfbuzz -lpspdisplay -lpsppower -lpspctrl -lpspaudio -lbz2 -lmodplug -lpspvram \
  -lz -lpng -lphysfs -lpspatrac3 -lstdc++ -ltinyxml2 -lfreetype -lSDL2main -lSDL2 -lGL -lpsphprm -lpspvfpu -lSDL2_ttf -lm -lc -Wl,--end-group

EXTRA_TARGETS = EBOOT.PBP
PSP_EBOOT_TITLE = Super Haxagon
PSP_EBOOT_ICON = media/ICON0.PNG
# PSP_EBOOT_PIC0 = res/PIC0.PNG
# PSP_EBOOT_PIC1 = res/PIC1.PNG

SFOFLAGS = -d MEMSIZE=1

include $(PSPSDK)/lib/build.mak