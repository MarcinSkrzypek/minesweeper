#ifndef FONTLOADER_H
#define FONTLOADER_H

#include <windows.h>
#include <map>
#include <string>

class FontLoader {
  public:
    FontLoader();
    virtual ~FontLoader();

    void loadFonts();
    void createFonts();
    void createFont(int height, const std::wstring& fontName);
    HFONT getFont(const std::wstring& fontName);

  private:
    std::map<std::wstring, HFONT> fonts;
};

#endif // FONTLOADER_H
