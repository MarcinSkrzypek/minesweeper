#include "BitmapLoader.h"
#include <map>
#include <string>

BitmapLoader::BitmapLoader() {
}

BitmapLoader::~BitmapLoader() {
    for (auto& pair : images) {
        if (pair.second) {
            DeleteObject(pair.second);
        }
    }
}

void BitmapLoader::loadImages() {
    images[L"Zero"] = loadImage(L"icons/Zero.bmp");
    images[L"One"] = loadImage(L"icons/One.bmp");
    images[L"Two"] = loadImage(L"icons/Two.bmp");
    images[L"Three"] = loadImage(L"icons/Three.bmp");
    images[L"Four"] = loadImage(L"icons/Four.bmp");
    images[L"Five"] = loadImage(L"icons/Five.bmp");
    images[L"Six"] = loadImage(L"icons/Six.bmp");
    images[L"Seven"] = loadImage(L"icons/Seven.bmp");
    images[L"Eight"] = loadImage(L"icons/Eight.bmp");
    images[L"Mines"] = loadImage(L"icons/Mines.bmp");
    images[L"MinesBorderless"] = loadImage(L"icons/MinesBorderless.bmp");
    images[L"MinesGuess"] = loadImage(L"icons/MinesGuess.bmp");
    images[L"QuestionMark"] = loadImage(L"icons/QuestionMark.bmp");
}

HBITMAP BitmapLoader::loadImage(const wchar_t* imagePath) {
    HBITMAP image = (HBITMAP)LoadImageW(NULL, imagePath, IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
    if (!image) {
        MessageBoxW(NULL, L"Failed to load image.", L"Error", MB_OK | MB_ICONERROR);
    }
    return image;
}

HBITMAP BitmapLoader::getImage(const std::wstring& imageName) const {
    auto it = images.find(imageName);
    if (it != images.end()) {
        return it->second;
    }
    return nullptr;
}

HBITMAP BitmapLoader::getBitmapForValue(int value) const {
    switch (value) {
    case 0:
        return getImage(L"Zero");
    case 1:
        return getImage(L"One");
    case 2:
        return getImage(L"Two");
    case 3:
        return getImage(L"Three");
    case 4:
        return getImage(L"Four");
    case 5:
        return getImage(L"Five");
    case 6:
        return getImage(L"Six");
    case 7:
        return getImage(L"Seven");
    case 8:
        return getImage(L"Eight");
    case 9:
        return getImage(L"Mines");
    default:
        return nullptr;
    }
}
