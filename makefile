all:
	g++ main.cpp -o main -Wall `pkg-config --cflags --libs allegro-5 allegro_font-5 allegro_image-5 allegro_primitives-5`

