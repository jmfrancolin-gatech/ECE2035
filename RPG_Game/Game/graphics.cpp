#include "graphics.h"
#include "mbed.h"
#include "globals.h"


void draw_player(int u, int v, int key)
{
    char player[122] = "KKKKKKKKKKKKKKSSSSSKKKKKSSSSSSSKKKSSSSSSSSSKKSWKSSSWKSKKSKKSSSKKSKKSBBKKKBBSKKKBSKKKSBKKKKKSSSSSKKKKKKKKKKKKKKKKKKKKKKKKK";
    draw_img(u, v, player);
}

void draw_key(int u, int v)
{
    char key[122] = "KKKKKKKKKKKKKKKYKYKKKKKKKYKKKYKKKKKKOYKYOKKKKKKKOYOKKKKKKKKYKKKKKKKKKKYKKKKKKKKKOYKKKKKKKKKKYKKKKKKKKOYOKKKKKKKKKKKKKKKK";
    draw_img(u, v, key);
}

void draw_old_man(int u, int v)
{
    char old_man[122] = "KKAAAAAAKKKKKAWAAWAKKKKAAKAAKAAKKKAAKAAKAAKKKKWAAAAWwKKKKWWWWWWKKKKKWKKKKWKKKKKWWWWWWKKKKKKWWWWKKKKKKKWWWWKKKKKKKKWWKKKKK";
    draw_img(u, v, old_man);
}

void draw_leef(int u, int v)
{
    char leef[122] = "KKKKTTKKKKKKKKKTTKKKKKTKKTTTTKKTKTTKTTTTKTTKTTTWTTWTTTKTTTKTTKTTTKTTTKTTKTTTKKTTTTTTTTKKKKTTTTTTKKKKKKKTTKKKKKKKKKTTKKKKK";
    draw_img(u, v, leef);
}

void draw_danger(int u, int v)
{
    char danger[122] = "RRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRRR";
    draw_img(u, v, danger);
}

void draw_door(int u, int v)
{
    char door[122] = "BBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBBB";
    draw_img(u, v, door);
}

void draw_batman(int u, int v)
{
    char batman[122] = "WWKWWWWWKWWWWKKWWWKKWWWWKKKKKKKWWWWKKWKWKKWWWWKKKKKKKWWWWKAAAAAKWWWLLKKKKKLLWWLLLLLLLLLWWWKKKGKKKWWWWKLLLLLKWWWWKLLKLLKWW";
    draw_img(u, v, batman);
}

void draw_mushroom(int u, int v)
{
    char mushroom[122] = "KKKKKKKKKKKKKWWRRRWWKKKRWWRRRWRRKKRRRWWWRRRKKRRRWWWRRRKKKKKKKKKKKKKKKWWWWWKKKKKKWWWWWKKKKKKWWWWWKKKKKKWWWWWKKKKKKKKKKKKKK";
    draw_img(u, v, mushroom);
}



#define YELLOW 0xFFFF00
#define BROWN  0xD2691E
#define DIRT   BROWN

#define GOLD   0xffc125
#define Burlywood2 0xeec591
#define DarkSalmon 0xe9967a
#define Chartreuse4 0x458b00

void draw_img(int u, int v, const char* img)
{
    int colors[11*11];
    for (int i = 0; i < 11*11; i++)
    {
        if      (img[i] == 'R') colors[i] = RED;
        else if (img[i] == 'G') colors[i] = GREEN;
        else if (img[i] == 'B') colors[i] = BLUE;
        
        else if (img[i] == 'K') colors[i] = BLACK;
        else if (img[i] == 'W') colors[i] = WHITE;
        
        else if (img[i] == 'Y') colors[i] = YELLOW;
        else if (img[i] == 'O') colors[i] = GOLD;
        
        else if (img[i] == 'A') colors[i] = Burlywood2;
        else if (img[i] == 'S') colors[i] = DarkSalmon;
        else if (img[i] == 'T') colors[i] = Chartreuse4;
        
        else if (img[i] == '1') colors[i] = DIRT;
        else if (img[i] == 'L') colors[i] = LGREY;
        else if (img[i] == '3') colors[i] = DGREY;
        
        else colors[i] = BLACK;
    }
    uLCD.BLIT(u, v, 11, 11, colors);
    wait_us(250); // Recovery time!
}

void draw_nothing(int u, int v)
{
    // Fill a tile with blackness
    uLCD.filled_rectangle(u, v, u+10, v+10, BLACK);
}

void draw_wall(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, BROWN);
}

void draw_plant(int u, int v)
{
    uLCD.filled_rectangle(u, v, u+10, v+10, GREEN);
}

void draw_upper_status(int x, int y, int px, int py)
{
    if (x != px || y != py)
    {
        uLCD.filled_rectangle(0, 0, 127, 7, PURPLE);
        
        uLCD.textbackground_color(PURPLE);
        uLCD.locate(1, 0);
        uLCD.printf("x = %d,\ty = %d", x, y);
    }
}

void draw_lower_status(int healph, int p_healph, int life)
{
    if (healph != p_healph)
    {
    uLCD.filled_rectangle(0, 119, 127, 127, PURPLE);
    
    uLCD.textbackground_color(PURPLE);
    uLCD.locate(1, 15);
    uLCD.printf("healph: %d life: %d", healph, life);
    }
}

void draw_border()
{
    uLCD.filled_rectangle(0,     9, 127,  14, WHITE); // Top
    uLCD.filled_rectangle(0,    13,   2, 114, WHITE); // Left
    uLCD.filled_rectangle(0,   114, 127, 117, WHITE); // Bottom
    uLCD.filled_rectangle(124,  14, 127, 117, WHITE); // Right
}


