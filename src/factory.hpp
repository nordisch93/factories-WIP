#ifndef FACTORY_HPP
#define FACTORY_HPP

#include"sqlitewrapper.hpp"
#include<list>
#include<string>

class Item{
private:
    std::string name_;
public:
    inline Item(std::string name){
        name_ = name;
    };
    
    inline std::string getName(){
        return name_;
    };
};

struct ItemWithAmount {
    Item item_;
    uint32_t amount_;
};

class Recipe{
private:
    std::string name_;
    double energyRequired_;
    std::list<ItemWithAmount> ingredients_;
    std::list<ItemWithAmount> products_;
public:
    inline Recipe(){};
    inline Recipe(std::string name, std::list<ItemWithAmount> ingredients, std::list<ItemWithAmount> products){
        name_ = name;
        products_ = products;
        ingredients_ = ingredients;
    };

    inline std::string getName(){
        return name_;
    };
    inline double getEnergyRequired(){
        return energyRequired_;
    };
};



class AM{
private:
    enum class MACHINETYPE {GREY = 0, BLUE = 1, YELLOW = 2};

    Item product_;
    MACHINETYPE type_;
    double craftingSpeed_;
    //TODO: Add support for modules

public:
    AM(MACHINETYPE type, Item product);

    inline Item getProduct(){
        return product_;
    };
    inline MACHINETYPE getType(){
        return type_;
    };
    inline double getCraftingSpeed(){
        return craftingSpeed_;
    };
};

struct Throughput{
    Item item;
    double amountPerSecond;
};

class Factory{
private:
    std::list<Throughput> outputs_;
    std::list<Throughput> inputs_;
    std::list<AM> assemblingMachines_;
public:
    Factory();
    Factory(Throughput targetThroughput);
    
    void addOutput(Throughput targetThroughput);
    void removeOutput(Item outputToRemove);
    void printOutputs();
    void printInputs();
    void printFactory();
    void drawFactory();
};

class DatabaseWrapper{
private:
    Sqlitewrapper wrapper_;

    int buildDatabase(std::string location);
    int openDatabase(const char* filename);
    int refreshDatabase(std::string location);
    int deleteDatabase();

    int addRecipe();

public:
    Recipe getRecipe(std::string name);
};


#endif
