#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <cctype>
#include <limits>
#include <jni.h>
#include "zooprogram.h"
#include "yesnomenu.h"
#include "searchtypemenu.h"
#include "animaltypemenu.h"

using namespace std;

std::string max_substr(std::string&,std::size_t);
void output_animal_vector(std::vector<AnimalRecord>&, std::ostream&);
const char* skip_spaces(char*);
void trim_right(char*);
void read_field(char*,std::size_t,std::istream&);
MenuProgramCallback<ZooState> build_record_tracker(ZooState&);
MenuProgramCallback<ZooState> build_record_name(ZooState&);
MenuProgramCallback<ZooState> build_record_type(ZooState&);
MenuProgramCallback<ZooState> build_record_subtype(ZooState&);
MenuProgramCallback<ZooState> build_record_eggs(ZooState&);
MenuProgramCallback<ZooState> build_record_nursing(ZooState&);
MenuProgramCallback<ZooState> add_build_record(ZooState&);
MenuProgramCallback<ZooState> get_filename(ZooState&);
MenuProgramCallback<ZooState> read_vector_from_file(ZooState&);
MenuProgramCallback<ZooState> write_vector_to_file(ZooState&);
MenuProgramCallback<ZooState> nop_fn(ZooState&);
MenuProgramCallback<ZooState> display_single_animal_fn(ZooState&);
MenuProgramCallback<ZooState> select_animal(ZooState&);
void GenerateData();

void GenerateData()               //DO NOT TOUCH CODE IN THIS METHOD
{
     JavaVM *jvm;                      // Pointer to the JVM (Java Virtual Machine)
     JNIEnv *env;                      // Pointer to native interface
                                                              //================== prepare loading of Java VM ============================
     JavaVMInitArgs vm_args;                        // Initialization arguments
     JavaVMOption* options = new JavaVMOption[1];   // JVM invocation options
     options[0].optionString = (char*) "-Djava.class.path=";   // where to find java .class
     vm_args.version = JNI_VERSION_1_6;             // minimum Java version
     vm_args.nOptions = 1;                          // number of options
     vm_args.options = options;
     vm_args.ignoreUnrecognized = false;     // invalid options make the JVM init fail
                                                                          //=============== load and initialize Java VM and JNI interface =============
     jint rc = JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);  // YES !!
     delete options;    // we then no longer need the initialisation options.
     if (rc != JNI_OK) {
            // TO DO: error processing...
            cin.get();
            exit(EXIT_FAILURE);
     }
     //=============== Display JVM version =======================================
     cout << "JVM load succeeded: Version ";
     jint ver = env->GetVersion();
     cout << ((ver >> 16) & 0x0f) << "." << (ver & 0x0f) << endl;

     jclass cls2 = env->FindClass("ZooFileWriter");  // try to find the class
     if (cls2 == nullptr) {
            cerr << "ERROR: class not found !";
     }
     else {                                  // if class found, continue
            cout << "Class MyTest found" << endl;
            jmethodID mid = env->GetStaticMethodID(cls2, "createZooFile", "()V");  // find method
            if (mid == nullptr)
                   cerr << "ERROR: method void createZooFile() not found !" << endl;
            else {
                   env->CallStaticVoidMethod(cls2, mid);                      // call method
                   cout << endl;
            }
     }


     jvm->DestroyJavaVM();
     cin.get();
}

MenuProgramCallback<ZooState> nop_fn(ZooState& state) {
       state.selected_animal = std::vector<AnimalRecord>::iterator(nullptr);
       state.selection_callback = state.filename_callback = { nullptr };
       state.building_record.tracker = 0;
       state.building_record.name = "";
       state.building_record.type = "";
       state.building_record.subtype = "";
       state.building_record.is_nursing = false;
       state.building_record.num_eggs = 0;
       return { nullptr };
}

MenuProgramCallback<ZooState> read_vector_from_file(ZooState& state) {
       std::ifstream input(state.filename);
       char buffer[16];
       int is_nursing;
       std::vector<AnimalRecord> imported;
       AnimalRecord build_record;
       while(input.good()&&!input.eof()) {
              read_field(buffer, 6, input);
              trim_right(buffer);
              build_record.tracker = atoi(buffer);

              read_field(buffer, 15, input);
              trim_right(buffer);
              build_record.name = skip_spaces(buffer);
              
              read_field(buffer, 15, input);
              trim_right(buffer);
              build_record.type = skip_spaces(buffer);

              read_field(buffer, 15, input);
              trim_right(buffer);
              build_record.subtype = skip_spaces(buffer);

              input >> build_record.num_eggs;
              input >> is_nursing;
              build_record.is_nursing = is_nursing == 1 ? true : false;
              
              imported.push_back(build_record);

              input.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              while(input.peek()=='\n') input.ignore(1);
       }
       input.close();

       for(AnimalRecord record : imported) {
              state.inventory.push_back(record);
       }

       std::cout << "Data loaded from '" << state.filename << "'" << std::endl;

       return { &nop_fn };
}

MenuProgramCallback<ZooState> write_vector_to_file(ZooState& state) {

       std::ofstream output(state.filename);
       output_animal_vector(state.inventory, output);
       output.close();
       std::cout << "Data saved to '" << state.filename << "'" << std::endl;

       return { &nop_fn };
}

MenuProgramCallback<ZooState> get_filename(ZooState& state) {
       bool allow_input_filename = true;
       const char* default_filename = "zoodata.txt";
       if(allow_input_filename) {
              std::cout << "filename (default: '" << default_filename << "')" << std::endl << '>';
              std::getline(std::cin, state.filename, '\n');
              if(state.filename.length() < 1) {
                     state.filename = default_filename;
              }
       } else {
              state.filename = "zoodata.txt";
       }
       return { state.filename_callback };
}

MenuProgramCallback<ZooState> call_java_generate_fn(ZooState& state) {
       GenerateData();
       return { nop_fn };
}

MenuProgramCallback<ZooState> load_animal_data_fn(ZooState& state) {
       state.filename_callback = { &read_vector_from_file };
       return { &get_filename };
}

MenuProgramCallback<ZooState> generate_animal_data_fn(ZooState& state) {
       std::cout << "GENERATE ANIMAL DATA" << std::endl;
     /*
            TODO: Write proper code to load data from input file (generated using JNI) into vector/array.
     */
       return { &call_java_generate_fn };
}

MenuProgramCallback<ZooState> display_single_animal_fn(ZooState& state) {
       std::vector<AnimalRecord> temp_inventory;
       temp_inventory.push_back(AnimalRecord(*state.selected_animal));
       auto width = std::cout.width();
       const auto adjust = std::cout.adjustfield;
       std::cout 
              << std::setw(6) << "Track" << std::left
              << std::setw(15) << "Name"
              << std::setw(15) << "Type"
              << std::setw(15) << "Sub-Type"
              << " # Eggs / Is nursing" << std::endl << std::setw(width);
       std::cout.setf(adjust);
       output_animal_vector(temp_inventory, std::cout);
       return { &nop_fn };
}

MenuProgramCallback<ZooState> display_animal_data_fn(ZooState& state) {

       YesNoMenu ynm;
       std::cout << "Are you looking for a specific animal?" << std::endl;
       const unsigned int choice = ynm.get_choice(std::cin, std::cout);
       
       if(choice==1) {
              state.selection_callback = { &display_single_animal_fn };
              return { &select_animal };
       } else {
              auto width = std::cout.width();
              const auto adjust = std::cout.adjustfield;
              std::cout 
                     << std::setw(6) << "Track" << std::left
                     << std::setw(15) << "Name"
                     << std::setw(15) << "Type"
                     << std::setw(15) << "Sub-Type"
                     << " # Eggs / Is nursing" << std::endl << std::setw(width);
              std::cout.setf(adjust);
              output_animal_vector(state.inventory, std::cout);

              return { nop_fn };
       }
}

MenuProgramCallback<ZooState> build_record_tracker(ZooState& state) {
       do {
              std::cout << "Animal tracking number" << std::endl << '>';
              std::cin >> state.building_record.tracker;
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              if(state.building_record.tracker > 999999) {
                     std::cerr << "Invalid tracking number. Must be less than 1000000" << std::endl;
              }
       } while(state.building_record.tracker>999999);
       return { &build_record_name };
}

MenuProgramCallback<ZooState> build_record_name(ZooState& state) {
       std::cout << "Animal name" << std::endl << '>';
       std::getline(std::cin, state.building_record.name, '\n');
       return { &build_record_type };
}

MenuProgramCallback<ZooState> build_record_type(ZooState& state) {
       AnimalTypeMenu typemenu;
       std::cout << "Animal type" << std::endl;
       const unsigned int animal_type = typemenu.get_choice(std::cin, std::cout);
       state.building_record.type = typemenu.get_choice_text(animal_type);
       return { &build_record_subtype };
}

MenuProgramCallback<ZooState> build_record_subtype(ZooState& state) {
       std::cout << "Animal sub-type" << std::endl << '>';
       std::getline(std::cin, state.building_record.subtype, '\n');
       return { &build_record_eggs };
}

MenuProgramCallback<ZooState> build_record_eggs(ZooState& state) {
       if(state.building_record.type=="Mammal") {
              // mammals will always have zero eggs.
              state.building_record.num_eggs = 0;
       } else {
              std::cout << "How many eggs are there?" << std::endl << '>';
              std::cin >> state.building_record.num_eggs;
       }
       return { &build_record_nursing };
}

MenuProgramCallback<ZooState> build_record_nursing(ZooState& state) {
       YesNoMenu yesnomenu;
       std::cout << "Is the animal nursing?" << std::endl;
       state.building_record.is_nursing = yesnomenu.get_choice(std::cin, std::cout);
       return { &add_build_record };
}

MenuProgramCallback<ZooState> add_build_record(ZooState& state) {
       YesNoMenu confirm;
       std::cout << "Are you sure you wish to add this record?" << std::endl;
       if(confirm.get_choice(std::cin, std::cout)==1) {
              state.inventory.push_back(AnimalRecord(state.building_record));
       }
       return { &nop_fn };
}

MenuProgramCallback<ZooState> add_record_fn(ZooState& state) {
       return { &build_record_tracker };
}

MenuProgramCallback<ZooState> delete_selected_animal(ZooState& state) {
       YesNoMenu confirm;
       std::cout << "Are you sure you wish to delete this record?" << std::endl;
       if(confirm.get_choice(std::cin, std::cout)==1) {
              state.inventory.erase(state.selected_animal);
       }
       return { nop_fn };
}

MenuProgramCallback<ZooState> select_animal(ZooState& state) {
       SearchTypeMenu search_type;
       std::cout << "Do you wish to search by tracker id, or name?" << std::endl;
       const unsigned int choice = search_type.get_choice(std::cin, std::cout);

       while(choice == 0) {
              int id;
              std::cout << "Animal tracker id" << std::endl << '>';
              std::cin >> id;
              std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
              for(auto it=state.inventory.begin();it<state.inventory.end();it++) {
                     if(it->tracker == id) {
                            state.selected_animal = it;
                            return state.selection_callback;
                     }
              }
       }

       while(choice == 1) {
              std::cout << "Animal name" << std::endl << '>';
              std::string buffer;
              std::getline(std::cin, buffer, '\n');
              std::cout << "Searching for '" << buffer << "'" << std::endl;
              for(auto it=state.inventory.begin();it<state.inventory.end();it++) {
                     std::cout << "Found '" << it->name << "'" << std::endl;
                     if(it->name.compare(buffer)==0) {
                            state.selected_animal = it;
                            return state.selection_callback;
                     }
              }
       }

       return state.selection_callback;
}

MenuProgramCallback<ZooState> delete_record_fn(ZooState& state) {
       if(state.inventory.size() < 1) {
              std::cerr << "There are no records to delete." << std::endl;
              return { nop_fn };
       } else {
              state.selection_callback = { &delete_selected_animal };
              return { &select_animal };
       }
}

MenuProgramCallback<ZooState> save_data_fn(ZooState& state) {
       state.filename_callback = { &write_vector_to_file };
       return { &get_filename };
}

MenuProgramCallback<ZooState> quit_program_fn(ZooState& state) {
       return { nullptr };
}

std::string max_substr(std::string& src, std::size_t size) {
       return src.substr(0,std::min(src.size(), size));
}


const char* skip_spaces(char* src) {
       char* ptr = src;
       while(*ptr==' ') ptr++;
       return ptr;
}

void trim_right(char* ptr) {
       char* end = ptr + strlen(ptr);
       while((isspace(*end)||*end=='\0')&&end>=ptr) *end--='\0';
}

void read_field(char* buffer,std::size_t size,std::istream& input) {
       input.read(buffer, size);
       buffer[size] = '\0';
}

void output_animal_vector(std::vector<AnimalRecord>& vec, std::ostream& out) {
       const auto TRACK_FIELD = std::setw(6),
         NAME_FIELD = std::setw(15),
         TYPE_FIELD = std::setw(15),
         SUB_TYPE_FIELD = std::setw(15),
         DEFAULT_WIDTH = std::setw(out.width());
       const auto adjust = out.adjustfield;

       for(AnimalRecord record : vec) {
              out << std::right
                     << TRACK_FIELD << std::setfill('0') << record.tracker
                     << std::setfill(' ') << std::left
                     << NAME_FIELD << max_substr(record.name, 15)
                     << TYPE_FIELD <<max_substr(record.type, 15)
                     << SUB_TYPE_FIELD << max_substr(record.subtype, 15)
                     << DEFAULT_WIDTH
                     << ' ' << record.num_eggs
                     << ' ' << (record.is_nursing ? 1 : 0) << std::endl;
       }

       out.setf(adjust);
}

int main(const int argc, char** argv) {
       ZooProgram program(std::cin, std::cout);
       while(program.step());

	return 1;
}
