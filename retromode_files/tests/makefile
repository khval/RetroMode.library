
VER=-DVER=6

def=		-D__USE_INLINE__

files =	test_app.exe scroll_text.exe boing.exe \
		xor_color_fun.exe \
		screen_effects.exe \
		screen_effects2.exe \
		triangles.exe \
		boxs.exe \
		boxes_rounded.exe \
		lines.exe \
		polyline_polygon.exe \
		sprite.exe

all:		$(files)


%.exe:	%.cpp
		gcc  $(def) $< -o $@

clean:
		delete #?(.exe|.pre)
