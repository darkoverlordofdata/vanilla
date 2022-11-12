
CFLAGS = -g \
 		-Wall \
		-Wextra \
		-std=c17 \
		-pedantic \
		-I/usr/GNUstep/Local/Library/Headers \
		-I/usr/GNUstep/System/Library/Headers

#		-I/home/darko/GNUstep/Library/Headers \


LDFLAGS = -g \
		-L/usr/GNUstep/Local/Library/Libraries \
		-L/usr/GNUstep/System/Library/Libraries \
		-lpthread \
		-lobjc  \
		-lgnustep-base \
		-lgnustep-gui 

#		-L/home/darko/GNUstep/Library/Libraries \

all: main
main: main.o
	clang  $(CFLAGS) $(LDFLAGS) main.c -o $@

# clang -framework Cocoa -framework OpenGL -fobjc-arc -o $@ $<

	
main.o: main.c

clean:
	rm -f main.o main main.exe


#clang main.m `gnustep-config --objc-flags` `gnustep-config --objc-libs` -lobjc -lgnustep-base -o runGreeterObjc
