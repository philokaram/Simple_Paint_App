#include <vector>
#include <fstream>

struct Point
{
    double x,y;
    Point(double x = 0.0, double y = 0.0):x(x),y(y) {}
};
//Utility
void swap(int &x1,int &y1, int &x2, int &y2){
    int tempX = x1, tempY = y1;
    x1 = x2;
    y1 = y2;
    x2 = tempX;
    y2 = tempY;
}

void swap(Point &p1,Point &p2){
    Point temp(p1.x,p1.y);
    p1.x = p2.x;
    p1.y = p2.y;
    p2.x = temp.x;
    p2.y = temp.y;
}

int Round(double e){
    return (int) (e+0.5);
}


Point operator*(double d, Point p){
    return Point(p.x *d, p.y *d);
}

Point operator+(Point p1, Point p2){
    return Point(p1.x + p2.x, p1.y + p2.y);
}
Point operator-(Point p1, Point p2){
    return Point(p1.x - p2.x, p1.y - p2.y);
}



union OutCode
{
    struct {unsigned left:1,right:1,top:1,bottom:1;};
    unsigned all:4;
};


// Save as .bmp
#pragma pack(push, 1)

struct BMPFileHeader {
    uint16_t bfType = 0x4D42;  // 'BM'
    uint32_t bfSize;
    uint16_t bfReserved1 = 0;
    uint16_t bfReserved2 = 0;
    uint32_t bfOffBits = 54;  // 14 for file header + 40 for info header
};

struct BMPInfoHeader {
    uint32_t biSize = 40;      // size of this header
    int32_t  biWidth;
    int32_t  biHeight;
    uint16_t biPlanes = 1;
    uint16_t biBitCount = 24;  // 24-bit bitmap
    uint32_t biCompression = 0;
    uint32_t biSizeImage = 0;  // can be 0 for BI_RGB
    int32_t  biXPelsPerMeter = 2835;
    int32_t  biYPelsPerMeter = 2835;
    uint32_t biClrUsed = 0;
    uint32_t biClrImportant = 0;
};

#pragma pack(pop)

void saveAsBMP(const std::vector<std::vector<COLORREF>>& pixels, const std::string& filename) {
    int height = pixels.size();
    if (height == 0) return;

    int width = pixels[0].size();
    int rowPadding = (4 - (width * 3) % 4) % 4; // BMP rows must be padded to 4-byte boundaries

    BMPFileHeader fileHeader;
    BMPInfoHeader infoHeader;

    infoHeader.biWidth = width;
    infoHeader.biHeight = height;
    infoHeader.biSizeImage = (3 * width + rowPadding) * height;
    fileHeader.bfSize = fileHeader.bfOffBits + infoHeader.biSizeImage;

    std::ofstream outFile(filename, std::ios::binary);
    if (!outFile) {
        std::cerr << "Error: Could not open file for writing.\n";
        return;
    }

    outFile.write(reinterpret_cast<const char*>(&fileHeader), sizeof(fileHeader));
    outFile.write(reinterpret_cast<const char*>(&infoHeader), sizeof(infoHeader));

    // BMP stores pixels bottom-up
    for (int y = height - 1; y >= 0; --y) {
        for (int x = 0; x < width; ++x) {
            COLORREF color = pixels[y][x];
            unsigned char b = GetBValue(color);
            unsigned char g = GetGValue(color);
            unsigned char r = GetRValue(color);
            outFile.write(reinterpret_cast<const char*>(&b), 1);
            outFile.write(reinterpret_cast<const char*>(&g), 1);
            outFile.write(reinterpret_cast<const char*>(&r), 1);
        }
        // Write padding
        uint8_t padding[3] = {0, 0, 0};
        outFile.write(reinterpret_cast<const char*>(padding), rowPadding);
    }

    outFile.close();
}


std::vector<std::vector<COLORREF>> GetPixels(HWND hwnd, int capture_width, int capture_height, int startY) {
    std::vector<std::vector<COLORREF>> pixels_to_save;

    // Get the device context of the window
    HDC hdcWindow = GetDC(hwnd);
    if (!hdcWindow) {
        std::cerr << "Error: Failed to get window DC.\n";
        return pixels_to_save;
    }

    // Create a compatible DC and a compatible bitmap
    HDC hdcMem = CreateCompatibleDC(hdcWindow);
    if (!hdcMem) {
        std::cerr << "Error: Failed to create compatible DC.\n";
        ReleaseDC(hwnd, hdcWindow);
        return pixels_to_save;
    }

    HBITMAP hBitmap = CreateCompatibleBitmap(hdcWindow, capture_width, capture_height);
    if (!hBitmap) {
        std::cerr << "Error: Failed to create compatible bitmap.\n";
        DeleteDC(hdcMem);
        ReleaseDC(hwnd, hdcWindow);
        return pixels_to_save;
    }

    // Select the bitmap into the compatible DC
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hdcMem, hBitmap);

    // Copy the content of the window's client area to the compatible DC
    BitBlt(hdcMem, 0, 0, capture_width, capture_height, hdcWindow, 0, 0, SRCCOPY);

    // Now, get pixel data from the compatible bitmap
    pixels_to_save.resize(capture_height - startY);
    for (int y = startY; y < capture_height; ++y) {
        std::vector<COLORREF> row;
        row.reserve(capture_width);
        for (int x = 0; x < capture_width; ++x) {
            row.push_back(GetPixel(hdcMem, x, y));
        }
        pixels_to_save[y - startY] = row;
    }

    // Clean up
    SelectObject(hdcMem, hOldBitmap);
    DeleteObject(hBitmap);
    DeleteDC(hdcMem);
    ReleaseDC(hwnd, hdcWindow);

    return pixels_to_save;
}