CC = clang++
CFLAGS = -fsanitize=address
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lm

build:
	$(CC) $(CFLAGS) -o plotter main.cpp Plotter_app.cpp Coordinate_plane.cpp Vector2d.cpp Matrix3d.cpp $(LDFLAGS)

clean:
	rm -f plotter