#ifndef BITMAPLOADER_H
#define BITMAPLOADER_H


#include <Windows.h>
#include <map>
#include <string>

class BitmapLoader
{
public:
    BitmapLoader();
    ~BitmapLoader();

    void loadImages();
    HBITMAP getImage(const std::wstring& imageName) const;
    HBITMAP getBitmapForValue(int value) const;

private:
    std::map<std::wstring, HBITMAP> images;
    HBITMAP loadImage(const wchar_t* imagePath);
};

#endif // BITMAPLOADER_H
