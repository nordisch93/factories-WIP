#include"factory.hpp"

Factory::Factory(){
    std::list<Throughput> outputs;
    std::list<Throughput> inputs;
    std::list<AM> assemblingMachines;
    outputs_ = outputs;
    inputs_ = inputs;
    assemblingMachines_ = assemblingMachines;
}
Factory::Factory(Throughput){
    std::list<Throughput> outputs;
    std::list<Throughput> inputs;
    std::list<AM> assemblingMachines;
    outputs_ = outputs;
    inputs_ = inputs;
    assemblingMachines_ = assemblingMachines;
}
