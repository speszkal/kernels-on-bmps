#include "dependencies.h"

Color::Color()
{
    r = 0;
    g = 0;
    b = 0;
}

Color::Color(float red, float green, float blue)
{
    r = red;
    g = green;
    b = blue;
}

Color::~Color()
{}

Image::Image()
{}

Image::~Image()
{}

void Image::Load(const char* path)
{
    std::fstream file(path, std::ios::in | std::ios::binary);

    if(!file.is_open())
    {
        std::cout<<"Error: Couldn't open the file under given path: " << path <<"\n";
        return;
    }

    unsigned char fileHeader[FileHeaderSize];
    file.read(reinterpret_cast<char*>(fileHeader), FileHeaderSize);

    unsigned char DIBHeader[DIBHeaderSize];
    file.read(reinterpret_cast<char*>(DIBHeader), DIBHeaderSize);

    if(fileHeader[0] != 'B' || fileHeader[1] != 'M')
    {
        std::cout<<"Error: The file provided is not a BMP file.\n";
        //return;
    }

    const int fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);
    const int offsetToPixelArray = fileHeader[10] + (fileHeader[11] << 8) + (fileHeader[12] << 16) + (fileHeader[13] << 24);

    width = DIBHeader[4] + (DIBHeader[5] << 8) + (DIBHeader[6] << 16) + (DIBHeader[7] << 24);
    height = DIBHeader[8] + (DIBHeader[9] << 8) + (DIBHeader[10] << 16) + (DIBHeader[11] << 24);

    pixels.resize(width*height);

    file.ignore(offsetToPixelArray - DIBHeaderSize - FileHeaderSize);

    const int paddingAmount = (4 - (width*3)%4)%4;

    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            unsigned char pix[3];
            file.read(reinterpret_cast<char*>(pix), 3);

            pixels[y*width+x].r = static_cast<float>(pix[2]) / 255.0f;
            pixels[y*width+x].g = static_cast<float>(pix[1]) / 255.0f;
            pixels[y*width+x].b = static_cast<float>(pix[0]) / 255.0f;
        }

        file.ignore(paddingAmount);
    }

    file.close();

    std::cout<<"File has been successfully read!"<<std::endl;
}

void Image::Export(const char* path)
{
    std::fstream file(path, std::ios::out | std::ios::binary);

    if(!file.is_open())
    {
        std::cout<<"Error: Couldn't open the file under the path: " << path <<"\n";
        return;
    }

    unsigned char fileHeader[FileHeaderSize];
    unsigned char DIBHeader[DIBHeaderSize];

    //#File header
    //Signature
    fileHeader[0] = 'B';
    fileHeader[1] = 'M';

    //File size
    const int paddingAmount = (4 - (width*3)%4)%4;
    const int fileSize = FileHeaderSize + DIBHeaderSize + width*height*3 + paddingAmount*height;
    
    fileHeader[2] = fileSize;
    fileHeader[3] = fileSize >> 8;
    fileHeader[4] = fileSize >> 16;
    fileHeader[5] = fileSize >> 24;

    //Reserved 1 & Reserved 2
    fileHeader[6] = 0;
    fileHeader[7] = 0;
    fileHeader[8] = 0;
    fileHeader[9] = 0;

    //File Offset to PixelArray
    const int offsetToPixelArray = FileHeaderSize + DIBHeaderSize;
    
    fileHeader[10] = offsetToPixelArray;
    fileHeader[11] = offsetToPixelArray >> 8;
    fileHeader[12] = offsetToPixelArray >> 16;
    fileHeader[13] = offsetToPixelArray >> 24;

    file.write(reinterpret_cast<char*>(fileHeader), FileHeaderSize);

    //#DIB header
    //DIB Header Size
    DIBHeader[0] = DIBHeaderSize;
    DIBHeader[1] = DIBHeaderSize >> 8;
    DIBHeader[2] = DIBHeaderSize >> 16;
    DIBHeader[3] = DIBHeaderSize >> 24;

    //Width
    DIBHeader[4] = width;
    DIBHeader[5] = width >> 8;
    DIBHeader[6] = width >> 16;
    DIBHeader[7] = width >> 24;

    //Height
    DIBHeader[8] = height;
    DIBHeader[9] = height >> 8;
    DIBHeader[10] = height >> 16;
    DIBHeader[11] = height >> 24;

    //Planes (must be 1)
    DIBHeader[12] = 1;
    DIBHeader[13] = 0;

    //Bits per Pixel (3*8 = 24)
    DIBHeader[14] = 24;
    DIBHeader[15] = 0;

    //Compression (no compression)
    for(int i=16; i<20; i++) DIBHeader[i] = 0;

    //Image Size (no compression)
    for(int i=20; i<24; i++) DIBHeader[i] = 0;

    //X Pixels Per Meter (idk)
    for(int i=24; i<28; i++) DIBHeader[i] = 0;

    //Y Pixels Per Meter (idk)
    for(int i=28; i<32; i++) DIBHeader[i] = 0;

    //Colors in Color Table (o for default 2^n)
    for(int i=32; i<36; i++) DIBHeader[i] = 0;

    //Important Colors (generally ignored)
    for(int i=36; i<40; i++) DIBHeader[i] = 0;

    file.write(reinterpret_cast<char*>(DIBHeader), DIBHeaderSize);

    //PixelArray
    unsigned char padding[] = {0, 0, 0};
    
    for(int y=0; y<height; y++)
    {
        for(int x=0; x<width; x++)
        {
            unsigned char pix[3];
            
            pix[2] = static_cast<unsigned char>(pixels[y*width+x].r * 255.0f);
            pix[1] = static_cast<unsigned char>(pixels[y*width+x].g * 255.0f);
            pix[0] = static_cast<unsigned char>(pixels[y*width+x].b * 255.0f);
            file.write(reinterpret_cast<char*>(pix), 3);
        }

        file.write(reinterpret_cast<char*>(padding), paddingAmount);
    }

    file.close();

    std::cout<<"File has been successfully created!"<<std::endl;
}

void Image::SetSize(int width, int height)
{
    this->width = width;
    this->height = height;

    pixels.resize(width*height);
}

void Image::SetColor(int x, int y, Color& rgb)
{
    pixels[y*width + x] = rgb;
}

Color Image::GetColor(int x, int y)
{
    return pixels[y*width + x];
}

/*void Image::LoadKernel(const char* path);
{

}*/

void Image::ApplyKernel(const int kernelID, float threshold)
{
    std::vector<Color> result;
    result.resize(width*height);

    Kernel k = gKernels[kernelID];
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float sumR=0, sumG=0, sumB=0; // RGB
            //float sum = 0; // Gray scale

            for(int i=0; i<k.width*k.height; i++)
            {
                if(y==0 && (i==0 || i==1 || i==2)) continue;
                if(x==0 && (i==0 || i==3 || i==6)) continue;
                if(y==height-1 && (i==6 || i==7 || i==8)) continue;
                if(x==width-1 && (i==2 || i==5 || i==8)) continue;
                
                int idx = (y + k.offsetTable[i].s) * width + (x+k.offsetTable[i].f);
                
                sumR += pixels[idx].r * k.values[i]; // RGB
                sumG += pixels[idx].g * k.values[i];
                sumB += pixels[idx].b * k.values[i]; 

                //sum += (pixels[idx].r + pixels[idx].g + pixels[idx].b) / 3 * (float)k.values[i]; // Gray scale
            }
            
            
            
            (sumR >= threshold || sumR <= -threshold) ? sumR = 1 : sumR = 0;
            (sumG >= threshold || sumG <= -threshold) ? sumG = 1 : sumG = 0;
            (sumB >= threshold || sumB <= -threshold) ? sumB = 1 : sumB = 0;

            result[y*width+x] = {sumR, sumG, sumB}; // RGB
            //result[y*width+x] = {sum, sum, sum};
        }
    }

    pixels = result;
}

void Image::ApplyGreyScale()
{
    std::vector<Color> result;
    result.resize(width*height);
    
    for(int y = 0; y < height; y++)
    {
        for(int x = 0; x < width; x++)
        {
            float avg = (pixels[y*width+x].r + pixels[y*width+x].g + pixels[y*width+x].b) / 3;

            result[y*width+x] = {avg, avg, avg};
        }
    }

    pixels = result;
}