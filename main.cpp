// -----------------------------------------------------------------------
// Hello world -- Allegro 5
// -----------------------------------------------------------------------
// Kronoman - In Loving Memory Of My Father
// Aug 2017
// -----------------------------------------------------------------------
#include <stdio.h>
#include <stdlib.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>


const float FPS = 60;

const int POINTS_COUNT = 8;

const int SCREEN_W = 800;
const int SCREEN_H = 600;

class Point
{
	public:
		int x,y;
		int dx,dy; 
		int r;
		ALLEGRO_COLOR c;
		
		void reset()
		{
			x = rand()%SCREEN_W;
			y = rand()%SCREEN_H;
			dx = rand()%10-5;
			dy = rand()%10-5;
			r = rand()%5+2;
			c = al_map_rgb(rand()%256,rand()%256 , rand()%256);
		}
		
		void update()
		{
			x += dx;
			y += dy;
			
			if (x < 0 || x > SCREEN_W)
			{
				c = al_map_rgb(rand()%256,rand()%256 , rand()%256); // al rebotar en los costaos solamente, cambia de color
				dx = -dx;
			}

			if (y < 0 || y > SCREEN_H)		
			{				
				dy = -dy;
				r = rand()%5+2; // en vertical, cambia de tamano
			}		
		}
		
};



int main(int argc, char **argv)
{
	ALLEGRO_DISPLAY *screen = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font = NULL;
	
	bool running = true;
	bool redraw = true;
	
	Point point[POINTS_COUNT];
	

	srand(time(NULL)); // random numbers for animation

	// allegro
	if(!al_init())
	{
		fprintf(stderr, "Failed to initialize Allegro\n");
		return -1;
	}
	
	// image
	if (!al_init_image_addon())
	{
		fprintf(stderr, "ERROR: al_init_image_addon()\n");
		return -1;
	}

	// font
	if (!al_init_font_addon())
	{
		fprintf(stderr, "ERROR: al_init_font_addon()\n");
		return -1;
	}
	
	// keyboard
	if (!al_install_keyboard())
	{
		fprintf(stderr, "ERROR: al_install_keyboard()\n");
		return -1;
	}

	// Initialize the timer
	timer = al_create_timer(1.0 / FPS);
	if (!timer) {
		fprintf(stderr, "Failed to create timer\n");
		return -1;
	}
	
	// Screen
	screen = al_create_display(SCREEN_W, SCREEN_H);
	if (!screen)
	{
		fprintf(stderr, "Failed to create display\n");
		return -1;
	}
	
	// Create the event queue
	event_queue = al_create_event_queue();
	if (!event_queue) 
	{
		fprintf(stderr, "Failed to create event queue");
		return 1;
	}	

	// default font and color
	font = al_create_builtin_font();
	if (!font)
	{
		fprintf(stderr, "Failed to create default font\n");
		return -1;
	}
	
	// primitives graphics
	if (!al_init_primitives_addon())
	{
		fprintf(stderr, "ERROR: al_init_primitives_addon()");
		return 1;
	}

	// Register event sources
	al_register_event_source(event_queue, al_get_display_event_source(screen));
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_keyboard_event_source());

	// Display a black screen
	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_flip_display();

	// Start the timer
	al_start_timer(timer);

	// Game loop
	int x=0, y=0, dx=0, dy=0;
	
	dx=rand()%5+3;
	dy=rand()%5+3;
	
	const char *msg = "Hola mundo Allegro5!";
	
	int w = al_get_text_width(font, msg);
	int h = al_get_font_line_height(font);
	
	
	// create particles
	for (int i=0; i<POINTS_COUNT; i++)
		point[i].reset();
	
	while (running) 
	{
		ALLEGRO_EVENT event;
		ALLEGRO_TIMEOUT timeout;

		// Initialize timeout
		al_init_timeout(&timeout, 0.06);

		// Fetch the event (if one exists)
		bool get_event = al_wait_for_event_until(event_queue, &event, &timeout);

		// Handle the event
		if (get_event) 
		{
			switch (event.type) 
			{
				case ALLEGRO_EVENT_TIMER:
					redraw = true;
					
					x+=dx;
					y+=dy;

					if (x<0) 
						dx=rand()%5+3;
						
					if (y<0)
						dy=rand()%5+3;
					
					if (x>SCREEN_W-w) 
						dx=-(rand()%5+3);
						
					if (y>SCREEN_H-h)
						dy=-(rand()%5+3);

					for (int i=0; i<POINTS_COUNT; i++)
						point[i].update();
				

					
				break;
					
				case ALLEGRO_EVENT_DISPLAY_CLOSE:
					running = false;
				break;
					
				case ALLEGRO_EVENT_KEY_DOWN:
				
				break;
				
				case ALLEGRO_EVENT_KEY_CHAR:
				
				break;
				
				case ALLEGRO_EVENT_KEY_UP:
					running = false;
				break;
					
				default:
					fprintf(stderr, "Unsupported event received: %d\n", event.type);
				break;
			}
		}

		// Check if we need to redraw
		if (redraw && al_is_event_queue_empty(event_queue)) 
		{
			// Redraw
			al_clear_to_color(al_map_rgb(0,0,0));
			al_draw_text(font, al_map_rgb(rand()%256,rand()%256 , rand()%256), x, y, 0, msg);
			
			for (int i=0; i<POINTS_COUNT; i++)
				al_draw_filled_circle(point[i].x, point[i].y, point[i].r, point[i].c);
			
			// lineas
			for (int i=0; i<POINTS_COUNT-1; i++)
			 al_draw_line(point[i].x, point[i].y, point[i+1].x, point[i+1].y, point[i].c, 1); // hasta el ante ultimo
			 
 		    al_draw_line(point[0].x, point[0].y, point[POINTS_COUNT-1].x, point[POINTS_COUNT-1].y, point[0].c, 1); // enlace final
			
			
			al_flip_display();
			redraw = false;
		}
	}



	// cleanup
	al_destroy_font(font);
	al_destroy_display(screen);
	al_destroy_event_queue(event_queue);

	return 0;
}
