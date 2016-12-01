#include <allegro.h>
#include <stdlib.h>
#include <stdio.h>

#define WScreen 800
#define HScreen 600

#define CorPreto makecol(0, 0, 0)
#define CorBranco makecol(255, 255, 255)
#define CorAzul makecol(145, 145, 255)
#define CorVermelho makecol(255, 0, 0)

struct sprite
{
    int x, y;
    int height, width;
    float col_height, col_width;
    float col_x_offset, col_y_offset;
    BITMAP *bmp;
};

volatile int a = 0;
int TempoExplo=0;
int Pontos=0;
int Vidas=3;
int Nivel=1;
int s_explo_teste=0;

//BUFFER (IMAGEM TEMPORARIA)
BITMAP *buffer;
PALETTE pal;

//SONS
SAMPLE *s_tiro;
SAMPLE *s_explo;

//SPRITES
struct sprite nave[3];
struct sprite tiro;
struct sprite explo;
struct sprite navei;

//TIMER
void func() { a++; }

//DIVERSOS


int InitGame();
void MediaLoad();
void Teclado();
void Atirar();
int Colisao(struct sprite img1, struct sprite img2);
void NaveI();

int main() 
{ 
    int EscDir;
    InitGame();

    buffer = create_bitmap(WScreen, HScreen);
    clear_bitmap(buffer);
        
    while(1)
    {
        poll_keyboard();
        Teclado();
        if((Colisao(tiro, navei) == 1) || (TempoExplo != 0))
        {
            if (s_explo_teste == 0) 
               { play_sample(s_explo,255,90,1000,0); s_explo_teste = 1; }
               
            explo.x = navei.x - 28;
            explo.y = navei.y - 23;
            draw_sprite(buffer, explo.bmp, explo.x, explo.y);
            tiro.y = -1 * tiro.height;
            TempoExplo++;
            if(TempoExplo == 40)
            {
                Pontos++;
            Novo:
                TempoExplo = 0;
                s_explo_teste = 0;
                
                srand(time(NULL));                
                navei.x = 1 + (rand() % WScreen);                
                srand(time(NULL));
                while((navei.x < 0) || (navei.x > WScreen - navei.width))
                    navei.x = 1 + (rand() % WScreen);
                    
                navei.y = 15;
            }
        }
        else
        {
            NaveI(); 
            srand(time(NULL)); 
            if((1 + (rand() % 4)) == 3)    
                navei.y += 1; 
            
            if(navei.y > HScreen)
            {
                Vidas --;
                if(Vidas == 0)
                         goto GameOver;
                goto Novo;
            }
                       
            draw_sprite(buffer, navei.bmp, navei.x, navei.y);
        }

        if(tiro.y > -1 * tiro.height)
        {
            draw_sprite(buffer, tiro.bmp, tiro.x, tiro.y);
            tiro.y -= 3;
        }
        
        textprintf_ex(buffer, font, 3, 3, CorBranco, CorPreto, "Nivel: %d", Nivel);
        textprintf_ex(buffer, font, 3, 15, CorBranco, CorPreto, "Vidas: %d", Vidas);
        textprintf_ex(buffer, font, 3, 27, CorBranco, CorPreto, "Pontos: %d", Pontos);
        
        textprintf_ex(buffer, font, 0, HScreen - 8, CorBranco, CorPreto, "FAI - Ciencias da Computacao - RA: 0345/04");
        
        blit(buffer, screen, 0, 0, 0, 0, WScreen, HScreen);
        clear_bitmap(buffer);
    }
GameOver:
    while(!key[KEY_ESC])
    {
        poll_keyboard();
        textprintf_ex(buffer, font, 0, 0, CorBranco, CorPreto, "ESC p/ sair");     
        textprintf_ex(buffer, font, WScreen / 2 - 40, HScreen/2, CorVermelho, CorPreto, "Game Over");
        blit(buffer, screen, 0, 0, 0, 0, WScreen, HScreen);
        clear_bitmap(buffer);
    }
    
    allegro_exit();
    return 0;
}END_OF_MAIN();

int InitGame()
{
    allegro_init();
    set_window_title("Trabalho de POO - FAI - Ciencias da Computacao - RA: 0345/04");
    install_keyboard();
    install_timer();
    install_mouse();
    install_sound(DIGI_AUTODETECT,MIDI_AUTODETECT,NULL);
    install_int(func,100);

    set_color_depth(32);
    set_gfx_mode(GFX_AUTODETECT_WINDOWED, WScreen, HScreen, 0, 0); 
    MediaLoad();
}

void MediaLoad()
{
    nave[0].bmp = load_bitmap("data/n1.bmp",pal);
    nave[0].width = nave[0].bmp->w;
    nave[0].height = nave[0].bmp->h;
    nave[0].x = WScreen / 2 - nave[0].width / 2;    
    nave[0].y = HScreen - nave[0].height - 5;   
    nave[0].col_width = nave[0].width * 0.80;
    nave[0].col_height = nave[0].height * 0.80;
    
    nave[1].bmp = load_bitmap("data/n2.bmp",pal);
    nave[1].width = nave[1].bmp->w;
    nave[1].height = nave[1].bmp->h;
    nave[1].x = WScreen / 2 - nave[1].width / 2;    
    nave[1].y = HScreen - nave[1].height - 5;   
    nave[1].col_width = nave[1].width * 0.80;
    nave[1].col_height = nave[1].height * 0.80;
    
    nave[2].bmp = load_bitmap("data/n3.bmp",pal);
    nave[2].width = nave[2].bmp->w;
    nave[2].height = nave[2].bmp->h;
    nave[2].x = WScreen / 2 - nave[2].width / 2;    
    nave[2].y = HScreen - nave[2].height - 5;   
    nave[2].col_width = nave[2].width * 0.80;
    nave[2].col_height = nave[2].height * 0.80;
    
    navei.bmp = load_bitmap("data/ni.bmp",pal);
    navei.width = navei.bmp->w;
    navei.height = navei.bmp->h;
    navei.x = WScreen / 2 - navei.width / 2;    
    navei.y = 15;
    navei.col_width = navei.width * 0.80;
    navei.col_height = navei.height * 0.80;
    
    explo.bmp = load_bitmap("data/exp1.bmp",pal);
    explo.width = explo.bmp->w;
    explo.height = explo.bmp->h;
    explo.x = WScreen / 2 - explo.width / 2;    
    explo.y = 15;
    
    tiro.bmp = load_bitmap("data/tiro.bmp",pal);
    tiro.width = tiro.bmp->w;
    tiro.height = tiro.bmp->h;
    tiro.x = 0;    
    tiro.y = 0;   
    tiro.col_width = tiro.width * 0.80;
    tiro.col_height = tiro.height * 0.80;
    
    s_tiro = load_wav("data/laser.wav");
    s_explo = load_wav("data/exp.wav");
}
                       
void Teclado()
{
    if(key[KEY_LEFT])
    {
        if((key[KEY_SPACE]) && (tiro.y <= 0))
        {
            Atirar();
        }
        if(nave[0].x > 0)
            nave[0].x -= 2;
        nave[1].x = nave[0].x;
        draw_sprite(buffer, nave[1].bmp, nave[1].x, nave[1].y);
    }
    else if(key[KEY_RIGHT])
         {
             if((key[KEY_SPACE]) && (tiro.y <= -1 * tiro.height))
             {
                 Atirar();
             }
             if(nave[0].x < WScreen - nave[0].width)
                 nave[0].x += 2;
             nave[2].x = nave[0].x;
             draw_sprite(buffer, nave[2].bmp, nave[2].x, nave[2].y);
         }
         else if(key[KEY_ESC])
                  exit(0);
              else if((key[KEY_SPACE]) && (tiro.y <= -1 * tiro.height))
                   {
                       Atirar();
                   }
                   else
                       draw_sprite(buffer, nave[0].bmp, nave[0].x, nave[0].y);    
}

void Atirar()
{
     play_sample(s_tiro,255,90,1000,0);
     tiro.x = nave[0].x;
     tiro.y = HScreen - nave[2].height - 20;
     draw_sprite(buffer, nave[0].bmp, nave[0].x, nave[0].y);
}

int Colisao(struct sprite object1, struct sprite object2)
{
    float left1, left2;
    float right1, right2;
    float top1, top2;
    float bottom1, bottom2;
 
    left1 = object1.x + object1.col_x_offset;
    left2 = object2.x + object2.col_x_offset;
    right1 = left1 + object1.col_width;
    right2 = left2 + object2.col_width;
    top1 = object1.y + object1.col_y_offset;
    top2 = object2.y + object1.col_y_offset;
    bottom1 = top1 + object1.col_height;
    bottom2 = top2 + object2.col_height;
 
    if (bottom1 < top2) return(0);
    if (top1 > bottom2) return(0);
 
    if (right1 < left2) return(0);
    if (left1 > right2) return(0);
 
    return(1);
}

void NaveI()
{
    switch(Pontos)
    {
        case 10:
             Nivel = 2;
             break;
        case 20:
             Nivel = 3;
             break;
        case 30:
             Nivel = 4;
             break;
        case 40:
             Nivel = 5;
             break;
        case 50:
             Nivel = 6;
             break;
    }
    srand(time(NULL));
    do{
        if((1 + (rand() % 2)) == 1)
            navei.x -= Nivel;
        else
            navei.x += Nivel;
    }while((navei.x < 0) || (navei.x > WScreen - navei.width));     
}
