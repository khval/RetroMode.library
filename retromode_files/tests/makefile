
VER=-DVER=6

def=		-D__USE_INLINE__

files =	test_app.exe test_loop.exe scroll_text.exe boing.exe

all:		$(files)


%.exe:	%.cpp
		gcc  $(def) $< -o $@

clean:
		delete #?(.exe|.pre)
