#include<iostream>
#include<string>
#include<cstring>
#include<stdexcept>
#include<sstream>

using namespace std;

class MainApp{

private:
    bool fileIsOpen = false;
    bool fileHasChanged = false;

public:
    int openFile(char* Filename) {

        //TODO: OPEN FILE
        fileIsOpen = true;
        fileHasChanged = false;
        return 0;
    };

    int closeFile() {
        if (fileHasChanged)
        {
            cout <<  "There are unsaved changes. Exit anyways? Y/N\n";
            while (true) 
            {
                char input = getchar();
                if (input == 'Y')
                {
                    //TODO close FILE
                    fileIsOpen = false;
                }
                else if (input == 'N')
                    break;
            }
        }
        else {
            //TODO close FILE
            fileIsOpen = false;
        }
        return 0;
    };
    int saveFile(char* filename) {
        //TODO SAVE FILE
        fileHasChanged = false;
        return 0;
    };

    int mainloop(char* filename){
        string input;
         while(true){
             cout << "Chose an action: Add/Remove a factory, add/remove Assembling Machines to/from an existing Factory, exit program..\n";
             cin >> input;
             if(input == "exit"){
                 cout << "exiting..";
                 //TODO exit program
                 return 0;
             }
         }
    };
};

enum class MAINLOOP_COMMAND {NEW_FACTORY = 0, LOAD_FACTORY = 1, SAVE_FACTORY = 2, EXIT = 3, HELP = 4};
enum class FACLOOP_COMMAND {ADD_AM = 0, EDIT_AM = 1, RM_AM = 2, CHANGE_RESTRICTION = 3, PRINT = 6, DISPLAY = 7, EXPORT = 8, SAVE = 10, EXIT = 11, HELP = 12};
enum class EDITLOOP_COMMAND {CHANGE_RECIPE = 0, CHANGE_TYPE = 1};



class ExceptionHandler{

public:
    ExceptionHandler(){
    }
    
    int HandleError(std::invalid_argument e) {
        cout <<  "Bad argument. For help type \"-h\"\n";
        return 1;
    }
    int HandleError(std::system_error e){
        cout << "An error occurered with errorcode " <<e.code() << " meaning " <<e.what();
        return 1;
    }
    int HandleError() {
        cout <<  "An unknown error occured.\n";
        return 1;
    }
};

class MainloopHandler {
private:
    MAINLOOP_COMMAND cmd;

public:

    MainloopHandler(int cmd, int argc, char** argv){
    }

    int execute(){
        switch (cmd) {

        case MAINLOOP_COMMAND::HELP:
             cout <<  "Another very helpful message";
             break;
        case MAINLOOP_COMMAND::NEW_FACTORY:

             break;
        case MAINLOOP_COMMAND::LOAD_FACTORY:

             break;
        case MAINLOOP_COMMAND::EXIT:

             break;
         default:
             throw std::invalid_argument("Unknown command");
             break;
        }
    }

};

class StartupHandler {
private:
        MAINLOOP_COMMAND cmd;
        char* filename;

public:

    StartupHandler(int argc,  char* argv[]) {
      if (argc < 2) {
          cmd = MAINLOOP_COMMAND::NEW_FACTORY;
          filename = "new_factory";
        }
      else if (!strcmp(argv[1], "-h") || !strcmp(argv[1], "-help"))
          cmd = MAINLOOP_COMMAND::HELP;
        else if (!strcmp(argv[1], "-n") || !strcmp(argv[1], "-new")) {
            cmd = MAINLOOP_COMMAND::NEW_FACTORY;
            if (argc < 3) {
                throw std::invalid_argument("Not enough arguments!");
            }
            filename = argv[2];
        }
            else if (!strcmp(argv[1], "-o") || !strcmp(argv[1], "-open")) {
                cmd = MAINLOOP_COMMAND::LOAD_FACTORY;
                if (argc < 3) {
                    throw std::invalid_argument("Not enough arguments!");
                }
                filename = argv[2];
            }
      else
      {
          throw std::invalid_argument("Invalid command");
      }
    }

    static void printHelp() {
        cout <<  "Startup Handler help: \n";
    }

    int execute() {
        MainApp app;
        //cout << static_cast<int>(cmd) << "\n";
        switch (cmd) {
        case MAINLOOP_COMMAND::HELP:
            printHelp();
            return 0;
            break;
        case MAINLOOP_COMMAND::NEW_FACTORY:
             try{
                 // create file
                 app.openFile(filename);
             }
             catch (const std::system_error& ex){
                 ExceptionHandler handler;
                 return handler.HandleError();
             }
             catch ( ... ) {
                 ExceptionHandler handler;
                 return handler.HandleError();
             }
             return app.mainloop(filename);
             break;
        case MAINLOOP_COMMAND::LOAD_FACTORY:
             try{
                 // create file
                 app.openFile(filename);
             }
             catch (const std::system_error& ex){
                 ExceptionHandler handler;
                 return handler.HandleError();
             }
             catch ( ... ) {
                 ExceptionHandler handler;
                 return handler.HandleError();
             }
             return app.mainloop(filename);
             break;
        }
        return 0;
    }
};

int main(int argc, char* argv[]){
    try {
        StartupHandler startup(argc, argv);
        return startup.execute();
    }
    catch (const std::invalid_argument& e) {
        ExceptionHandler handler;
        return handler.HandleError(e);
    }
    catch (const std::system_error& e){
        ExceptionHandler handler;
        return handler.HandleError(e);
    }
    catch ( ... ) {
        ExceptionHandler handler;
        return handler.HandleError();
    }
    return 0;
}
