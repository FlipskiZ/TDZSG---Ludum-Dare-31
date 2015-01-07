#include "Engine.h"
#include "MenuState.h"
#include "GameState.h"
#include "PlayState.h"

Button::Button(){
    this->clicked = false;
}

void Button::draw(){
    if(this->typeId < 3){
        al_draw_rectangle(this->posX+5, this->posY+5, this->posX+this->width-5, this->posY+this->height-5, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), 10);
    }else if(this->typeId < 40){
        al_draw_rectangle(this->posX+5, this->posY+5, this->posX+this->width-5, this->posY+this->height-5, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), 5);
    }else{
        al_draw_rectangle(this->posX+5, this->posY+5, this->posX+this->width-5, this->posY+this->height-5, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), 2.5);
    }

    switch(this->typeId){
        case 0:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Resume"); break;
        case 1:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Play"); break;
        case 2:
            al_draw_text(defaultFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "Quit"); break;

        case 3:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "GUN1"); break;
        case 4:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "GUN2"); break;
        case 5:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "GUN3"); break;
        case 6:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "GUN4"); break;
        case 7:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MG1"); break;
        case 8:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "SHTGUN1"); break;
        case 9:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "GUN5"); break;
        case 10:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MG2"); break;
        case 11:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "SHTGUN2"); break;
        case 12:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MG3"); break;
        case 13:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MGSHTGUN"); break;
        case 14:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "RCKTLCHR"); break;
        case 15:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MINIGUN"); break;
        case 16:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MGRCKT"); break;
        case 17:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "QUAKOR"); break;

        case 18:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "HP*1.5"); break;
        case 19:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MLE*2"); break;
        case 20:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "STM*1.5"); break;
        case 21:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "HP*2"); break;
        case 22:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MLE*2"); break;
        case 23:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MVSPD*1.5"); break;
        case 24:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "HP*2"); break;
        case 25:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "MLE*1.5"); break;
        case 26:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "ST&MV*1.5"); break;
        case 27:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "ALL*1.5"); break;
        case 28:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "HP&STM*2"); break;
        case 29:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "ST&MV*2"); break;
        case 30:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "ALL*2"); break;
        case 31:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "HP&STM*3"); break;
        case 32:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "ST&MV*4"); break;
        case 40:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "AMMO+10"); break;
        case 41:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "HPPACK"); break;
        case 42:
            al_draw_text(smallFont, (this->selected) ? al_map_rgb(155,155,155) : al_map_rgb(215,215,215), this->posX+this->width/2, this->posY+this->height/3, ALLEGRO_ALIGN_CENTRE, "DYNAMITE"); break;

    }
}

void Button::update(){
    this->clicked = false;
    if(checkCollision(this->posX, this->posY, mouseX, mouseY, this->width, this->height, 0, 0)){
        if(mouseButtonLeftClick){
            switch(this->typeId){
                case 0:
                    break; //Reserved for resume

                case 1:
                    break; //Reserved for screenState change

                case 2:
                    break; //Reserved for quit

            }
            this->clicked = true;
        }
        this->selected = true;
    }else{
        this->selected = false;
    }
}
