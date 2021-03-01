#ifndef __menu_prog_h
#define __menu_prog_H

#include "menu.h"


template <typename ProgramState>
struct MenuProgramCallback {
    MenuProgramCallback (*func)(ProgramState&);
};

template <typename ProgramState>
class MenuProgram {
    protected:
        ProgramState state;
        const MenuProgramCallback<ProgramState>* functions;
        const Menu* menu;
    private:
        std::istream& in;
        std::ostream& out;

        MenuProgramCallback<ProgramState> run_function(MenuProgramCallback<ProgramState> mpcb) {
            return mpcb.func(this->state);
        }

        MenuProgramCallback<ProgramState> run_function(const unsigned int fnindex) {
            return this->run_function(this->functions[fnindex]);
        }

    public:
        MenuProgram(
            const MenuProgramCallback<ProgramState> fns[],
            const Menu* menuobj,
            std::istream& input,
            std::ostream& output
        ) :
            functions(fns),
            menu(menuobj),
            in(input),
            out(output) {

            }
        
        const bool step() {
            const unsigned int choice = this->menu->get_choice(in, out);
            MenuProgramCallback<ProgramState> mpcb = this->run_function(choice);
            bool multiple_funcs = false;
            while(mpcb.func != nullptr) {
                multiple_funcs = true;
                mpcb = this->run_function(mpcb);
            }
            return multiple_funcs;
        }
};

#endif