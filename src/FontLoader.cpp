#include "FontLoader.h"

FontLoader::FontLoader() {
    //loadFonts();
    //createFonts();
}

FontLoader::~FontLoader() {
    for (const auto& font : fonts) {
        DeleteObject(font.second);
    }
    RemoveFontResourceEx("resources/fonts/Digital-7 400.ttf", FR_PRIVATE, NULL);
}

void FontLoader::loadFonts() {
    if(AddFontResourceEx("resources/fonts/Digital-7 400.ttf", FR_PRIVATE, NULL) == 0) {
        MessageBoxW(NULL, L"Failed to load font Digital-7.", L"Error", MB_OK | MB_ICONERROR);
    }
}

void FontLoader::createFonts() {
    createFont(30, L"Digital-7");
}

HFONT FontLoader::getFont(const std::wstring& fontName) {
    auto it = fonts.find(fontName);
    if (it != fonts.end()) {
        return it->second;
    }
    return nullptr;
}

void FontLoader::createFont(int height, const std::wstring& fontName) {
    HFONT font =  CreateFontW(height, 0, 0, 0,
                              FW_NORMAL, FALSE, FALSE, FALSE,
                              DEFAULT_CHARSET, OUT_DEFAULT_PRECIS,
                              CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY,
                              FIXED_PITCH | FF_SWISS, fontName.c_str());
    if (!font) {
        std::wstring errorMessage = L"Failed to create font " + fontName + L".";
        MessageBoxW(NULL, errorMessage.c_str(), L"Error", MB_OK | MB_ICONERROR);
    }
    fonts[fontName] = font;
}
