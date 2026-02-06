#include "world.hpp"
#include <cmath>



bool is_adjacent(const Position& left, const Position& right){
    return (((std::abs(left.x-right.x) == 1)&&(left.y-right.y == 0)) || ((left.x-right.x == 0)&&(std::abs(left.y-right.y) == 1)));
}


void DNA::find_articulations(const DNA &old_dna, bool *articulation){
    int* disc = new int[old_dna.get_gene_count()]{};//list in order of discovery time, contains index of gene
    int* parent = new int[old_dna.get_gene_count()]{};//list in gene order, contians index of direct parernt in BFS tree
    bool* unvisited = new bool[old_dna.get_gene_count()]{};//list in gene order, contians if this gene has been visited
    
    //use BFS to fill discovery times
    int root = rand()%(old_dna.get_gene_count());
    int disc_counter = 0;
    int current = root;


    int found_new_cell = true;
    
    //make BFS tree
    //wwhile hasn't visited every cell(to few discovered cells)
    while (disc_counter < old_dna.get_gene_count()){
        //if current cell was just found
        if (found_new_cell){
            //add current node to discovery, and mark it as visited
            *(disc+disc_counter) = current;
            *(unvisited+current) = true;
            //no longer this node is just found
            found_new_cell = false;
            //increase the amount of nodes discovered
            disc_counter++;
            
        } else {//only run if current cell has already been found
            
            
            //for every adjent cell next to current
            for (int adj = 0; adj<(old_dna.connections+current)->con_indexes.size(); adj++){
                //if the adj cell is unvisited, then set it as current cell
                //THERE IS NO WAY FOR A PARENT TO BE UNDISOVERED
                if (*(unvisited+  (old_dna.connections+current)->con_indexes[adj]) == false){
                    found_new_cell = true;
                    //set parent of adj cell to current
                    *(parent+(old_dna.connections+current)->con_indexes[adj]) = current;
                    current = (old_dna.connections+current)->con_indexes[adj];
                    break;
                }
            }
            //if no unvisited cell was fonud by that for loop, then set current to parent
            if (!found_new_cell){
                int found_parent = false;
                //go backwards through disc, so you find youngest parent
                current = *(parent+current);
                
            }
        }
    }
    //BFS tree is made

    //adj_disc = the backtrack numeber, adj_disc used to detect loops, (adj_disc = lowest adj_disc or disc of an adjacent cell that is not a parent)
    int* adj_disc = new int[old_dna.get_gene_count()]{};//list in gene order, contains the lowest adjacent discovery time
    //init with each genes respective discovery number
    for (int i = 0; i<old_dna.get_gene_count(); i++){
        *(adj_disc+*(disc+i)) = i;
    }
    
    //going backwards through discovery
    for (int current_disc = old_dna.get_gene_count()-1; current_disc>=0; current_disc--){
        // current gene index
        int gene = *(disc+current_disc);
        //skip gene if it is the root, but is not an artic if dna has less than 3 genes
        if (gene == root){
            *(articulation+gene) = (old_dna.get_gene_count()>2);
            continue;
        }
        
        
        //for every adjacent cell next to current gene
        for (int adj = 0; adj <(old_dna.connections+gene)->con_indexes.size(); adj++){
            
            //adj_gene index
            int adj_gene = (old_dna.connections+gene)->con_indexes[adj];
            
            //if the adj is the parent, then skip(parents don't matter to children)
            if (*(parent+gene) == adj_gene) continue;
            

            //if the adj has was discovered earlier or adj_disc is less, then make current gene's adj_disc equal to it
            //(detects if a backedge is present)
            if (*(adj_disc+adj_gene) < *(adj_disc+gene)){
                *(adj_disc+gene) = *(adj_disc+adj_gene);

            } else if (*(adj_disc+adj_gene) >= current_disc){// ONLY CAN HAPPEN IF ADJ IS A CHILD and doesn't has a backedge to ealrier node
                //so if child does have backedge to a ealier node, make current gene a articulation node
                *(articulation+gene) = true;
            }
        }
    }
    //clean up
    delete [] disc;
    delete [] parent;
    delete [] unvisited;
    delete [] adj_disc;
}

void DNA::remove(const DNA &old_dna){
    
    
    //make space for genes and connecitons
    gene_count = old_dna.get_gene_count()-1;
    genes = new Gene[gene_count];
    connections = new Gene_Connection[gene_count];
    
    bool* articulation = new bool[gene_count+1]{};
    find_articulations(old_dna, articulation);
    
    //get numebr of removable genes
    int removable_count = 0;
    for (int i = 0; i<gene_count+1; i++){
        if (*(articulation+i) == false){
            removable_count++;
        }
    }
    
    //set genes, and find the remove index
    int remove_index = rand()%removable_count;
    int new_i = 0;
    bool removed = false;
    for (int old = 0; old<gene_count+1; old++){
        
        if ((!removed)&&(*(articulation+old) == true)){
            remove_index++;
        } else if((!removed)&&(remove_index == old)){
            removed = true;
            
            continue;
        }
    }

    //set and adjust connections to acount for removed gene
    new_i = 0;
    for (int old = 0; old<gene_count+1; old++){

        if (old == remove_index){
            continue;
        }
        if (remove_index == 0){
            *(genes+new_i) = *(old_dna.get_genes()+old);
            (genes+new_i)->pos -= (old_dna.get_genes()+1)->pos;
        }else{
            *(genes+new_i) = *(old_dna.get_genes()+old);
        }
        
        *(connections+new_i) = *(old_dna.get_connections()+old);
        int remove_adj = -1;
        for (int adj = 0; adj < (connections+new_i)->con_indexes.size(); adj++){
            
            if ((connections+new_i)->con_indexes[adj] == remove_index){
                remove_adj = adj;
                continue;
            } else if ((connections+new_i)->con_indexes[adj] > remove_index){
                (connections+new_i)->con_indexes[adj]--;
            }
        }
        if (remove_adj != -1){
            (connections+new_i)->con_indexes.erase((connections+new_i)->con_indexes.begin()+remove_adj);
        }
        
        new_i++;

    }
    //clean
    delete [] articulation;


}
void DNA::add(const DNA &old_dna){
    
    //make space for genes and connecitons
    gene_count = old_dna.get_gene_count()+1;
    genes = new Gene[gene_count];
    connections = new Gene_Connection[gene_count];

    //set genes, exept last one, last is reserved for the new cell
    for (int i = 0; i<gene_count-1; i++){
        *(connections+i) = *(old_dna.get_connections()+i);
        
        *(genes+i) = *(old_dna.get_genes()+i);
    }
    
    int root_index;
    Position new_pos;
    bool is_occupied = true;
    
    //search for a cell that is not occupied, and adjacent
    while (true){
        //pick a random root cell
        root_index = rand()%(gene_count-1);

        //make sure that it has less than 4 adjacent cells(less than 4 connections)(has space for another adjacent cell)
        if ((connections+root_index)->con_indexes.size() >= 4)continue;//make sure root doesn't have all adjecent filled aleardy
        
        
        while (is_occupied){
            
            
            int direction = rand()%4;
            //get new position due to a random direciton
            switch (direction)
            {
            case 0:
                new_pos = (genes+root_index)->pos + Position{0, -1};
                break;
            case 1:
                new_pos = (genes+root_index)->pos + Position{0, 1};
                break;
            case 2:
                new_pos = (genes+root_index)->pos + Position{-1, 0};
                break;
            case 3:
                new_pos = (genes+root_index)->pos + Position{1, 0};
                break;
            }
            
            
            //check if this new position is already occupdied by an adjacent cell to root
            is_occupied = false;
            for (int i = 0; i<(connections+root_index)->con_indexes.size(); i++){
                //this if statement means:    if new_pos == current root adjacent cell position(if is spot is already occupdied)
                if (new_pos == (genes+((connections+root_index)->con_indexes[i]))->pos){
                    //this direction is occupied, go to next direciton
                    is_occupied = true;
                    break;
                }
            }

        }
        //will break if and only if the root cell doesn't have all adjacent cells filled aleady
        break;
    }
    
    (genes+(gene_count-1))->pos = new_pos;
    (genes+(gene_count-1))->type = Cell_Type((rand()%4)+3);
    
    for (int i = 0; i<gene_count-1; i++){
        if (is_adjacent(new_pos, (genes+i)->pos)){
            
            //add two way connection between new cell and old cell, only if it is adjactent
            (connections+i)->con_indexes.push_back(gene_count-1);
            (connections+gene_count-1)->con_indexes.push_back(i);
        }
    }


}
void DNA::change(const DNA &old_dna){
    
    copy_dna(old_dna);
    (genes+(rand()%gene_count))->type = Cell_Type((rand()%4)+3);
}
void DNA::rotate(int direction){
    int temp_x;
    switch (direction)
    {
        case 0:
            for (int g = 0; g<gene_count; g++){
                temp_x = (genes+g)->pos.x;
                (genes+g)->pos.x = -std::move((genes+g)->pos.y);
                (genes+g)->pos.y = std::move(temp_x);
            }
            break;
        case 1:
            for (int g = 0; g<gene_count; g++){
                temp_x = (genes+g)->pos.x;
                (genes+g)->pos.x = std::move((genes+g)->pos.y);
                (genes+g)->pos.y = -std::move(temp_x);
            }
            break;
        case 2:
            for (int g = 0; g<gene_count; g++){
                (genes+g)->pos*=Position{-1, -1};
            }
            break;
        case 3:
            
            break;
    
        default:
            break;
    }
}
void DNA::copy_dna(const DNA &old_dna){
    gene_count = old_dna.get_gene_count();
    genes = new Gene[gene_count];
    connections = new Gene_Connection[gene_count];

    for (int i = 0; i<gene_count; i++){
        *(connections+i) = *(old_dna.get_connections()+i);
        *(genes+i) = *(old_dna.get_genes()+i);
    }
}
void DNA::print() const{
    //make sure connections are right
    std::cout<<"connections: \n";
    for (int i = 0; i<gene_count; i++){
        for (int m = 0; m<(connections+i)->con_indexes.size(); m++){
            std::cout<<(connections+i)->con_indexes[m]<<", ";
        }
        std::cout<<" : "<<i<<"\n";
    }
    std::cout<<"genes: \n";
    for (int i = 0; i<gene_count; i++){
        std::cout<<(genes+i)->type<<" : "<<(genes+i)->pos.x<<", "<<(genes+i)->pos.y<<"\n";
    }
}
void DNA::set_and_mutate(const DNA &old_dna){
    int rand_val = rand()%100;
    if (rand_val <70){
        copy_dna(old_dna);
    }else if (rand_val<80){
        add(old_dna);
    }else if (rand_val<90){
        if (old_dna.get_gene_count()<=2){
            copy_dna(old_dna);
        }else{
            remove(old_dna);
        }
    }else if (rand_val<100){
        change(old_dna);
    }
    rotate(rand_val%4);
    
    
}
void DNA::set(const DNA &old_dna){
    copy_dna(old_dna);
}
Gene* DNA::get_genes() const{
    return genes;
}
Gene_Connection* DNA::get_connections() const{
    return connections;
}
int DNA::get_gene_count() const{
    return gene_count;
}
DNA::DNA(const DNA& old_dna, bool mutate){
    if (mutate){
        set_and_mutate(old_dna);
    }else{
        set(old_dna);
    }
}
DNA::DNA(){
    
}
DNA::DNA(const Gene* old_genes, int old_gene_count, bool mutate){
    if (mutate){


    }else{
        gene_count = old_gene_count;
        genes = new Gene[old_gene_count];
        connections = new Gene_Connection[old_gene_count];
        //set genes

        for (int i = 0; i<gene_count; i++){
            
            *(genes+i) = *(old_genes+i);
        }
        //find and set all connections
        for (int root = 0; root<gene_count; root++){
            for (int adj = root; adj<gene_count; adj++){
                //make sure they aren't the same cell or already checked
                if ((root == adj)) continue;
                //if the root and adj are adjacent, then connect them
                
                if (is_adjacent((genes+root)->pos, (genes+adj)->pos)){
                    (connections+root)->con_indexes.push_back(adj);
                    (connections+adj)->con_indexes.push_back(root);
                }
            }
        }
    }
}
DNA::~DNA(){
    delete [] connections;
    delete [] genes;

}


void Cell::run_killer(){
    
    bool killed = false;
    if (((this-1)->owner != nullptr)&&((this-1)->owner != owner)){
        //check if the adjacent cell can hurt current cell
        if ((this-1)->type == Cell_Type::KILLER){
            //hurt current cell
            killed = true;
        }
        //hurt adjacent cell
        (this-1)->hurt();
    }
    if (((this+1)->owner != nullptr)&&((this+1)->owner != owner)){
        //check if the adjacent cell can hurt current cell
        if ((this+1)->type == Cell_Type::KILLER){
            //hurt current cell
            killed = true;
        }
        //hurt adjacent cell
        (this+1)->hurt();
    }
    if (((this-dim_x)->owner != nullptr)&&((this-dim_x)->owner != owner)){
        //check if the adjacent cell can hurt current cell
        if ((this-dim_x)->type == Cell_Type::KILLER){
            //hurt current cell
            killed = true;
        }
        //hurt adjacent cell
        (this-dim_x)->hurt();
    }
    if (((this+dim_x)->owner != nullptr)&&((this+dim_x)->owner != owner)){
        //check if the adjacent cell can hurt current cell
        if ((this+dim_x)->type == Cell_Type::KILLER){
            //hurt current cell
            killed = true;
        }
        //hurt adjacent cell
        (this+dim_x)->hurt();
    }
    if (killed) hurt();
}
void Cell::run_shell(){
    return;
}
void Cell::run_plant(){
    if (owner->cells.has_jet) return;
    float rand_val = rand()%100;
    if (rand_val>plant_food_rate) return;
    if (rand_val<plant_food_rate*.25){
        if ((this-1)->type == Cell_Type::EMPTY){
            (this-1)->type = Cell_Type::FOOD;
        }
    }else if (rand_val<plant_food_rate*.5){
        if ((this+1)->type == Cell_Type::EMPTY){
            (this+1)->type = Cell_Type::FOOD;
        }
    }else if (rand_val<plant_food_rate*.75){
        if ((this-dim_x)->type == Cell_Type::EMPTY){
            (this-dim_x)->type = Cell_Type::FOOD;
        }
    }else{
        if ((this+dim_x)->type == Cell_Type::EMPTY){
            (this+dim_x)->type = Cell_Type::FOOD;
        }
    }
    return;
}
void Cell::run_jet(){
    return;
}
void Cell::run_filler(){
    return;
}
void Cell::run_stomach(){

    
    if ((this-1)->type == Cell_Type::FOOD){
        (this-1)->type = Cell_Type::EMPTY;
        owner->food_count++;
    }
    if ((this+1)->type == Cell_Type::FOOD){
        (this+1)->type = Cell_Type::EMPTY;
        owner->food_count++;
    }
    if ((this-dim_x)->type == Cell_Type::FOOD){
        (this-dim_x)->type = Cell_Type::EMPTY;
        owner->food_count++;
    }
    if ((this+dim_x)->type == Cell_Type::FOOD){
        (this+dim_x)->type = Cell_Type::EMPTY;
        owner->food_count++;
    }
}


void Cell::run(){
    
    switch (type)
    {
    case Cell_Type::EMPTY:
        std::cout<<"ran empty?\n";
        break;
    case Cell_Type::FOOD:
        break;
    case Cell_Type::WALL:
        if (owner != nullptr){
            std::cout<<"diff parent";
            abort();
        }
        break;
    case Cell_Type::KILLER:
        run_killer();
        break;
    case Cell_Type::SHELL:
        run_shell();
        break;
    case Cell_Type::PLANT:
        run_plant();
        break;
    case Cell_Type::JET:
        run_jet();
        break;
    case Cell_Type::FILLER:
        run_filler();
        break;
    case Cell_Type::STOMACH:
        run_stomach();
        break;
    }
    
    
}

void Cell::destroy(){
    type = Cell_Type::EMPTY;
    owner = nullptr;
}
void Cell::kill(){
    if (type == Cell_Type::WALL){
        
        abort();
    }
    type = Cell_Type::FOOD;
    owner = nullptr;
}
bool Cell::hurt(){
    if (type!=Cell_Type::SHELL){
        kill();
        return true;
    }
    return false;
}




void Cell_Group::update(int dim_x){
    move_blocked = !has_jet;
    if (!has_jet){//if didn't have a jet last update, then it can't move(offset = 0)
        current_run_length = 0;
        offset = 0;
        
        
    }else if ((current_run_length==0)||(current_run_length>=run_length)){//if it had a jet last update, then make random offset
        
        switch (rand()%4)
        {
        case 0:
            offset = -1;
            break;
        case 1:
            offset = 1;
            break;
        case 2:
            offset = -dim_x;
            break;
        case 3:
            offset = dim_x;
            break;
        }
        current_run_length = 1;
    }else{
        current_run_length++;
    }
    
    has_jet = false;
    

    delete [] temp_cell_info;
    temp_cell_info_length = cells.size();
    temp_cell_info = new Cell[temp_cell_info_length];

    int new_i = 0;
    for (int i = 0; i<temp_cell_info_length; i++){

        //remove cell from vector if it is was deleted
        if ((cells[new_i])->type == Cell_Type::WALL){
            std::cout<<"inhatences \n";
            abort();
        }
        if (cells[new_i]->owner != owner){
            
            if (cells.size()-1<2){
                
                kill_all();
                return;
            }
            
            if ((offset != 0)&&(!move_blocked)){
                //make sure cell is still accounted for in temp cell info
                *(temp_cell_info+i) = Cell{Cell_Type::EMPTY, nullptr};
            }
            
            cells.erase(cells.begin()+new_i);
            continue;
        }
        
        //update has_jet var
        if ((!has_jet)&&(cells[new_i]->type == Cell_Type::JET)){
            has_jet = true;
        }
        //make sure that move is not blocked
        if ((offset != 0)&&(!move_blocked)  &&  ((cells[new_i]+offset)->type != Cell_Type::EMPTY) && ((cells[new_i]+offset)->owner != owner)){
            move_blocked = true;
        }else{
            if ((cells[new_i])->type == Cell_Type::WALL){
                std::cout<<"inhatences \n";
                abort();
            }
            *(temp_cell_info+i) = *(cells[new_i]);
        }
        new_i++;
    }
    if ((has_jet)&&(!move_blocked)&&(offset!=0)){
        owner->pos += Position{offset%dim_x, static_cast<int>(std::floor(offset/dim_x))};
    }else{
        current_run_length = 0;
    }
}
bool Cell_Group::attempt_move(){

    
    
    
    //make sure this creature can move
    if ((!has_jet)||(move_blocked)||(offset==0)) return false;
    int new_i = 0;
    //clear all the cells that this creature owns
    for (int i = 0; i<temp_cell_info_length; i++){
        if ((temp_cell_info+i)->owner != nullptr){
            *(cells[new_i]) = {Cell_Type::EMPTY, nullptr};
            new_i++;
        }
        
    }
    //paste all tempararly stored cells back onto the grid, with a displacement
    new_i = 0;
    for (int i = 0; i<temp_cell_info_length; i++){
        if ((temp_cell_info+i)->owner != nullptr){
            cells[new_i]+=offset;
            *(cells[new_i]) = std::move(*(temp_cell_info+i));
            new_i++;
        }
        
        
    }

    
    return true;
};
void Cell_Group::run_all(){
    for (Cell* c:cells){
        c->run();
    }
}
void Cell_Group::add_cell(Cell* cell){
    cells.push_back(cell);
}
void Cell_Group::destroy_all(){
    for (Cell* c:cells){
        if (c->type == Cell_Type::WALL){
            std::cout<<"kill ";
            abort();
        }
        c->destroy();
    }
    cells.clear();
}
void Cell_Group::kill_all(){
    for (Cell* c:cells){
        if (c->type == Cell_Type::WALL){
            std::cout<<"kill ";
            abort();
        }
        c->kill();
    }
    cells.clear();
}
Cell_Group::~Cell_Group(){
    delete [] temp_cell_info;
}











bool Creature::update(int dim_x){
    age++;
    cells.update(dim_x);

    //if all cells are killed, then return true(creature has to be destroyed)
    if ((cells.cells.size() == 0)||(age>dna.get_gene_count()*dna.get_gene_count()*7)){
        
        return true;
    }
    return false;
}
void Creature::use_repro_food(){
    food_count-=2*dna.get_gene_count();
}
bool Creature::ready_reproduce(){
    return (food_count>2*dna.get_gene_count());
}
void Creature::kill_cells(){
    cells.kill_all();
}
void Creature::destroy_cells(){
    cells.destroy_all();
}
void Creature::fly(){
    cells.attempt_move();
}
Creature::~Creature(){
    
}



Creature* Creature_Container::add_new(){
    Creature* new_creature = new Creature{};
    creatures.push_back(new_creature);
    return new_creature;
}
void Creature_Container::destroy_all(){
    for (Creature* c:creatures){
        c->destroy_cells();
        delete c;
    }
    creatures.clear();
}
void Creature_Container::kill_all(){
    for (Creature* c:creatures){
        c->kill_cells();
        delete c;
    }
    creatures.clear();
}
Creature_Container::~Creature_Container(){
    destroy_all();
}




void World::step(){
    //update cells
    for (int i = 0; i < creatures.creatures.size();){
        
        if (creatures.creatures[i]->update(dim_x)){
            creatures.creatures[i]->kill_cells();
            delete creatures.creatures[i];
            creatures.creatures.erase(creatures.creatures.begin()+i);
        }else{
            i++;
        }
        
    }
    //fly
    for (int i = 0; i < creatures.creatures.size(); i++){
        creatures.creatures[i]->fly();
    }
    
    
    run_all_cells();
    reproduce_creatures();
}
void World::run_all_cells(){
    
    for (Creature* c:creatures.creatures){
        
        c->cells.run_all();
        
    }
}
bool World::place_new_creature(const DNA &old_dna, Position pos, bool mutate){
    
    
    DNA new_dna(old_dna, mutate);
    Gene* new_genes = new_dna.get_genes();
    //check if all cells are empty
    
    for (int i = 0; i<new_dna.get_gene_count(); i++){
        Position cell_pos = (new_genes+i)->pos+pos;
        
        if ((cell_pos.x<0)||(cell_pos.y<0)||(cell_pos.x>=dim_x)||(cell_pos.y>=dim_y)||((grid+cell_pos.x+(cell_pos.y*dim_x))->type != Cell_Type::EMPTY)) return false;
    }
    
    //make new creature
    Creature* new_creature = creatures.add_new();
    new_creature->cells.owner = new_creature;
    //set genes
    new_creature->dna.set(new_dna);
    new_creature->pos = pos;
    
    
    //place cells on grid
    for (int i = 0; i<new_dna.get_gene_count(); i++){
        
        Position cell_pos = (new_genes+i)->pos+pos;
        if ((grid+cell_pos.x+(cell_pos.y*dim_x))->type == Cell_Type::WALL){
            
            abort();
        }
        (grid+cell_pos.x+(cell_pos.y*dim_x))->type = (new_genes+i)->type;
        (grid+cell_pos.x+(cell_pos.y*dim_x))->owner = new_creature;
        new_creature->cells.add_cell((grid+cell_pos.x+(cell_pos.y*dim_x)));
    }
    
    return true;
}
World::World(int dim_x, int dim_y){
    this->dim_x = dim_x;
    this->dim_y = dim_y;
    grid = new Cell[(dim_x)*(dim_y)];
    Cell empty;

    //populate grid
    Cell* current_cell=grid;
    for (int y=0; y<dim_y; y++){
        for (int x=0; x<dim_x; x++){
            if ((x==0)||(x==dim_x-1)||(y==0)||(y==dim_y-1)){
                current_cell->type = Cell_Type(2);
                current_cell->owner = nullptr;
            }else{
                current_cell->type = Cell_Type(0);
                current_cell->owner = nullptr;
            }
            current_cell++;
        }
    }

}
World::~World(){
    creatures.destroy_all();
    delete [] grid;
}








void World::reproduce_creatures(){
    for (int i = 0; i < creatures.creatures.size(); i++){
        
        if (creatures.creatures[i]->ready_reproduce()){
            creatures.creatures[i]->use_repro_food();
            Position new_pos = creatures.creatures[i]->pos;
            switch (rand()%4)
            {
            case 0:
                new_pos = new_pos+Position{-5, -5};
                break;
            case 1:
                new_pos = new_pos+Position{5, -5};
                break;
            case 2:
                new_pos = new_pos+Position{5, 5};
                break;
            case 3:
                new_pos = new_pos+Position{-5, 5};
                break;
            }
            
            
            place_new_creature(creatures.creatures[i]->dna, new_pos, true);
        }
    }
}




