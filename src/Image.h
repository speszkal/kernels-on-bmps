struct Color
{
    float r, g, b;

    Color();
    Color(float r, float g, float b);
    ~Color();
};

/*
struct Kernel
{
    int width, height;
    
    int offsetTable[100][100];

    int values[100][100];
    
    Kernel();
    ~Kernel();
};
*/

class Image
{
    public:
        Image();
        ~Image();

        void Load(const char* path);
        void Export(const char* path);

        void SetSize(int width, int height);
        std::pair<int, int> GetSize() { return {width, height}; };

        void SetColor(int x, int y, Color& rgb);
        Color GetColor(int x, int y);

        //void LoadKernel(const char* path);
        void ApplyKernel(const int kernelID, float threshold);
        void ApplyGreyScale();

    private:
        int width=0, height=0;
        std::vector<Color> pixels;
        //std::vector<Kernel> kernels;
        
};