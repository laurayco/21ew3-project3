#include <iostream>
#include <zooprogram.h>

const MenuProgramCallback<ZooState>
    ZooProgram::load_data = { &load_animal_data_fn },
    ZooProgram::generate_data = { &generate_animal_data_fn },
    ZooProgram::display_data = { &display_animal_data_fn },
    ZooProgram::add_record = { &add_record_fn },
    ZooProgram::delete_record = { &delete_record_fn },
    ZooProgram::save_data = { &save_data_fn },
    ZooProgram::quit_program = { &quit_program_fn };

static const MenuProgramCallback<ZooState> __menu_functions[] = {
    ZooProgram::load_data,
    ZooProgram::generate_data,
    ZooProgram::display_data,
    ZooProgram::add_record,
    ZooProgram::delete_record,
    ZooProgram::save_data,
    ZooProgram::quit_program
};

const MainMenu ZooProgram::menu;

ZooProgram::ZooProgram(std::istream& in, std::ostream& out) :
MenuProgram<ZooState>(
    __menu_functions,
    &ZooProgram::menu,
    in,
    out
) {
    state.selected_animal = std::vector<AnimalRecord>::iterator(nullptr);
}