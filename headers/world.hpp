#include <iostream>
#include <vector>


extern int dim_x;
extern int dim_y;
extern int plant_food_rate;

struct Position{
    int x, y;
    Position operator +(Position& rpos){
        return Position{x+rpos.x, y+rpos.y};
    }
    Position operator +(Position&& rpos){
        return Position{x+rpos.x, y+rpos.y};
    }
    Position operator *(Position& rpos){
        return Position{x*rpos.x, y*rpos.y};
    }
    Position operator *(Position&& rpos){
        return Position{x*rpos.x, y*rpos.y};
    }
    Position operator /(Position& rpos){
        return Position{(int)(x/rpos.x), (int)(y/rpos.y)};
    }
    Position operator /(int& div){
        return Position{(int)(x/div), (int)(y/div)};
    }
    Position operator /(int&& div){
        return Position{(int)(x/div), (int)(y/div)};
    }
    bool operator ==(const Position& other){
        
        return ((x==other.x)&&(y==other.y));
    }
    void operator +=(const Position& other){
        x+=other.x;
        y+=other.y;
    }
    void operator +=(const Position&& other){
        x+=other.x;
        y+=other.y;
    }
    void operator -=(const Position& other){
        x-=other.x;
        y-=other.y;
    }
    void operator -=(const Position&& other){
        x-=other.x;
        y-=other.y;
    }
    void operator *=(const Position& other){
        x*=other.x;
        y*=other.y;
    }
    void operator *=(const Position&& other){
        x*=other.x;
        y*=other.y;
    }
    
    
};
bool is_adjacent(const Position& left, const Position& right);
enum Cell_Type{
    EMPTY,
    FOOD,
    WALL,
    KILLER,
    SHELL,
    PLANT,
    JET,
    FILLER,
    STOMACH
};
struct Gene{
    Position pos;
    Cell_Type type;
};
struct Gene_Connection{
    std::vector<int> con_indexes{};
};
class DNA{

    int gene_count;
    Gene* genes;
    Gene_Connection* connections;

    void add(const DNA &old_dna);//sets genes to old genes, but adds one, adds to connections
    void remove(const DNA &old_dna); //sets genes to old genes, but removes on, remove from connections
    void static find_articulations(const DNA &old_dna, bool *articulation);//changes articulations list to contian true if it is an articulation point
    void change(const DNA &old_dna);//sets genes to old genes, but changes cell type of one, does not change connections
    void rotate(int direction);//rotates all the positions around the root gene, direction is either 0, 1, 2, or 3
    void copy_dna(const DNA &old_dna);//copy and pastes all dna stuff from old to new
    public:

    void set_and_mutate(const DNA &old_dna);//sets old genes to new genes, but attempts to mutate it
    Gene* get_genes() const;
    Gene_Connection* get_connections() const;
    
    void print() const;
    int get_gene_count() const;
    void set(const DNA &old_dna);//set ol dto genes to new
    DNA(const Gene* old_genes, int gene_count, bool mutate);
    DNA(const DNA &old_dna, bool mutate);
    DNA();
    ~DNA();
};







class Creature;


class Cell{
    private:
    void run_killer();
    void run_shell();
    void run_plant();
    void run_jet();
    void run_filler();
    void run_stomach();

    public:
    Cell_Type type = Cell_Type::EMPTY;
    Creature* owner = nullptr;
    void destroy();//turns cell to EMPTY
    void kill();//turns cell into FOOD
    bool hurt();//only should be called by killer cell, kills this cell if cell type doesn't say otherwise
    
    void run();// runs cell with respect to type
    
    
    
};

class Cell_Group{
    protected:
    
    
    bool move_blocked = false;
    int offset;
    Cell* temp_cell_info = nullptr;
    int temp_cell_info_length;
    static constexpr int run_length = 7;
    int current_run_length = 0;
    public:
    bool has_jet = false;
    Creature* owner;
    std::vector<Cell*> cells;
    //void run_all();//runs all cells inside of this group, if cell died then it removes it from group.
    void update(int dim_x);//updates all info including cells list, has jet, move_blocked
    bool attempt_move();//attempt to move all cells in direction, false if didn't move
    void run_all();//run all cells
    void add_cell(Cell* cell);//adds cell to vector
    void destroy_all(); // destroys all cells inside of this group
    void kill_all();//kills all cells inside  of this group
    ~Cell_Group();//destroys temp_cell_info
};

class Creature{
    protected:
    
    float mutation_rate=.001f;
    
    public:
    int age = 0;
    DNA dna;
    Position pos;
    Cell_Group cells;
    int food_count=0;
    
    bool update(int dim_x);//returns whether this creature is ready to be destroyed, updates cells too
    bool ready_reproduce();//returns true if this creature has enough food to reproduce
    void fly();//moves all cells to random dirrection, if they won't collide with other stuff
    void use_repro_food();//removes food from this creature based on dna size
    void kill_cells();//kills all cells this owns
    void destroy_cells();//destroys all cells this owns
    
    ~Creature();//cleans up genes
};

class Creature_Container{
    public:
    std::vector<Creature*> creatures;
    Creature* add_new();
    //void run_all();//runs all creatures inside of this group
    
    void destroy_all();//destroys all creatures inside of this group
    void kill_all();//kills all creatures inside of this group
    ~Creature_Container();//cleans up creatures
};

class World{
    private:
    
    public:
    
    Cell* grid = nullptr;
    Creature_Container creatures;
    int dim_x;
    int dim_y;
    

    void step();
    void run_cell(Cell* cell);
    void reproduce_creatures();//attempts to reproduce all creatures
    void run_all_cells();//runs all creaturs this world owns
    bool place_new_creature(const DNA &old_dna, Position pos, bool mutate);//attempts to place a new creature, if succesful returns true; else false
    World(int dim_x, int dim_y);//set up grid and inizilize dims
    ~World();//cleans up grid, and creatures
};


















