#ifndef DEF_FONT
#define DEF_FONT

#include <string>
#include <SDL2/SDL_ttf.h>
#include <map>

//Enum to declare all fonts that can be used
enum Fonts
{
    ROBOTOFIRST = 8,
    ROBOTO8 = 8,
	ROBOTO9 = 9,
	ROBOTO10 = 10,
	ROBOTO11 = 11,
	ROBOTO12 = 12,
	ROBOTO13 = 13,
	ROBOTO14 = 14,
	ROBOTO15 = 15,
	ROBOTO16 = 16,
	ROBOTO17 = 17,
	ROBOTO18 = 18,
	ROBOTO19 = 19,
	ROBOTO20 = 20,
	ROBOTO21 = 21,
	ROBOTO22 = 22,
	ROBOTO23 = 23,
	ROBOTO24 = 24,
	ROBOTO25 = 25,
	ROBOTO26 = 26,
	ROBOTO27 = 27,
	ROBOTO28 = 28,
	ROBOTO29 = 29,
	ROBOTO30 = 30,
	ROBOTO31 = 31,
	ROBOTO32 = 32,
	ROBOTO33 = 33,
	ROBOTO34 = 34,
	ROBOTO35 = 35,
	ROBOTO36 = 36,
	ROBOTO37 = 37,
	ROBOTO38 = 38,
	ROBOTO39 = 39,
	ROBOTO40 = 40,
	ROBOTO41 = 41,
	ROBOTO42 = 42,
	ROBOTO43 = 43,
	ROBOTO44 = 44,
	ROBOTO45 = 45,
	ROBOTO46 = 46,
	ROBOTO47 = 47,
	ROBOTO48 = 48,
    ROBOTOLAST = 48,

    AQUINOFIRST = 8,
    AQUINO8 = 8,
	AQUINO9 = 9,
	AQUINO10 = 10,
	AQUINO11 = 11,
	AQUINO12 = 12,
	AQUINO13 = 13,
	AQUINO14 = 14,
	AQUINO15 = 15,
	AQUINO16 = 16,
	AQUINO17 = 17,
	AQUINO18 = 18,
	AQUINO19 = 19,
	AQUINO20 = 20,
	AQUINO21 = 21,
	AQUINO22 = 22,
	AQUINO23 = 23,
	AQUINO24 = 24,
	AQUINO25 = 25,
	AQUINO26 = 26,
	AQUINO27 = 27,
	AQUINO28 = 28,
	AQUINO29 = 29,
	AQUINO30 = 30,
	AQUINO31 = 31,
	AQUINO32 = 32,
	AQUINO33 = 33,
	AQUINO34 = 34,
	AQUINO35 = 35,
	AQUINO36 = 36,
	AQUINO37 = 37,
	AQUINO38 = 38,
	AQUINO39 = 39,
	AQUINO40 = 40,
	AQUINO41 = 41,
	AQUINO42 = 42,
	AQUINO43 = 43,
	AQUINO44 = 44,
	AQUINO45 = 45,
	AQUINO46 = 46,
	AQUINO47 = 47,
	AQUINO48 = 48,
    AQUINOLAST = 48
};

enum FontFamily
{
    ROBOTO,
    AQUINO
};

class Font
{
public:
    Font(std::string path, int size);
    ~Font();

    std::string getPath();
    Font* setPath(std::string path);

    int getSize();
    Font* setSize(int size);
    int getMaxHeight();
    static int getMaxHeight(Fonts font);
    static int getMaxHeight(Font* font);

    static int getLineSkip(Font* font);

    TTF_Font* getFont();
    Font* setFont(TTF_Font* font);

    static std::map<Fonts, Font*> fontMap;
    static std::map<int, Font*> robotoFontMap;
    static std::map<int, Font*> aquinoFontMap;
    static std::map<FontFamily, std::map<int, Font*>> fontMMap;

    static Font* getFontFromMap(Fonts font); //depreciated
    static Font* getFontFromMap(FontFamily, int size);

    static std::map<int, Fonts> robotoSizes;
    static std::map<Fonts, int> robotoSizesReverser;
    static std::map<int, Fonts> aquinoSizes;
    static std::map<Fonts, int> aquinoSizesReverser;

    static Fonts getFontEnum(FontFamily fontFamily, int size);

    static int getBiggestFontToFit(FontFamily fontFamily, std::string text, int width, int height);

    static int getBiggestFontForHeight(FontFamily fontFamily, std::string text, int height);

    static Fonts getSmallerFont(FontFamily fontFamily, Fonts curentFont);

    static int getSmallerFont(FontFamily fontFamily, int fontSize);

protected:
    std::string m_path;
    int m_size;
    TTF_Font* m_font = nullptr;
};

#endif
