// Project includes
#include "globals.h"
#include "hardware.h"
#include "map.h"
#include "graphics.h"
#include "speech.h"

#include "mbed.h"
#include "print.h"

// Functions in this file
int get_action (GameInputs inputs);
int update_game (int action);
void draw_game (int init);
void init_main_map ();
int main ();

int action_button();

int go_up();
int go_down();
int go_right();
int go_left();

void game_over();
void reset_game();
void game();

int readPB1(int pb1);
void f_wait();

/**
 * The main game state. Must include Player locations and previous locations for
 * drawing to work properly. Other items can be added as needed.
 */
struct {
    int x,y;    // Current locations
    int px, py; // Previous locations
    int has_key;
    
    int healph;
    int p_healph;
    
    int life;
    int p_life;
    int mushroom;
    
    bool key;
    bool leaf;
} Player;

/**
 * Given the game inputs, determine what kind of update needs to happen.
 * Possbile return values are defined below.
 */
#define NO_ACTION 0
#define ACTION_BUTTON 1
#define MENU_BUTTON 2
#define GO_LEFT 3
#define GO_RIGHT 4
#define GO_UP 5
#define GO_DOWN 6

DigitalOut test_led(LED1);

int get_action(GameInputs inputs)
{   
    if(inputs.b2 == 0)
    {
        omnipotent = !omnipotent;
        test_led = !test_led;
        wait(1.0);
    }
    
    if (inputs.b1 == 0)
    return ACTION_BUTTON;
    
    if (inputs.ay > 0.30) return GO_UP;
    if (inputs.ay < -0.30) return GO_DOWN;
    
    if (inputs.ax > 0.30) return GO_RIGHT;
    if (inputs.ax < -0.30) return GO_LEFT;
    
    return NO_ACTION;
}

/**
 * Update the game state based on the user action. For example, if the user
 * requests GO_UP, then this function should determine if that is possible by
 * consulting the map, and update the Player position accordingly.
 * 
 * Return values are defined below. FULL_DRAW indicates that for this frame,
 * draw_game should not optimize drawing and should draw every tile, even if
 * the player has not moved.
 */
#define NO_RESULT 0
#define GAME_OVER 1
#define FULL_DRAW 2
#define HURT      3
#define WON       4
int update_game(int action)
{
    // Save player previous location before updating
    Player.px = Player.x;
    Player.py = Player.y;
    
    Player.p_healph = Player.healph;
    
    // Do different things based on the each action.
    // You can define functions like "go_up()" that get called for each case.
    
    
    switch(action)
    {
        case GO_UP:             return go_up();
        case GO_DOWN:           return go_down();     
        case GO_RIGHT:          return go_right();
        case GO_LEFT:           return go_left();     
        
        case ACTION_BUTTON:     return action_button();
        case MENU_BUTTON:       break;
        default:                break;
    }
    return NO_RESULT;
}

int action_button()
{
    
    // interaction with old batman
    if (get_north(Player.x, Player.y) -> type == BATMAN || get_south(Player.x, Player.y) -> type == BATMAN || get_east(Player.x, Player.y) -> type == BATMAN || get_west(Player.x, Player.y) -> type == BATMAN)
    {
        const char* line1 = "Hey kid.. If you find all my mushrooms I'll reward you with a special prize..\n";
        const char* line2 = "Go on kid, keep looking for than..\n";
        const char* line3 = "Full life and full healph for you!\n";
        const char* line4 = "All right kid. Charity time is over. Go away.\n";
        
        if      (Player.mushroom == 0)  {speech(line1);}
        
        else if (Player.mushroom > 0 && Player.mushroom < 5)   {speech(line2);}
        
        else if (Player.mushroom == 5)
        {
            if      (get_north(Player.x, Player.y) -> data == false)  {get_north(Player.x, Player.y) -> data = true; speech(line3); Player.life = Player.life + 1; Player.healph = 5;}
            else if (get_south(Player.x, Player.y) -> data == false)  {get_south(Player.x, Player.y) -> data = true; speech(line3); Player.life = Player.life + 1; Player.healph = 5;}
            else if (get_east(Player.x, Player.y)  -> data == false)  {get_east(Player.x, Player.y)  -> data = true; speech(line3); Player.life = Player.life + 1; Player.healph = 5;}
            else if (get_west(Player.x, Player.y)  -> data == false)  {get_west(Player.x, Player.y)  -> data = true; speech(line3); Player.life = Player.life + 1; Player.healph = 5;}
            
            else {speech(line4);}
        }
        
        return FULL_DRAW;
    }
    
    // interaction with mushroom
    if (get_north(Player.x, Player.y) -> type == MUSHROOM || get_south(Player.x, Player.y) -> type == MUSHROOM || get_east(Player.x, Player.y) -> type == MUSHROOM || get_west(Player.x, Player.y) -> type == MUSHROOM)
    {
        speech("You found a mushroom!");
        
        Player.mushroom = Player.mushroom + 1;
        
        if      (get_north(Player.x, Player.y) -> type == MUSHROOM)  map_erase(Player.x    , Player.y - 1);
        else if (get_south(Player.x, Player.y) -> type == MUSHROOM)  map_erase(Player.x    , Player.y + 1);
        else if (get_east(Player.x, Player.y) -> type == MUSHROOM)   map_erase(Player.x + 1, Player.y    );
        else if (get_west(Player.x, Player.y) -> type == MUSHROOM)   map_erase(Player.x - 1, Player.y    );
        
        return FULL_DRAW;
    }   
    
    // interaction with leaf
    if (get_north(Player.x, Player.y) -> type == LEEF || get_south(Player.x, Player.y) -> type == LEEF || get_east(Player.x, Player.y) -> type == LEEF || get_west(Player.x, Player.y) -> type == LEEF)
    {
        speech("You have the magical leaf. Go back to the old man");
        
        Player.leaf = true;
        
        if      (get_north(Player.x, Player.y) -> type == LEEF)  map_erase(Player.x    , Player.y - 1);
        else if (get_south(Player.x, Player.y) -> type == LEEF)  map_erase(Player.x    , Player.y + 1);
        else if (get_east(Player.x, Player.y) -> type == LEEF)   map_erase(Player.x + 1, Player.y    );
        else if (get_west(Player.x, Player.y) -> type == LEEF)   map_erase(Player.x - 1, Player.y    );
        
        return FULL_DRAW;
    }
    
    // interaction with key
    if (get_north(Player.x, Player.y) -> type == KEY || get_south(Player.x, Player.y) -> type == KEY || get_east(Player.x, Player.y) -> type == KEY || get_west(Player.x, Player.y) -> type == KEY)
    {
        speech("You gotta a key");
        
        Player.key = true;
        
        if      (get_north(Player.x, Player.y) -> type == KEY)  map_erase(Player.x    , Player.y - 1);
        else if (get_south(Player.x, Player.y) -> type == KEY)  map_erase(Player.x    , Player.y + 1);
        else if (get_east(Player.x, Player.y) -> type == KEY)   map_erase(Player.x + 1, Player.y    );
        else if (get_west(Player.x, Player.y) -> type == KEY)   map_erase(Player.x - 1, Player.y    );
        
        return FULL_DRAW;
    }
    
    // interaction with old man
    if (get_north(Player.x, Player.y) -> type == OLD_MAN || get_south(Player.x, Player.y) -> type == OLD_MAN || get_east(Player.x, Player.y) -> type == OLD_MAN || get_west(Player.x, Player.y) -> type == OLD_MAN)
    {
        const char* line1 = "My son, I'm very sick and I neded my medicined. It is a very special medicine that I keep loocked at all times. The problem is: I misplace my key somewhere. Please go find the key and bring my medice, otherwise I'll die very soon.\n";
        
        const char* line2 = "Go my soon, go get me that medicine\n";
        
        const char* line3 = "Thank you my soon. You saved my life.\n";
        
        if      (get_north(Player.x, Player.y) -> data == false)  {get_north(Player.x, Player.y) -> data = true; speech(line1);}
        else if (get_south(Player.x, Player.y) -> data == false)  {get_south(Player.x, Player.y) -> data = true; speech(line1);}
        else if (get_east(Player.x, Player.y)  -> data == false)  {get_east(Player.x, Player.y)  -> data = true; speech(line1);}
        else if (get_west(Player.x, Player.y)  -> data == false)  {get_west(Player.x, Player.y)  -> data = true; speech(line1);}
        
        else if (Player.leaf == false) {speech(line2);}
        else    {speech(line3); return WON;}
        
        return FULL_DRAW;
    }
    
    // interact with door
    if (get_north(Player.x, Player.y) -> type == XDOOR || get_south(Player.x, Player.y) -> type == XDOOR || get_east(Player.x, Player.y) -> type == XDOOR || get_west(Player.x, Player.y) -> type == XDOOR)
    {    
        if (Player.key == true)
        {   
            speech("The door is now open");
            
            if      (get_north(Player.x, Player.y) -> type == XDOOR)    map_erase(Player.x    , Player.y - 1);
            else if (get_south(Player.x, Player.y) -> type == XDOOR)    map_erase(Player.x    , Player.y + 1);
            else if (get_east(Player.x, Player.y) -> type == XDOOR)     map_erase(Player.x + 1, Player.y    );
            else if (get_west(Player.x, Player.y) -> type == XDOOR)     map_erase(Player.x - 1, Player.y    );
            
            Player.key = false;
            return FULL_DRAW;
        }
        speech("The door is closed");
        return FULL_DRAW;
    }
    
    return NO_RESULT;
}

int go_up()
{
    MapItem* next = get_north(Player.x, Player.y);
    MapItem* here = get_here(Player.x, Player.y);
    MapItem* bounce_position = get_south(Player.x, Player.y);
    
    if (omnipotent)
    {
        Player.y = Player.y - 1;
        if (!(next -> walkable) || !(here -> walkable))
        return FULL_DRAW;
        
        return NO_RESULT;
    }
    
    if (next -> walkable)
    {
        Player.y = Player.y - 1;
        return NO_RESULT;
    }
    
    
    if (next -> type == DANGER)
    {
        if (bounce_position -> walkable)
        Player.y = Player.y + 1;
    
        Player.healph = Player.healph - 1;
        
        if (Player.healph <= 0)
        return GAME_OVER;
        
        return HURT;
    }
    
    if (next -> type == PLANT)
    {
        add_key(Player.x, Player.y - 1);
        add_plant(Player.x, Player.y - 2);
        return FULL_DRAW;
    }
        
    
    return NO_RESULT;
}

int go_down()
{
    MapItem* next = get_south(Player.x, Player.y);
    MapItem* here = get_here(Player.x, Player.y);
    MapItem* bounce_position = get_north(Player.x, Player.y);
    
    if (omnipotent)
    {
        Player.y = Player.y + 1;
        if (!(next -> walkable) || !(here -> walkable))
        return FULL_DRAW;
        
        return NO_RESULT;
    }
    
    if (next -> walkable)
    {
        Player.y = Player.y + 1;
        return NO_RESULT;
    }
    
    if (next -> type == DANGER)
    {
        if(bounce_position -> walkable)
        Player.y = Player.y - 1;
        
        Player.healph = Player.healph - 1;
        
        if (Player.healph <= 0)
        return GAME_OVER;
        
        return HURT;
    }
    
    if (next -> type == PLANT)
    {
        add_key(Player.x, Player.y + 1);
        add_plant(Player.x, Player.y + 2);
        return FULL_DRAW;
    }
    
    return NO_RESULT;
}

int go_right()
{
    MapItem* next = get_east(Player.x, Player.y);
    MapItem* here = get_here(Player.x, Player.y);
    MapItem* bounce_position = get_west(Player.x, Player.y);
    
    if (omnipotent)
    {
        Player.x = Player.x + 1;
        if (!(next -> walkable) || !(here -> walkable))
        return FULL_DRAW;
        
        return NO_RESULT;
    }
    
    if (next -> walkable)
    {
        Player.x = Player.x + 1;
        return NO_RESULT;
    }
    
    if (next -> type == DANGER)
    {
        if(bounce_position -> walkable)
        Player.x = Player.x - 1;
        
        Player.healph = Player.healph - 1;
        
        if (Player.healph <= 0)
        return GAME_OVER;
        
        return HURT;
    }
    
    if (next -> type == PLANT)
    {
        add_key(Player.x + 1, Player.y);
        add_plant(Player.x + 2, Player.y);
        return FULL_DRAW;
    }
    return NO_RESULT;
}

int go_left()
{
    MapItem* next = get_west(Player.x, Player.y);
    MapItem* here = get_here(Player.x, Player.y);
    MapItem* bounce_position = get_east(Player.x, Player.y);
    
    if (omnipotent)
    {
        Player.x = Player.x - 1;
        if (!(next -> walkable) || !(here -> walkable))
        return FULL_DRAW;
        
        return NO_RESULT;
    }
    
    if (next -> walkable)
    {
        Player.x = Player.x - 1;
        return NO_RESULT;
    }
    
    if (next -> type == DANGER)
    {     
        if(bounce_position -> walkable)
        Player.x = Player.x + 1;
        
        Player.healph = Player.healph - 1;
        
        if (Player.healph <= 0)
        return GAME_OVER;
        
        return HURT;
    }
    
    if (next -> type == PLANT)
    {
        add_key(Player.x - 1, Player.y);
        add_plant(Player.x - 2, Player.y);
        return FULL_DRAW;
    }
    return NO_RESULT;
}

void game_over(int next_state)
{
    if (next_state == WON)
    {
        uLCD.color(RED);
        uLCD.cls();
        uLCD.text_width(3);
        uLCD.text_height(3);
        uLCD.printf("YOU\nWON");
        uLCD.text_width(1);
        uLCD.text_height(1);
        uLCD.color(GREEN);
        while (true)
        {
            // stay here
        }
    }
        
    if (next_state == GAME_OVER)
    {
        uLCD.color(RED);
        Player.life = Player.life - 1;
        uLCD.cls();
        uLCD.text_width(2);
        uLCD.text_height(2);
        uLCD.printf("Lifes remanning:\n%d\n", Player.life);
        uLCD.color(GREEN);
        f_wait(); 
        
        // Still has lifes
        if (Player.life > 0)
        {
            uLCD.cls();
            uLCD.text_width(2);
            uLCD.text_height(2);
            uLCD.printf("Press pb1\nto\nrestart\n");
            uLCD.text_width(1);
            uLCD.text_height(1);
            
            f_wait();
            uLCD.cls();
            reset_game();
            game();
        }
        
        // No life remanning
        uLCD.color(RED);
        uLCD.cls();
        uLCD.text_width(3);
        uLCD.text_height(3);
        uLCD.printf("GAME\nOVER");
        uLCD.color(GREEN);
        while (true)
        {
            //stay here forever
        }
    }
}


/**
 * Entry point for frame drawing. This should be called once per iteration of
 * the game loop. This draws all tiles on the screen, followed by the status 
 * bars. Unless init is nonzero, this function will optimize drawing by only 
 * drawing tiles that have changed from the previous frame.
 */
void draw_game(int init)
{
    
    // Draw game border first
    if(init) draw_border();
    
    // Iterate over all visible map tiles
    for (int i = -5; i <= 5; i++) // Iterate over columns of tiles
    {
        for (int j = -4; j <= 4; j++) // Iterate over one column of tiles
        {
            // Here, we have a given (i,j)
            
            // Compute the current map (x,y) of this tile
            int x = i + Player.x;
            int y = j + Player.y;
            
            // Compute the previous map (px, py) of this tile
            int px = i + Player.px;
            int py = j + Player.py;
                        
            // Compute u,v coordinates for drawing
            int u = (i+5)*11 + 3;
            int v = (j+4)*11 + 15;
            
            // Figure out what to draw
            DrawFunc draw = NULL;
            if (init && i == 0 && j == 0) // Only draw the player on init
            {
                draw_player(u, v, Player.has_key);
                continue;
            }
            else if (x >= 0 && y >= 0 && x < map_width() && y < map_height()) // Current (i,j) in the map
            {
                MapItem* curr_item = get_here(x, y);
                MapItem* prev_item = get_here(px, py);
                if (init || curr_item != prev_item) // Only draw if they're different
                {
                    if (curr_item) // There's something here! Draw it
                    {
                        draw = curr_item->draw;
                    }
                    else // There used to be something, but now there isn't
                    {
                        draw = draw_nothing;
                    }
                }
            }
            else if (init) // If doing a full draw, but we're out of bounds, draw the walls.
            {
                draw = draw_wall;
            }

            // Actually draw the tile
            if (draw) draw(u, v);
        }
    }

    // Draw status bars    
    draw_upper_status(Player.x, Player.y, Player.px, Player.py);
    draw_lower_status(Player.healph, Player.p_healph, Player.life);
    //add_danger(0, 0);
    
    if (init == HURT)
    wait(1.0);
}


/**
 * Initialize the main world map. Add walls around the edges, interior chambers,
 * and plants in the background so you can see motion.
 */
void init_main_map()
{
    // "Random" plants
    Map* map = set_active_map(0);
    for(int i = map_width() + 3; i < map_area(); i += 39)
    {
        add_danger(i % map_width(), i / map_width());
    }
    pc.printf("plants\r\n");
    
    add_old_man(7, 9);
    add_leef(40, 20);
    add_batman(15, 30);
    add_plant(10, 15);
    
    add_mushroom(10,10);
    add_mushroom(23,28);
    add_mushroom(30,17);
    add_mushroom(6,60);
    add_mushroom(5,34);
    
    
    //add_danger(Player.x, Player.y);
            
    pc.printf("Adding walls!\r\n");
    add_wall(0,              0,              HORIZONTAL, map_width());
    add_wall(0,              map_height()-1, HORIZONTAL, map_width());
    add_wall(0,              0,              VERTICAL,   map_height());
    add_wall(map_width()-1,  0,              VERTICAL,   map_height());
    
    add_wall(map_width()/2,  0,              VERTICAL,    map_height());
    
    add_door(map_width()/2, 20);
    
    pc.printf("Walls done!\r\n");

    print_map();
}

void reset_game()
{
    omnipotent = false;
    Player.key = false;
    Player.leaf = false;
    Player.healph = 5;
    Player.mushroom = 0;
    
    uLCD.printf("Loadig Game...\n");
    // Initialize the maps
    init_main_map();
    
    // Initialize game state
    set_active_map(0);
    Player.x = Player.y = 5;
    
    // Initial drawing
    draw_game(FULL_DRAW);
}

void game()
{
    GameInputs inputs;
    int action;
    int next_state;
    
    // Main game loop
    while(1)
    {
        // Timer to measure game update speed
        Timer t; t.start();
        
        // Actuall do the game update:
        // 1. Read inputs
        inputs = read_inputs();
        
        // 2. Determine action (get_action)
        action = get_action(inputs);
          
        // 3. Update game (update_game)
         next_state = update_game(action);
        
        // 3b. Check for game over
        game_over(next_state);
        
        // 4. Draw frame (draw_game)
        draw_game(next_state);
        
        // 5. Frame delay
        t.stop();
        int dt = t.read_ms();
        if (dt < 100) wait_ms(100 - dt);
    }
}

void f_wait()
{   
    int pb1 = 1;   
    while (pb1 == 1)
    {
        for (int i = 0; i < 4; i++)
        {
            uLCD.filled_circle(120, 121, 4, RED); wait(0.1);
            pb1 = readPB1(pb1);
        }
        
        if (pb1 == 0) break;
        
        for (int i = 0; i < 4; i++)
        {
            uLCD.filled_circle(120, 121, 4, BLACK); wait(0.1);
            pb1 = readPB1(pb1);
        }
    }
}

/**
 * Program entry point! This is where it all begins.
 * This function orchestrates all the parts of the game. Most of your
 * implementation should be elsewhere - this holds the game loop, and should
 * read like a road map for the rest of the code.
 */

int main()
{   
    // Clear left over serial data
    fflush(pc);
    
    // First things first: initialize hardware
    ASSERT_P(hardware_init() == ERROR_NONE, "Hardware init failed!");
    
    uLCD.text_width(4); //4X size text
    uLCD.text_height(4);
    uLCD.color(RED);
    for (int i=5; i>=0; --i) {
        uLCD.locate(1,2);
        uLCD.printf("%2D",i);
        wait(.5);
    }
    
    uLCD.cls();
    uLCD.text_width(2);
    uLCD.text_height(2);
    uLCD.printf("The\nBinding\nof\nIzaac");
    f_wait();
    uLCD.cls();
    
    
    uLCD.color(GREEN);
    uLCD.text_width(1);
    uLCD.text_height(1);    
    
    
    // Create Hash Table
    maps_init();
    
    Player.life = 3;
    Player.p_life = Player.life;
    
    //wait(5.0);
    
    reset_game();
    game();
}
    