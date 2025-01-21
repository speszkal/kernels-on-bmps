#include "dependencies.h"

Kernel::Kernel()
{}

Kernel::~Kernel()
{}

std::vector<Kernel> gKernels;

/*Kernel gLoadKernel(const char* path)
{
    std::fstream file(path, std::ios::in);

    Kernel input;

    int w, h;

    file.read(input )
}*/