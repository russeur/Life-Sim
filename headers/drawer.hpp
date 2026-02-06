#include <SDL2/SDL.h>
#include "world.hpp"
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <variant>





struct Color{
    Uint8 R, G, B, A;
    Color(){}
    Color(Uint8 _R, Uint8 _G, Uint8 _B, Uint8 _A){
        R = _R;
        G = _G;
        B = _B;
        A = _A;
    }
    Color(const Color &color){
        this->R = color.R;
        this->G = color.G;
        this->B = color.B;
        this->A = color.A;
    }
};





class SDL_drawer{
    SDL_Color bg;
    
    SDL_Window *win;
    
    SDL_Event windowEvent;


    protected:
    


    int win_width, win_height;
    SDL_Renderer *renderer;

    




    public:

    SDL_Renderer *get_renderer();

    void show_render();//update screen

    

    void clear_render();//clear screen with bg color

    int poll_events(SDL_Event *event);
    
    

    void draw_rect(Sint32 x, Sint32 y, Uint32 w, Uint32 h, SDL_Color color);


    void SDL_ready(const char *title, int win_width, int win_height, SDL_Color background); // init for this
    void SDL_deconstruct();


    SDL_drawer(const char *title, int win_width, int win_height, SDL_Color background);

    ~SDL_drawer();
    
};

class SDL_text_box{
    protected:
    SDL_Renderer* renderer = nullptr;
    SDL_Rect txt_bounds;
    SDL_Texture* texture = nullptr;
    TTF_Font* font = nullptr;
    SDL_Color text_color;
    char* text = nullptr;
    

    
    

    


    public:
    
    void update_texture();
    void set_txt(const char* txt);
    void set_color(SDL_Color color);
    void set_pos(int x, int y);
    void set_font(TTF_Font* font);
    void set_renderer(SDL_Renderer* renderer);
    SDL_Texture* get_texture();
    void render_txt();
    SDL_text_box();
    SDL_text_box(SDL_Renderer* renderer, TTF_Font* font);
    ~SDL_text_box();
};


class SDL_grid_drawer: public SDL_drawer{
    int square_size;
    
    

    



    public:


    void update_grid(SDL_Color *color_array);
    

    



    SDL_grid_drawer(const char *title, int cell_size, int win_width, int win_height, SDL_Color background);
    ~SDL_grid_drawer();
};

class SDL_Cell_Drawer: public SDL_drawer{
    int square_size, dim_x, dim_y;
    Position start_view, end_view;
    static constexpr SDL_Color colors[] = {
        0, 0, 0, 255,
        0, 0, 200, 255,
        100, 100, 100, 255,
        255, 0, 0, 255,
        255, 255, 0, 255,
        0, 255, 0, 255,
        100, 100, 255, 255,
        150, 150, 0, 255,
        255, 150, 0

    };
    
    SDL_Rect* rects = nullptr;
    int *offsets = nullptr;
    int *progress = nullptr;

    public:

    void _update_grid(Cell* grid);
    void update_grid(Cell* grid);
    void update_pixels(Cell* grid);
    void update_with_surface(Cell* grid);
    
    void set_view(Position start, Position end);//sets the view box



    SDL_Cell_Drawer(int dim_x, int dim_y, const char *title, int cell_size, int win_width, int win_height, SDL_Color background);
    ~SDL_Cell_Drawer();
};







template <typename r_type, typename ...args>
class Callable{
    public:
    typedef r_type (*func_class)(args...);
    func_class function = nullptr;
    r_type operator()(args ...arguments){
        return function(arguments...);
    }
    Callable(func_class func_pntr){
        this->function = func_pntr;
    }
    Callable(){function = nullptr;}
};

template <typename class_>
class Button : public SDL_Rect{
    
    protected:
    
    
    SDL_Renderer* renderer = nullptr;

    Callable<void, class_*> func;
    class_* caller;
    
    
    public:
    
    
    SDL_Rect box;
    
    
    SDL_Color color;
    



    void render_box(){
        SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
        SDL_RenderFillRect(renderer, &box);
    }
    void run(){
        func(caller);
    }
    void set_connection(Callable<void, class_*> func, class_* caller){
        this->func = func;
        this->caller = caller;
    }

    void mouse_press(SDL_MouseButtonEvent &e){

        if ((e.button == SDL_BUTTON_LEFT)&&(e.x > box.x)&&(e.y > box.y)&&(e.x < box.x+box.w)&&(e.y < box.y+box.h)){
            std::cout<<"pressed \n";
            run();
        }
        
    }
    Button(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color, Callable<void, class_*> func, class_* caller){
        set_connection(func, caller);
        
        this->color.r = color.r;
        this->color.g = color.g;
        this->color.b = color.b;
        this->color.a = color.a;

        this->box.x = rect.x;
        this->box.y = rect.y;
        this->box.w = rect.w;
        this->box.h = rect.h;

        this->renderer = renderer;
    }

    Button(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color){
        this->color.r = color.r;
        this->color.g = color.g;
        this->color.b = color.b;
        this->color.a = color.a;

        this->box.x = rect.x;
        this->box.y = rect.y;
        this->box.w = rect.w;
        this->box.h = rect.h;

        this->renderer = renderer;
    }
    Button(SDL_Renderer* renderer, SDL_Rect rect){
        this->box.x = rect.x;
        this->box.y = rect.y;
        this->box.w = rect.w;
        this->box.h = rect.h;
        this->renderer = renderer;
    }
    Button(SDL_Renderer* renderer){
        this->box.x = 0;
        this->box.y = 0;
        this->box.w = 0;
        this->box.h = 0;
        this->renderer = renderer;
    }
};



class textInput: protected SDL_text_box{

    protected:
    bool active = false;
    int max_length = 0;
    
    int length=0;

    
    
    
    

    Callable<void, char*> function;
    
    
    SDL_Rect box;
    
    
    SDL_Color color;

    

    public:

    
    

    
    void set_bg_color(SDL_Color bg);
    void set_box_bounds(int x, int y, int w, int h);
    Sint32 get_box_x();
    Sint32 get_box_y();
    Uint32 get_box_w();
    Uint32 get_box_h();
    void set_text_color(SDL_Color color);
    void add_char(char letter);
    void pop();//removes last letter
    bool is_active();
    void mouse_press(SDL_MouseButtonEvent &e);//call this to update activity based on mouse click
    void update_txt();
    void render();
    void run_func();
    void set_func(Callable<void, char*>);
    void set_font(TTF_Font* font);
    void set_renderer(SDL_Renderer* renderer);
    void new_text_string(int max_length);
    void update_text_texture();

    textInput(SDL_Renderer* renderer, Callable<void, char*> func, TTF_Font* font, int max_length, SDL_Rect bounds = SDL_Rect{0, 0, 10, 10}, SDL_Color bg_color = SDL_Color{255, 0, 0, 255}, SDL_Color text_color = SDL_Color{0, 255, 0, 255});
    textInput();
    ~textInput();
};


class textInputContainer{
    protected:
    
    
    int length = 0;

    public:
    textInput* list = nullptr;

    void key_press(SDL_Event &e);
    void mouse_press(SDL_MouseButtonEvent &e);
    void set_item(int index, textInput item);
    void set_item(int index, SDL_Renderer* renderer, Callable<void, char*> func, TTF_Font* font, int max_length, SDL_Rect bounds = SDL_Rect{0, 0, 10, 10}, SDL_Color bg_color = SDL_Color{255, 0, 0, 255}, SDL_Color text_color = SDL_Color{0, 255, 0, 255});
    void new_list(int length, SDL_Renderer* default_renderer, TTF_Font* default_font);
    void render_all();
    textInputContainer(int length, SDL_Renderer* default_renderer, TTF_Font* default_font);
    
    ~textInputContainer();
};






template <typename ...classes_>
class buttonContainer{
    public:
    
    

    using arbButton = std::variant<Button<classes_>...>;
    
    std::vector<arbButton> bttnVector;

    public:
    
    template <typename class_>
    void add_new(SDL_Renderer* renderer, SDL_Rect rect, SDL_Color color, Callable<void, class_*> func, class_* caller){
        bttnVector.push_back(std::move(Button<class_>(renderer, rect, color, func, caller)));
    }
    void remove(int index){
        bttnVector.erase(bttnVector.begin()+index);
    }

    void get_button(int index){
        *(bttnVector.begin()+index);
    }

    void render(){
        for (auto& i : bttnVector){
            std::visit([](auto& b){b.render_box();}, i);
        }
    }
    void mouse_press(SDL_MouseButtonEvent &e){
        for (auto& i : bttnVector){
            std::visit([&e](auto& b){b.mouse_press(e);}, i);
        }
    }




};



