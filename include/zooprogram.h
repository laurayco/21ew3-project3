#ifndef __zoo_prog_h__
#define __zoo_prog_h__

#include <iostream>
#include <vector>
#include <string>

#include "mainmenu.h"
#include "menuprog.h"

struct AnimalRecord {
    std::string name, type, subtype;
    unsigned int num_eggs;
    unsigned int tracker;
    bool is_nursing;
};

struct ZooState {
    std::vector<AnimalRecord> inventory;
    std::vector<AnimalRecord>::iterator selected_animal;
    std::string filename;
    MenuProgramCallback<ZooState> selection_callback, filename_callback;
    AnimalRecord building_record;
};

extern MenuProgramCallback<ZooState> load_animal_data_fn(ZooState&);
extern MenuProgramCallback<ZooState> generate_animal_data_fn(ZooState&);
extern MenuProgramCallback<ZooState> display_animal_data_fn(ZooState&);
extern MenuProgramCallback<ZooState> add_record_fn(ZooState&);
extern MenuProgramCallback<ZooState> delete_record_fn(ZooState&);
extern MenuProgramCallback<ZooState> save_data_fn(ZooState&);
extern MenuProgramCallback<ZooState> quit_program_fn(ZooState&);

class ZooProgram : public MenuProgram<ZooState> {
    public:
        static const MenuProgramCallback<ZooState>
            load_data,
            generate_data,
            display_data,
            add_record,
            delete_record,
            save_data,
            quit_program;
        static const MainMenu menu;
        ZooProgram(std::istream&, std::ostream&);
};

#endif