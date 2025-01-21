#include "src/dependencies.h"

int main()
{
    Image image;

    /*image.SetSize(100, 100);

    for(int y = 0; y < 100; y++)
    {
        for(int x = 0; x < 100; x++)
        {
            Color rgb = {0, 1, 0};
            image.SetColor(x, y, rgb);
        }
    }*/

    image.Load("data/photos/sample.bmp");

    /*for(int y = 0; y < 100; y++)
    {
        for(int x = 0; x < 100; x++)
        {
            Color rgb = {x/45.0f+y/45.0f, 0, (1-x/45.0f)+y/45.0f};
            image.SetColor(x, y, rgb);
        }
    }*/
        
    //Load Kernel
    Kernel k;

    //Perwitt Kernel
    k.width = 3;
    k.height = 3;
    
    k.offsetTable.resize(k.width*k.height);
    k.values.resize(k.width*k.height);

    k.offsetTable = { {-1, -1}, {0, -1}, {1, -1}, {-1, 0}, {0, 0}, {1, 0}, {-1, 1}, {0, 1}, {1, 1}, };
    k.values = { 1, 0, -1, 1, 0, -1, 1, 0, -1 };

    gKernels.push_back(k);
    
    Image greyScale = image;
    greyScale.ApplyGreyScale();

    //kernel operations;
    Image processedImg = greyScale;
    processedImg.ApplyKernel(0, 0.5);

    greyScale.Export("results/grey.bmp");
    processedImg.Export("results/result.bmp");
    
    //image.Export("test.bmp");

    return 0;
}