
#include "drawer.hpp"
#include "iostream"



SDL_text_box::SDL_text_box(){

}
SDL_text_box::SDL_text_box(SDL_Renderer* renderer, TTF_Font* font){
    
    this->font = font;
    this->renderer = renderer;
    txt_bounds.x = 0;
    txt_bounds.y = 0;
    txt_bounds.w = 0;
    txt_bounds.h = 0;

    text_color.r = 0;
    text_color.g = 0;
    text_color.b = 0;
    text_color.a = 0;
}
SDL_text_box::~SDL_text_box(){
    delete text;
    SDL_DestroyTexture(texture);
}
void SDL_text_box::update_texture(){
    
    SDL_Surface* txtSurf = TTF_RenderText_Solid(font, text, text_color);
    if (txtSurf == NULL){
        std::cout<<"error renderer text solid: "<<TTF_GetError()<<"\n";
    }
    
    
    txt_bounds.w = txtSurf->w;
    txt_bounds.h = txtSurf->h;
    texture = SDL_CreateTextureFromSurface(renderer, txtSurf);
    /*if (texture == nullptr){
        
    }else{
        SDL_UpdateTexture(texture, &txt_bounds, txtSurf->pixels, txtSurf->pitch);
    }*/
    
    SDL_FreeSurface(txtSurf);
    std::cout<<"text: "<<text<<"\n";
    std::cout<<"box: "<<txt_bounds.x<<", "<<txt_bounds.y<<", "<<txt_bounds.w<<", "<<txt_bounds.h<<"\n";
    std::cout<<"errors: "<<SDL_GetError()<<"\n";
    
}
void SDL_text_box::set_txt(const char* txt){
    delete this->text;
    (this->text) = new char[1]{*txt};
    std::cout<<"set txt: "<<(this->text)<<"\n";
    
}
void SDL_text_box::set_color(SDL_Color color){
    this->text_color = color;
}
void SDL_text_box::set_pos(int x, int y){
    this->txt_bounds.x = x;
    this->txt_bounds.y = y;
}
SDL_Texture* SDL_text_box::get_texture(){
    return texture;
}
void SDL_text_box::render_txt(){
    
    if (SDL_RenderCopy(renderer, texture, NULL, &txt_bounds)!= 0){
        std::cout<<"cannot render texture: "<<SDL_GetError()<<"\n";
    }
    
}
void SDL_text_box::set_font(TTF_Font* font){
    this->font = font;
}
void SDL_text_box::set_renderer(SDL_Renderer* renderer){
    this->renderer = renderer;
}



SDL_drawer::SDL_drawer(const char *title, int win_width, int win_height, SDL_Color background){
    SDL_ready(title, win_width, win_height, background);
    this->win_width = win_width;
    this->win_height = win_height;
}
SDL_drawer::~SDL_drawer(){
    
    SDL_deconstruct();
}



void SDL_drawer::draw_rect(Sint32 x, Sint32 y, Uint32 w, Uint32 h, SDL_Color color){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_Rect r;
    r.x = x;
    r.y = y;
    r.w = w;
    r.h = h;

    SDL_RenderFillRect(renderer, &r);
}

int SDL_drawer::poll_events(SDL_Event *event){
    return SDL_PollEvent(event);
}



void SDL_drawer::SDL_ready(const char *title, int win_width, int win_height, SDL_Color background){
    
    SDL_Init(SDL_INIT_EVERYTHING);
    this->win = SDL_CreateWindow(title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, win_width, win_height, SDL_WINDOW_ALLOW_HIGHDPI);
    this->renderer = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED);
    this->bg = background;
    clear_render();
    
}

void SDL_drawer::SDL_deconstruct(){
    
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(win);
    SDL_Quit();
    
}


void SDL_drawer::show_render(){
    SDL_RenderPresent(renderer);
}



void SDL_drawer::clear_render(){
    SDL_SetRenderDrawColor(renderer, bg.r, bg.g, bg.b, bg.a);
    SDL_RenderClear(renderer);
}

SDL_Renderer *SDL_drawer::get_renderer(){
    return renderer;
}




SDL_grid_drawer::SDL_grid_drawer(const char *title="SDL_drawer", int cell_size = 1, int win_width = 500, int win_height = 500, SDL_Color background = SDL_Color{0, 0, 0, 255}):SDL_drawer(title, win_width, win_height, background){
    this->square_size = cell_size;
    
}
SDL_grid_drawer::~SDL_grid_drawer(){
    
}



void SDL_grid_drawer::update_grid(SDL_Color *color_array){
    
    
    SDL_Color *color_pntr = color_array;
    
    for (int y = 0; y< (win_height/square_size); y++){
        
        for (int x = 0; x< (win_width/square_size); x++){
            
            SDL_SetRenderDrawColor(renderer, color_pntr->r, color_pntr->g, color_pntr->b, 255);
            SDL_Rect rect;
            rect.h = square_size;
            rect.w = square_size;
            
            rect.x = (x*square_size);
            
            
            rect.y = y*square_size;
            
            
            SDL_RenderFillRect(renderer, &rect);
            color_pntr+=1;
        }
    }


    
}

SDL_Cell_Drawer::SDL_Cell_Drawer(int dim_x, int dim_y, const char *title="SDL_drawer", int cell_size = 1, int win_width = 500, int win_height = 500,  SDL_Color background = SDL_Color{0, 0, 0, 255}):SDL_drawer(title, win_width, win_height, background){
    this->square_size = cell_size;
    this->dim_x = dim_x;
    this->dim_y = dim_y;
    start_view = {0, 0};
    end_view = {win_width, win_height};
}
SDL_Cell_Drawer::~SDL_Cell_Drawer(){
    delete [] offsets;
    delete [] progress;
    delete [] rects;
}

void SDL_Cell_Drawer::set_view(Position start, Position end){
    std::cout<<(win_width)<<"/"<<((end.x-start.x)/(win_width/dim_x));
    square_size = (win_width)/((end.x-start.x)/(win_width/dim_x));
    start_view = start/(win_width/dim_x);
    end_view = end/(win_width/dim_x);
    std::cout<<"new view: sx="<<start_view.x<<", sy="<<start_view.y<<", ex="<<end_view.x<<", ey="<<end_view.y<<"\n";
}

void SDL_Cell_Drawer::update_with_surface(Cell* grid){
    return;
}

void SDL_Cell_Drawer::update_pixels(Cell* grid){
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_TARGET, win_width, win_height);
    SDL_SetRenderTarget(renderer, texture);

    update_grid(grid);
    SDL_SetRenderTarget(renderer, NULL);
    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);

    return;
    
    SDL_RendererInfo info;
    SDL_GetRendererInfo(renderer, &info);
    std::cout<<"renderer name: "<<info.name<<"\n";
    std::cout<<"textrue formats: "<<"\n";
    for (Uint32 i = 0; i<info.num_texture_formats; i++){
        std::cout<<SDL_GetPixelFormatName(info.texture_formats[i])<<"\n";
    }


    //SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ABGR8888, SDL_TEXTUREACCESS_STREAMING, win_width, win_height);
    unsigned int format;
    Uint32* pixels = nullptr;
    int pitch = 0;
    if (SDL_QueryTexture(texture, &format, nullptr, NULL, NULL)){
        std::cout<<SDL_GetError()<<"\n";
    }
    std::cout<<"format: "<<format<<"\n";
    if (SDL_LockTexture(texture, NULL, (void**)&pixels, &pitch)){
        std::cout<<SDL_GetError()<<"\n";
    }

    SDL_PixelFormat pixelFormat;
    pixelFormat.format = format;

    Cell* grid_pntr = grid;
    Uint32 color;
    Uint32 pixelPosition;
    for (int y = 0; y< dim_y; y++){
        
        for (int x = 0; x< dim_x; x++){
            std::cout<<"running stuff\n";
            pixelPosition = y*(pitch/sizeof(unsigned int))+x;
            std::cout<<"pos: "<<pixelPosition<<"\n";
            
            color = SDL_MapRGBA(&pixelFormat, 245, 0, 0, 255);
            
            std::cout<<"color: "<<color<<"\n";
            //SDL_SetRenderDrawColor(renderer, colors[grid_pntr->type].r, colors[grid_pntr->type].g, colors[grid_pntr->type].b, 255);
            
            pixels[pixelPosition] = color;
            std::cout<<"set data\n";
            grid_pntr++;
        }
    }
    std::cout<<"done looping\n";
    SDL_UnlockTexture(texture);

    SDL_RenderCopy(renderer, texture, NULL, NULL);
    SDL_DestroyTexture(texture);

    


}
void SDL_Cell_Drawer::update_grid(Cell* grid){
    if (rects == nullptr){
        std::cout<<"set recttsssssssssssssssss\n";
        rects = new SDL_Rect[dim_x*dim_y]{};
        offsets = new int[9]{};
        progress = new int [9]{};
        for (int r = 0; r<dim_x*dim_y; r++){
            (rects+r)->h = square_size;
            (rects+r)->w = square_size;
        }
    }
    for (int i = 0; i<9; i++){
        *(offsets+i) = 0;
        *(progress+i) = 0;
    }
    
    Cell* grid_pointer = grid;
    for (int y = 0; y<dim_y; y++){
        for (int x = 0; x<dim_x; x++){
            
            if ((x>=start_view.x)&&(x<=end_view.x)&&(y>=start_view.y)&&(y<=end_view.y)){
                for(int o = (grid_pointer->type)+1; o<9; o++){
                    (*(offsets+o))++;
                }
            }
            grid_pointer++;
        }
        
    }
    
    grid_pointer = grid;
    
    SDL_Rect* current;
    
    for (int y = 0; y< dim_y; y++){
        
        for (int x = 0; x< dim_x; x++){
            
            //make sure position is inside of view
            if ((x>=start_view.x)&&(x<=end_view.x)&&(y>=start_view.y)&&(y<=end_view.y)){
                current = (rects+( (*(offsets+grid_pointer->type)) + (*(progress+grid_pointer->type)) ));
                //(win_width/(float)(end_view.x-start_view.x))
                current->w = square_size;
                current->h = square_size;
                current->x = ((x-start_view.x))*square_size;
                current->y = ((y-start_view.y))*square_size;
                (*(progress+grid_pointer->type))++;
            }
            grid_pointer++;
        }
    }
    
    for (int type = 0; type<9; type++){
        SDL_SetRenderDrawColor(renderer, colors[type].r, colors[type].g, colors[type].b, 255);
        SDL_RenderFillRects(renderer, rects+*(offsets+type), *(progress+type));
    }
    
    
}


void SDL_Cell_Drawer::_update_grid(Cell* grid){
    
    
    
    Cell *grid_pntr = grid;
    SDL_Rect rect;
    for (int y = 0; y< dim_y; y++){
        
        for (int x = 0; x< dim_x; x++){
            
            SDL_SetRenderDrawColor(renderer, colors[grid_pntr->type].r, colors[grid_pntr->type].g, colors[grid_pntr->type].b, 255);
            
            rect.h = square_size;
            rect.w = square_size;
            
            rect.x = x*square_size;
            
            
            rect.y = y*square_size;
            
            
            SDL_RenderFillRect(renderer, &rect);
            grid_pntr+=1;
        }
    }
    

    
}









void textInput::set_bg_color(SDL_Color bg){
    this->color.r = bg.r;
    this->color.g = bg.g;
    this->color.b = bg.b;
    this->color.a = bg.a;
}
void textInput::set_box_bounds(int x, int y, int w, int h){
    this->box.x = x;
    this->box.y = y;
    this->box.w = w;
    this->box.h = h;

    this->txt_bounds.x = x;
    this->txt_bounds.y = y;
}
Sint32 textInput::get_box_x(){
    return box.x;
}
Sint32 textInput::get_box_y(){
    return box.y;
}
Uint32 textInput::get_box_w(){
    return box.w;
}
Uint32 textInput::get_box_h(){
    return box.h;
}
void textInput::set_text_color(SDL_Color color){
    set_color(color);
}
void textInput::add_char(char letter){

    if (length >= max_length) return;
    *(text+(length++)) = letter;
}
void textInput::pop(){
    if (length == 0) return;
    *(text+(--length)) = static_cast<char>(" "[0]);
}
bool textInput::is_active(){
    return active;
}
void textInput::mouse_press(SDL_MouseButtonEvent &e){
    if ((e.button == SDL_BUTTON_LEFT)&&(e.x > box.x)&&(e.y > box.y)&&(e.x < box.x+box.w)&&(e.y < box.y+box.h)){
            std::cout<<"pressed \n";
            active = true;
    }else{active = false;}
    std::cout<<"actcivity: "<<active<<"\n";
    
}
void textInput::update_txt(){
    update_texture();
}
void textInput::render(){
    SDL_SetRenderDrawColor(renderer, color.r, color.g, color.b, color.a);
    SDL_RenderFillRect(renderer, &box);
    render_txt();
    
}
void textInput::run_func(){
    function(text);
    std::cout<<"run\n";
}
void textInput::set_func(Callable<void, char*> func){
    function = func;
}
void textInput::set_font(TTF_Font* font){
    this->font = font;
}
void textInput::set_renderer(SDL_Renderer* renderer){
    this->renderer = renderer;
}
void textInput::new_text_string(int max_length){
    this->length = 0;
    this->max_length = max_length;
    this->text = new char[max_length];
    for (int i=0; i<max_length; i++){
        *(text+i) = static_cast<char>(" "[0]);
    }
    *(text+0) = static_cast<char>("0"[0]);

}
void textInput::update_text_texture(){
    update_texture();
}

textInput::textInput(){
    new_text_string(1);
    
    
}
textInput::textInput(SDL_Renderer* renderer, Callable<void, char*> func, TTF_Font* font, int max_length, SDL_Rect bounds, SDL_Color bg_color, SDL_Color text_color):SDL_text_box(renderer, font){
    //button.set_connection(&textInput::box_pressed, this);
    set_renderer(renderer);
    
    set_func(func);
    set_box_bounds(bounds.x, bounds.y, bounds.w, bounds.h);
    set_bg_color(bg_color);
    set_text_color(text_color);
    new_text_string(max_length);
    update_texture();


}
textInput::~textInput(){
    delete [] text;
}

void textInputContainer::set_item(int index, textInput item){
    *(list+index) = item;
}
void textInputContainer::key_press(SDL_Event &e){
    if (e.type != SDL_KEYDOWN) return;
    if (e.key.keysym.sym >= 33 && e.key.keysym.sym <= 126){
        for (int i = 0; i<length; i++){
            if ((list+i)->is_active() != 0){
                (list+i)->add_char((char)e.key.keysym.sym);
                (list+i)->update_txt();
            }
        }
    }else if (e.key.keysym.sym == SDLK_RETURN){

        std::cout<<"enter key\n";
        for (int i = 0; i<length; i++){
            if ((list+i)->is_active() != 0){
                (list+i)->run_func();
            }
        }
    }else if (e.key.keysym.sym == SDLK_BACKSPACE){
        std::cout<<"backspace\n";
        for (int i = 0; i<length; i++){
            if ((list+i)->is_active() != 0){
                (list+i)->pop();
                (list+i)->update_txt();
            }
        }
    }

}
void textInputContainer::mouse_press(SDL_MouseButtonEvent &e){
    for (int i = 0; i<length; i++){
        (list+i)->mouse_press(e);
    }
}
void textInputContainer::new_list(int length, SDL_Renderer* default_renderer, TTF_Font* default_font){
    this->length = length;
    list = new textInput[length];
    for (int i = 0; i<length; i++){
        (list+i)->set_renderer(default_renderer);
        (list+i)->set_font(default_font);
    }
}
void textInputContainer::set_item(int index, SDL_Renderer* renderer, Callable<void, char*> func, TTF_Font* font, int max_length, SDL_Rect bounds, SDL_Color bg_color, SDL_Color text_color){
    (list+index)->set_renderer(renderer);
    (list+index)->set_func(func);
    (list+index)->set_font(font);
    (list+index)->new_text_string(max_length);
    (list+index)->set_box_bounds(bounds.x, bounds.y, bounds.w, bounds.h);
    (list+index)->set_bg_color(bg_color);
    (list+index)->set_text_color(text_color);
    (list+index)->update_text_texture();
}
void textInputContainer::render_all(){
    for (int i = 0; i<length; i++){
        (list+i)->render();
    }
}
textInputContainer::textInputContainer(int length, SDL_Renderer* default_renderer, TTF_Font* default_font){
    new_list(length, default_renderer, default_font);
}

textInputContainer::~textInputContainer(){
    delete [] list;
}




