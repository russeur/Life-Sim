//compile with the command "g++ -I src/include -L src/lib -o main main.cpp world.cpp drawer.cpp high_res_clock.cpp -lmingw32 -lSDL2main -lSDL2 -lSDL2_ttf"

#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include "drawer.hpp"
#include <iostream>
#include "high_res_clock.hpp"

const int width = 500;
const int height = 500;

int dim_x=250;
int dim_y=250;
int plant_food_rate = 60;

long long target_fps = 600000000;

SDL_Cell_Drawer drawer(dim_x, dim_y, "Life", width/dim_x, width, height, SDL_Color{0, 255, 0, 255});
World world(dim_x, dim_y);

int main(int argc, char *argv[]){
    
    



    TTF_Init();
    srand(time(0));

    TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);
    if (Sans == nullptr){
        std::cout<<"error opening font: "<<TTF_GetError()<<"\n";
    }

    bool is_quit = false;
    bool update_render = true;
    int step_count = 0;
    SDL_Event e;
    Position start_view{0, 0}, end_view{0, 0};

    
    
    
    Gene first_creature[] = {
        Gene{Position{0, 0}, Cell_Type::STOMACH},
        Gene{Position{1, 0}, Cell_Type::PLANT},
        Gene{Position{1, 1}, Cell_Type::PLANT},
        Gene{Position{0, -1}, Cell_Type::STOMACH}
    };
    
    DNA first_dna(&first_creature[0], 4, false);
    std::cout<<"init gene done\n";
    world.place_new_creature(first_dna, Position{125, 125}, false);
    
    std::cout<<"end\n";

    time_point start_time = now();
    time_point last_time = start_time;
    float current_fps = 0.0;

     //clear screen
    
    drawer.clear_render();
        
    //render stuff
    //drawer.update_grid(world.grid);

    //show render on screen
    drawer.show_render();

    while(is_quit == false){

        //make sure fps is capped

        current_fps = ((float)ticks_per_sec/(get_delta(last_time, now())));
        
        if (current_fps > target_fps) continue;
        last_time = now();
        //std::cout<<"fps: "<<current_fps<<"\n";

        

        step_count++;

       

        //update world
        world.step();
        
        if (update_render){
            drawer.clear_render();

            //render stuff
            drawer.update_grid(world.grid);

            //show render on screen
            drawer.show_render();
        }
         //clear screen
        

        //event handler
        while (drawer.poll_events(&e)){
            switch (e.type){
            case SDL_QUIT:
                is_quit = true;
                break;
            case SDL_KEYDOWN:
                if (e.key.keysym.sym == SDLK_0){
                    if ( update_render){
                        update_render = false;
                    }else{
                        update_render = true;
                    }
                
                }
                if (e.key.keysym.sym == SDLK_1){
                    drawer.set_view(Position{0, 0}, Position{width, height});
                }
                break;
            }
            

        }

        
        

    }
    std::cout<<"average fps: "<<((float)ticks_per_sec/((get_delta(start_time, now()))*(float)step_count));

    return EXIT_SUCCESS;

}
