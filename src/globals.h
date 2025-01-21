struct Kernel
{
    int width, height;
    
    std::vector<std::pair<int, int>> offsetTable;

    std::vector<float> values;
    
    Kernel();
    ~Kernel();
};

extern std::vector<Kernel> gKernels;

//extern Kernel gLoadKernel(const char* path);