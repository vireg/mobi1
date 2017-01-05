CROSS_COMPILE=/home/vrege/Android/Sdk/ndk-bundle/toolchains/arm-linux-androideabi-4.9/prebuilt/linux-x86_64/bin/arm-linux-androideabi-
NDKPATH=/home/vrege/Android/Sdk/ndk-bundle/platforms/android-19/
LIBDIR=$(NDKPATH)arch-arm/usr/lib
INCDIR=$(NDKPATH)arch-arm/usr/include

AS		= $(CROSS_COMPILE)as
CC 		= $(CROSS_COMPILE)gcc
AR 		= $(CROSS_COMPILE)ar
LD 		= $(CROSS_COMPILE)ld
NM 		= $(CROSS_COMPILE)nm
RANLIB 	= $(CROSS_COMPILE)ranlib
STRIP 	= $(CROSS_COMPILE)strip

INCLUDE = -I. -I$(INCDIR) 

CFLAGS = -Wall -fno-short-enums -g
LDFLAGS = -nostdlib -Wl,--dynamic-linker,"/system/bin/linker" -L$(LIBDIR) -lc -lm -lstdc++ -ldl -lgcc -llog

CRTBEGIN = $(LIBDIR)/crtbegin_dynamic.o
CRTEND = $(LIBDIR)/crtend_android.o

TARGET	= measure_power
SRCS	= measure_power.c
OBJS = $(SRCS:.c=.o)

# define the rule
.SUFFIXES:.c .o 

.c.o:
	@echo Compiling: $< 
	$(CC) -c $(CFLAGS)  $(INCLUDE) -o $@ $<

all: app

app: $(OBJS)
	@echo Linking: $(TARGET) 
	$(CC) -o $(TARGET) $(OBJS) $(LDFLAGS) $(CRTBEGIN) $(CRTEND)
	$(STRIP) -s $(TARGET)

clean:
	@rm -vf $(OBJS) $(TARGET) 
