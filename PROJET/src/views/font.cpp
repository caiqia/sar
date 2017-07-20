#include "font.hpp"
#include "text.hpp"
#include "../entities/log.hpp"
#include "../entities/error.hpp"

#include <iostream>

using namespace std;

//Object used to represent fonts

//Init function for our static font map
//We create a static map that links our Font enum to Font objects
//The objects are initialized here
map<Fonts, Font*> create_map()
{
    TTF_Init();
    map<Fonts, Font*> m;
    m[Fonts::ROBOTO8] = new Font("/nostal/fonts/Roboto-Regular.ttf", 8);
    m[Fonts::ROBOTO9] = new Font("/nostal/fonts/Roboto-Regular.ttf", 9);
    m[Fonts::ROBOTO10] = new Font("/nostal/fonts/Roboto-Regular.ttf", 10);
    m[Fonts::ROBOTO11] = new Font("/nostal/fonts/Roboto-Regular.ttf", 11);
    m[Fonts::ROBOTO12] = new Font("/nostal/fonts/Roboto-Regular.ttf", 12);
    m[Fonts::ROBOTO13] = new Font("/nostal/fonts/Roboto-Regular.ttf", 13);
    m[Fonts::ROBOTO14] = new Font("/nostal/fonts/Roboto-Regular.ttf", 14);
    m[Fonts::ROBOTO15] = new Font("/nostal/fonts/Roboto-Regular.ttf", 15);
    m[Fonts::ROBOTO16] = new Font("/nostal/fonts/Roboto-Regular.ttf", 16);
    m[Fonts::ROBOTO17] = new Font("/nostal/fonts/Roboto-Regular.ttf", 17);
    m[Fonts::ROBOTO18] = new Font("/nostal/fonts/Roboto-Regular.ttf", 18);
    m[Fonts::ROBOTO19] = new Font("/nostal/fonts/Roboto-Regular.ttf", 19);
    m[Fonts::ROBOTO20] = new Font("/nostal/fonts/Roboto-Regular.ttf", 20);
    m[Fonts::ROBOTO21] = new Font("/nostal/fonts/Roboto-Regular.ttf", 21);
    m[Fonts::ROBOTO22] = new Font("/nostal/fonts/Roboto-Regular.ttf", 22);
    m[Fonts::ROBOTO23] = new Font("/nostal/fonts/Roboto-Regular.ttf", 23);
    m[Fonts::ROBOTO24] = new Font("/nostal/fonts/Roboto-Regular.ttf", 24);
    m[Fonts::ROBOTO25] = new Font("/nostal/fonts/Roboto-Regular.ttf", 25);
    m[Fonts::ROBOTO26] = new Font("/nostal/fonts/Roboto-Regular.ttf", 26);
    m[Fonts::ROBOTO27] = new Font("/nostal/fonts/Roboto-Regular.ttf", 27);
    m[Fonts::ROBOTO28] = new Font("/nostal/fonts/Roboto-Regular.ttf", 28);
    m[Fonts::ROBOTO29] = new Font("/nostal/fonts/Roboto-Regular.ttf", 29);
    m[Fonts::ROBOTO30] = new Font("/nostal/fonts/Roboto-Regular.ttf", 30);
    m[Fonts::ROBOTO31] = new Font("/nostal/fonts/Roboto-Regular.ttf", 31);
    m[Fonts::ROBOTO32] = new Font("/nostal/fonts/Roboto-Regular.ttf", 32);
    m[Fonts::ROBOTO33] = new Font("/nostal/fonts/Roboto-Regular.ttf", 33);
    m[Fonts::ROBOTO34] = new Font("/nostal/fonts/Roboto-Regular.ttf", 34);
    m[Fonts::ROBOTO35] = new Font("/nostal/fonts/Roboto-Regular.ttf", 35);
    m[Fonts::ROBOTO36] = new Font("/nostal/fonts/Roboto-Regular.ttf", 36);
    m[Fonts::ROBOTO37] = new Font("/nostal/fonts/Roboto-Regular.ttf", 37);
    m[Fonts::ROBOTO38] = new Font("/nostal/fonts/Roboto-Regular.ttf", 38);
    m[Fonts::ROBOTO39] = new Font("/nostal/fonts/Roboto-Regular.ttf", 39);
    m[Fonts::ROBOTO40] = new Font("/nostal/fonts/Roboto-Regular.ttf", 40);
    m[Fonts::ROBOTO41] = new Font("/nostal/fonts/Roboto-Regular.ttf", 41);
    m[Fonts::ROBOTO42] = new Font("/nostal/fonts/Roboto-Regular.ttf", 42);
    m[Fonts::ROBOTO43] = new Font("/nostal/fonts/Roboto-Regular.ttf", 43);
    m[Fonts::ROBOTO44] = new Font("/nostal/fonts/Roboto-Regular.ttf", 44);
    m[Fonts::ROBOTO45] = new Font("/nostal/fonts/Roboto-Regular.ttf", 45);
    m[Fonts::ROBOTO46] = new Font("/nostal/fonts/Roboto-Regular.ttf", 46);
    m[Fonts::ROBOTO47] = new Font("/nostal/fonts/Roboto-Regular.ttf", 47);
    m[Fonts::ROBOTO48] = new Font("/nostal/fonts/Roboto-Regular.ttf", 48);
    m[Fonts::AQUINO8] = new Font("/nostal/fonts/aquino.ttf", 8);
    m[Fonts::AQUINO9] = new Font("/nostal/fonts/aquino.ttf", 9);
    m[Fonts::AQUINO10] = new Font("/nostal/fonts/aquino.ttf", 10);
    m[Fonts::AQUINO11] = new Font("/nostal/fonts/aquino.ttf", 11);
    m[Fonts::AQUINO12] = new Font("/nostal/fonts/aquino.ttf", 12);
    m[Fonts::AQUINO13] = new Font("/nostal/fonts/aquino.ttf", 13);
    m[Fonts::AQUINO14] = new Font("/nostal/fonts/aquino.ttf", 14);
    m[Fonts::AQUINO15] = new Font("/nostal/fonts/aquino.ttf", 15);
    m[Fonts::AQUINO16] = new Font("/nostal/fonts/aquino.ttf", 16);
    m[Fonts::AQUINO17] = new Font("/nostal/fonts/aquino.ttf", 17);
    m[Fonts::AQUINO18] = new Font("/nostal/fonts/aquino.ttf", 18);
    m[Fonts::AQUINO19] = new Font("/nostal/fonts/aquino.ttf", 19);
    m[Fonts::AQUINO20] = new Font("/nostal/fonts/aquino.ttf", 20);
    m[Fonts::AQUINO21] = new Font("/nostal/fonts/aquino.ttf", 21);
    m[Fonts::AQUINO22] = new Font("/nostal/fonts/aquino.ttf", 22);
    m[Fonts::AQUINO23] = new Font("/nostal/fonts/aquino.ttf", 23);
    m[Fonts::AQUINO24] = new Font("/nostal/fonts/aquino.ttf", 24);
    m[Fonts::AQUINO25] = new Font("/nostal/fonts/aquino.ttf", 25);
    m[Fonts::AQUINO26] = new Font("/nostal/fonts/aquino.ttf", 26);
    m[Fonts::AQUINO27] = new Font("/nostal/fonts/aquino.ttf", 27);
    m[Fonts::AQUINO28] = new Font("/nostal/fonts/aquino.ttf", 28);
    m[Fonts::AQUINO29] = new Font("/nostal/fonts/aquino.ttf", 29);
    m[Fonts::AQUINO30] = new Font("/nostal/fonts/aquino.ttf", 30);
    m[Fonts::AQUINO31] = new Font("/nostal/fonts/aquino.ttf", 31);
    m[Fonts::AQUINO32] = new Font("/nostal/fonts/aquino.ttf", 32);
    m[Fonts::AQUINO33] = new Font("/nostal/fonts/aquino.ttf", 33);
    m[Fonts::AQUINO34] = new Font("/nostal/fonts/aquino.ttf", 34);
    m[Fonts::AQUINO35] = new Font("/nostal/fonts/aquino.ttf", 35);
    m[Fonts::AQUINO36] = new Font("/nostal/fonts/aquino.ttf", 36);
    m[Fonts::AQUINO37] = new Font("/nostal/fonts/aquino.ttf", 37);
    m[Fonts::AQUINO38] = new Font("/nostal/fonts/aquino.ttf", 38);
    m[Fonts::AQUINO39] = new Font("/nostal/fonts/aquino.ttf", 39);
    m[Fonts::AQUINO40] = new Font("/nostal/fonts/aquino.ttf", 40);
    m[Fonts::AQUINO41] = new Font("/nostal/fonts/aquino.ttf", 41);
    m[Fonts::AQUINO42] = new Font("/nostal/fonts/aquino.ttf", 42);
    m[Fonts::AQUINO43] = new Font("/nostal/fonts/aquino.ttf", 43);
    m[Fonts::AQUINO44] = new Font("/nostal/fonts/aquino.ttf", 44);
    m[Fonts::AQUINO45] = new Font("/nostal/fonts/aquino.ttf", 45);
    m[Fonts::AQUINO46] = new Font("/nostal/fonts/aquino.ttf", 46);
    m[Fonts::AQUINO47] = new Font("/nostal/fonts/aquino.ttf", 47);
    m[Fonts::AQUINO48] = new Font("/nostal/fonts/aquino.ttf", 48);
    return m;
}

map<FontFamily, map<int, Font*>> createFontMap()
{
    TTF_Init();
    map<FontFamily, map<int, Font*>> fontMap;
    map<int, Font*> m;
    m[8] = new Font("/nostal/fonts/Roboto-Regular.ttf", 8);
    m[9] = new Font("/nostal/fonts/Roboto-Regular.ttf", 9);
    m[10] = new Font("/nostal/fonts/Roboto-Regular.ttf", 10);
    m[11] = new Font("/nostal/fonts/Roboto-Regular.ttf", 11);
    m[12] = new Font("/nostal/fonts/Roboto-Regular.ttf", 12);
    m[13] = new Font("/nostal/fonts/Roboto-Regular.ttf", 13);
    m[14] = new Font("/nostal/fonts/Roboto-Regular.ttf", 14);
    m[15] = new Font("/nostal/fonts/Roboto-Regular.ttf", 15);
    m[16] = new Font("/nostal/fonts/Roboto-Regular.ttf", 16);
    m[17] = new Font("/nostal/fonts/Roboto-Regular.ttf", 17);
    m[18] = new Font("/nostal/fonts/Roboto-Regular.ttf", 18);
    m[19] = new Font("/nostal/fonts/Roboto-Regular.ttf", 19);
    m[20] = new Font("/nostal/fonts/Roboto-Regular.ttf", 20);
    m[21] = new Font("/nostal/fonts/Roboto-Regular.ttf", 21);
    m[22] = new Font("/nostal/fonts/Roboto-Regular.ttf", 22);
    m[23] = new Font("/nostal/fonts/Roboto-Regular.ttf", 23);
    m[24] = new Font("/nostal/fonts/Roboto-Regular.ttf", 24);
    m[25] = new Font("/nostal/fonts/Roboto-Regular.ttf", 25);
    m[26] = new Font("/nostal/fonts/Roboto-Regular.ttf", 26);
    m[27] = new Font("/nostal/fonts/Roboto-Regular.ttf", 27);
    m[28] = new Font("/nostal/fonts/Roboto-Regular.ttf", 28);
    m[29] = new Font("/nostal/fonts/Roboto-Regular.ttf", 29);
    m[30] = new Font("/nostal/fonts/Roboto-Regular.ttf", 30);
    m[31] = new Font("/nostal/fonts/Roboto-Regular.ttf", 31);
    m[32] = new Font("/nostal/fonts/Roboto-Regular.ttf", 32);
    m[33] = new Font("/nostal/fonts/Roboto-Regular.ttf", 33);
    m[34] = new Font("/nostal/fonts/Roboto-Regular.ttf", 34);
    m[35] = new Font("/nostal/fonts/Roboto-Regular.ttf", 35);
    m[36] = new Font("/nostal/fonts/Roboto-Regular.ttf", 36);
    m[37] = new Font("/nostal/fonts/Roboto-Regular.ttf", 37);
    m[38] = new Font("/nostal/fonts/Roboto-Regular.ttf", 38);
    m[39] = new Font("/nostal/fonts/Roboto-Regular.ttf", 39);
    m[40] = new Font("/nostal/fonts/Roboto-Regular.ttf", 40);
    m[41] = new Font("/nostal/fonts/Roboto-Regular.ttf", 41);
    m[42] = new Font("/nostal/fonts/Roboto-Regular.ttf", 42);
    m[43] = new Font("/nostal/fonts/Roboto-Regular.ttf", 43);
    m[44] = new Font("/nostal/fonts/Roboto-Regular.ttf", 44);
    m[45] = new Font("/nostal/fonts/Roboto-Regular.ttf", 45);
    m[46] = new Font("/nostal/fonts/Roboto-Regular.ttf", 46);
    m[47] = new Font("/nostal/fonts/Roboto-Regular.ttf", 47);
    m[48] = new Font("/nostal/fonts/Roboto-Regular.ttf", 48);
    fontMap.insert(pair<FontFamily, map<int, Font*>> (FontFamily::ROBOTO, m));
    m.clear();
    m[8] = new Font("/nostal/fonts/aquino.ttf", 8);
    m[9] = new Font("/nostal/fonts/aquino.ttf", 9);
    m[10] = new Font("/nostal/fonts/aquino.ttf", 10);
    m[11] = new Font("/nostal/fonts/aquino.ttf", 11);
    m[12] = new Font("/nostal/fonts/aquino.ttf", 12);
    m[13] = new Font("/nostal/fonts/aquino.ttf", 13);
    m[14] = new Font("/nostal/fonts/aquino.ttf", 14);
    m[15] = new Font("/nostal/fonts/aquino.ttf", 15);
    m[16] = new Font("/nostal/fonts/aquino.ttf", 16);
    m[17] = new Font("/nostal/fonts/aquino.ttf", 17);
    m[18] = new Font("/nostal/fonts/aquino.ttf", 18);
    m[19] = new Font("/nostal/fonts/aquino.ttf", 19);
    m[20] = new Font("/nostal/fonts/aquino.ttf", 20);
    m[21] = new Font("/nostal/fonts/aquino.ttf", 21);
    m[22] = new Font("/nostal/fonts/aquino.ttf", 22);
    m[23] = new Font("/nostal/fonts/aquino.ttf", 23);
    m[24] = new Font("/nostal/fonts/aquino.ttf", 24);
    m[25] = new Font("/nostal/fonts/aquino.ttf", 25);
    m[26] = new Font("/nostal/fonts/aquino.ttf", 26);
    m[27] = new Font("/nostal/fonts/aquino.ttf", 27);
    m[28] = new Font("/nostal/fonts/aquino.ttf", 28);
    m[29] = new Font("/nostal/fonts/aquino.ttf", 29);
    m[30] = new Font("/nostal/fonts/aquino.ttf", 30);
    m[31] = new Font("/nostal/fonts/aquino.ttf", 31);
    m[32] = new Font("/nostal/fonts/aquino.ttf", 32);
    m[33] = new Font("/nostal/fonts/aquino.ttf", 33);
    m[34] = new Font("/nostal/fonts/aquino.ttf", 34);
    m[35] = new Font("/nostal/fonts/aquino.ttf", 35);
    m[36] = new Font("/nostal/fonts/aquino.ttf", 36);
    m[37] = new Font("/nostal/fonts/aquino.ttf", 37);
    m[38] = new Font("/nostal/fonts/aquino.ttf", 38);
    m[39] = new Font("/nostal/fonts/aquino.ttf", 39);
    m[40] = new Font("/nostal/fonts/aquino.ttf", 40);
    m[41] = new Font("/nostal/fonts/aquino.ttf", 41);
    m[42] = new Font("/nostal/fonts/aquino.ttf", 42);
    m[43] = new Font("/nostal/fonts/aquino.ttf", 43);
    m[44] = new Font("/nostal/fonts/aquino.ttf", 44);
    m[45] = new Font("/nostal/fonts/aquino.ttf", 45);
    m[46] = new Font("/nostal/fonts/aquino.ttf", 46);
    m[47] = new Font("/nostal/fonts/aquino.ttf", 47);
    m[48] = new Font("/nostal/fonts/aquino.ttf", 48);
    fontMap.insert(pair<FontFamily, map<int, Font*>> (FontFamily::AQUINO, m));
    m.clear();
    return fontMap;
}

map<int, Font*> createRobotoMap()
{
    TTF_Init();
    map<int, Font*> m;
    m[8] = new Font("/nostal/fonts/Roboto-Regular.ttf", 8);
    m[9] = new Font("/nostal/fonts/Roboto-Regular.ttf", 9);
    m[10] = new Font("/nostal/fonts/Roboto-Regular.ttf", 10);
    m[11] = new Font("/nostal/fonts/Roboto-Regular.ttf", 11);
    m[12] = new Font("/nostal/fonts/Roboto-Regular.ttf", 12);
    m[13] = new Font("/nostal/fonts/Roboto-Regular.ttf", 13);
    m[14] = new Font("/nostal/fonts/Roboto-Regular.ttf", 14);
    m[15] = new Font("/nostal/fonts/Roboto-Regular.ttf", 15);
    m[16] = new Font("/nostal/fonts/Roboto-Regular.ttf", 16);
    m[17] = new Font("/nostal/fonts/Roboto-Regular.ttf", 17);
    m[18] = new Font("/nostal/fonts/Roboto-Regular.ttf", 18);
    m[19] = new Font("/nostal/fonts/Roboto-Regular.ttf", 19);
    m[20] = new Font("/nostal/fonts/Roboto-Regular.ttf", 20);
    m[21] = new Font("/nostal/fonts/Roboto-Regular.ttf", 21);
    m[22] = new Font("/nostal/fonts/Roboto-Regular.ttf", 22);
    m[23] = new Font("/nostal/fonts/Roboto-Regular.ttf", 23);
    m[24] = new Font("/nostal/fonts/Roboto-Regular.ttf", 24);
    m[25] = new Font("/nostal/fonts/Roboto-Regular.ttf", 25);
    m[26] = new Font("/nostal/fonts/Roboto-Regular.ttf", 26);
    m[27] = new Font("/nostal/fonts/Roboto-Regular.ttf", 27);
    m[28] = new Font("/nostal/fonts/Roboto-Regular.ttf", 28);
    m[29] = new Font("/nostal/fonts/Roboto-Regular.ttf", 29);
    m[30] = new Font("/nostal/fonts/Roboto-Regular.ttf", 30);
    m[31] = new Font("/nostal/fonts/Roboto-Regular.ttf", 31);
    m[32] = new Font("/nostal/fonts/Roboto-Regular.ttf", 32);
    m[33] = new Font("/nostal/fonts/Roboto-Regular.ttf", 33);
    m[34] = new Font("/nostal/fonts/Roboto-Regular.ttf", 34);
    m[35] = new Font("/nostal/fonts/Roboto-Regular.ttf", 35);
    m[36] = new Font("/nostal/fonts/Roboto-Regular.ttf", 36);
    m[37] = new Font("/nostal/fonts/Roboto-Regular.ttf", 37);
    m[38] = new Font("/nostal/fonts/Roboto-Regular.ttf", 38);
    m[39] = new Font("/nostal/fonts/Roboto-Regular.ttf", 39);
    m[40] = new Font("/nostal/fonts/Roboto-Regular.ttf", 40);
    m[41] = new Font("/nostal/fonts/Roboto-Regular.ttf", 41);
    m[42] = new Font("/nostal/fonts/Roboto-Regular.ttf", 42);
    m[43] = new Font("/nostal/fonts/Roboto-Regular.ttf", 43);
    m[44] = new Font("/nostal/fonts/Roboto-Regular.ttf", 44);
    m[45] = new Font("/nostal/fonts/Roboto-Regular.ttf", 45);
    m[46] = new Font("/nostal/fonts/Roboto-Regular.ttf", 46);
    m[47] = new Font("/nostal/fonts/Roboto-Regular.ttf", 47);
    m[48] = new Font("/nostal/fonts/Roboto-Regular.ttf", 48);
    return m;
}

map<int, Font*> createAquinoMap()
{
    TTF_Init();
    map<int, Font*> m;
    m[8] = new Font("/nostal/fonts/Roboto-Regular.ttf", 8);
    m[9] = new Font("/nostal/fonts/Roboto-Regular.ttf", 9);
    m[10] = new Font("/nostal/fonts/Roboto-Regular.ttf", 10);
    m[11] = new Font("/nostal/fonts/Roboto-Regular.ttf", 11);
    m[12] = new Font("/nostal/fonts/Roboto-Regular.ttf", 12);
    m[13] = new Font("/nostal/fonts/Roboto-Regular.ttf", 13);
    m[14] = new Font("/nostal/fonts/Roboto-Regular.ttf", 14);
    m[15] = new Font("/nostal/fonts/Roboto-Regular.ttf", 15);
    m[16] = new Font("/nostal/fonts/Roboto-Regular.ttf", 16);
    m[17] = new Font("/nostal/fonts/Roboto-Regular.ttf", 17);
    m[18] = new Font("/nostal/fonts/Roboto-Regular.ttf", 18);
    m[19] = new Font("/nostal/fonts/Roboto-Regular.ttf", 19);
    m[20] = new Font("/nostal/fonts/Roboto-Regular.ttf", 20);
    m[21] = new Font("/nostal/fonts/Roboto-Regular.ttf", 21);
    m[22] = new Font("/nostal/fonts/Roboto-Regular.ttf", 22);
    m[23] = new Font("/nostal/fonts/Roboto-Regular.ttf", 23);
    m[24] = new Font("/nostal/fonts/Roboto-Regular.ttf", 24);
    m[25] = new Font("/nostal/fonts/Roboto-Regular.ttf", 25);
    m[26] = new Font("/nostal/fonts/Roboto-Regular.ttf", 26);
    m[27] = new Font("/nostal/fonts/Roboto-Regular.ttf", 27);
    m[28] = new Font("/nostal/fonts/Roboto-Regular.ttf", 28);
    m[29] = new Font("/nostal/fonts/Roboto-Regular.ttf", 29);
    m[30] = new Font("/nostal/fonts/Roboto-Regular.ttf", 30);
    m[31] = new Font("/nostal/fonts/Roboto-Regular.ttf", 31);
    m[32] = new Font("/nostal/fonts/Roboto-Regular.ttf", 32);
    m[33] = new Font("/nostal/fonts/Roboto-Regular.ttf", 33);
    m[34] = new Font("/nostal/fonts/Roboto-Regular.ttf", 34);
    m[35] = new Font("/nostal/fonts/Roboto-Regular.ttf", 35);
    m[36] = new Font("/nostal/fonts/Roboto-Regular.ttf", 36);
    m[37] = new Font("/nostal/fonts/Roboto-Regular.ttf", 37);
    m[38] = new Font("/nostal/fonts/Roboto-Regular.ttf", 38);
    m[39] = new Font("/nostal/fonts/Roboto-Regular.ttf", 39);
    m[40] = new Font("/nostal/fonts/Roboto-Regular.ttf", 40);
    m[41] = new Font("/nostal/fonts/Roboto-Regular.ttf", 41);
    m[42] = new Font("/nostal/fonts/Roboto-Regular.ttf", 42);
    m[43] = new Font("/nostal/fonts/Roboto-Regular.ttf", 43);
    m[44] = new Font("/nostal/fonts/Roboto-Regular.ttf", 44);
    m[45] = new Font("/nostal/fonts/Roboto-Regular.ttf", 45);
    m[46] = new Font("/nostal/fonts/Roboto-Regular.ttf", 46);
    m[47] = new Font("/nostal/fonts/Roboto-Regular.ttf", 47);
    m[48] = new Font("/nostal/fonts/Roboto-Regular.ttf", 48);
    return m;
}

//declaration of the font map with a call to the previous function
map<Fonts, Font*> Font::fontMap = create_map();

map<int, Font*> Font::robotoFontMap = createRobotoMap();

map<int, Font*> Font::aquinoFontMap = createAquinoMap();

map<FontFamily, map<int, Font*>> Font::fontMMap = createFontMap();

map<int, Fonts> Font::robotoSizes = {{8, Fonts::ROBOTO8}, {9, Fonts::ROBOTO9}, {10, Fonts::ROBOTO10}, {11, Fonts::ROBOTO11}, {12, Fonts::ROBOTO12}, {13, Fonts::ROBOTO13}, {14, Fonts::ROBOTO14}, {15, Fonts::ROBOTO15}, {16, Fonts::ROBOTO16}, {17, Fonts::ROBOTO17}, {18, Fonts::ROBOTO18}, {19, Fonts::ROBOTO19}, {20, Fonts::ROBOTO20}, {21, Fonts::ROBOTO21}, {22, Fonts::ROBOTO22}, {23, Fonts::ROBOTO23}, {24, Fonts::ROBOTO24}, {25, Fonts::ROBOTO25}, {26, Fonts::ROBOTO26}, {27, Fonts::ROBOTO27}, {28, Fonts::ROBOTO28}, {29, Fonts::ROBOTO29}, {30, Fonts::ROBOTO30}, {31, Fonts::ROBOTO31}, {32, Fonts::ROBOTO32}, {33, Fonts::ROBOTO33}, {34, Fonts::ROBOTO34}, {35, Fonts::ROBOTO35}, {36, Fonts::ROBOTO36}, {37, Fonts::ROBOTO37}, {38, Fonts::ROBOTO38}, {39, Fonts::ROBOTO39}, {40, Fonts::ROBOTO40}, {41, Fonts::ROBOTO41}, {42, Fonts::ROBOTO42}, {43, Fonts::ROBOTO43}, {44, Fonts::ROBOTO44}, {45, Fonts::ROBOTO45}, {46, Fonts::ROBOTO46}, {47, Fonts::ROBOTO47}, {48, Fonts::ROBOTO48}};
map<Fonts, int> Font::robotoSizesReverser = {{Fonts::ROBOTO8, 8}, {Fonts::ROBOTO9, 9}, {Fonts::ROBOTO10, 10}, {Fonts::ROBOTO11, 11}, {Fonts::ROBOTO12, 12}, {Fonts::ROBOTO13, 13}, {Fonts::ROBOTO14, 14}, {Fonts::ROBOTO15, 15}, {Fonts::ROBOTO16, 16}, {Fonts::ROBOTO17, 17}, {Fonts::ROBOTO18, 18}, {Fonts::ROBOTO19, 19}, {Fonts::ROBOTO20, 20}, {Fonts::ROBOTO21, 21}, {Fonts::ROBOTO22, 22}, {Fonts::ROBOTO23, 23}, {Fonts::ROBOTO24, 24}, {Fonts::ROBOTO25, 25}, {Fonts::ROBOTO26, 26}, {Fonts::ROBOTO27, 27}, {Fonts::ROBOTO28, 28}, {Fonts::ROBOTO29, 29}, {Fonts::ROBOTO30, 30}, {Fonts::ROBOTO31, 31}, {Fonts::ROBOTO32, 32}, {Fonts::ROBOTO33, 33}, {Fonts::ROBOTO34, 34}, {Fonts::ROBOTO35, 35}, {Fonts::ROBOTO36, 36}, {Fonts::ROBOTO37, 37}, {Fonts::ROBOTO38, 38}, {Fonts::ROBOTO39, 39}, {Fonts::ROBOTO40, 40}, {Fonts::ROBOTO41, 41}, {Fonts::ROBOTO42, 42}, {Fonts::ROBOTO43, 43}, {Fonts::ROBOTO44, 44}, {Fonts::ROBOTO45, 45}, {Fonts::ROBOTO46, 46}, {Fonts::ROBOTO47, 47}, {Fonts::ROBOTO48, 48}};

map<int, Fonts> Font::aquinoSizes = {{8, Fonts::AQUINO8}, {9, Fonts::AQUINO9}, {10, Fonts::AQUINO10}, {11, Fonts::AQUINO11}, {12, Fonts::AQUINO12}, {13, Fonts::AQUINO13}, {14, Fonts::AQUINO14}, {15, Fonts::AQUINO15}, {16, Fonts::AQUINO16}, {17, Fonts::AQUINO17}, {18, Fonts::AQUINO18}, {19, Fonts::AQUINO19}, {20, Fonts::AQUINO20}, {21, Fonts::AQUINO21}, {22, Fonts::AQUINO22}, {23, Fonts::AQUINO23}, {24, Fonts::AQUINO24}, {25, Fonts::AQUINO25}, {26, Fonts::AQUINO26}, {27, Fonts::AQUINO27}, {28, Fonts::AQUINO28}, {29, Fonts::AQUINO29}, {30, Fonts::AQUINO30}, {31, Fonts::AQUINO31}, {32, Fonts::AQUINO32}, {33, Fonts::AQUINO33}, {34, Fonts::AQUINO34}, {35, Fonts::AQUINO35}, {36, Fonts::AQUINO36}, {37, Fonts::AQUINO37}, {38, Fonts::AQUINO38}, {39, Fonts::AQUINO39}, {40, Fonts::AQUINO40}, {41, Fonts::AQUINO41}, {42, Fonts::AQUINO42}, {43, Fonts::AQUINO43}, {44, Fonts::AQUINO44}, {45, Fonts::AQUINO45}, {46, Fonts::AQUINO46}, {47, Fonts::AQUINO47}, {48, Fonts::AQUINO48}};
map<Fonts, int> Font::aquinoSizesReverser = {{Fonts::AQUINO8, 8}, {Fonts::AQUINO9, 9}, {Fonts::AQUINO10, 10}, {Fonts::AQUINO11, 11}, {Fonts::AQUINO12, 12}, {Fonts::AQUINO13, 13}, {Fonts::AQUINO14, 14}, {Fonts::AQUINO15, 15}, {Fonts::AQUINO16, 16}, {Fonts::AQUINO17, 17}, {Fonts::AQUINO18, 18}, {Fonts::AQUINO19, 19}, {Fonts::AQUINO20, 20}, {Fonts::AQUINO21, 21}, {Fonts::AQUINO22, 22}, {Fonts::AQUINO23, 23}, {Fonts::AQUINO24, 24}, {Fonts::AQUINO25, 25}, {Fonts::AQUINO26, 26}, {Fonts::AQUINO27, 27}, {Fonts::AQUINO28, 28}, {Fonts::AQUINO29, 29}, {Fonts::AQUINO30, 30}, {Fonts::AQUINO31, 31}, {Fonts::AQUINO32, 32}, {Fonts::AQUINO33, 33}, {Fonts::AQUINO34, 34}, {Fonts::AQUINO35, 35}, {Fonts::AQUINO36, 36}, {Fonts::AQUINO37, 37}, {Fonts::AQUINO38, 38}, {Fonts::AQUINO39, 39}, {Fonts::AQUINO40, 40}, {Fonts::AQUINO41, 41}, {Fonts::AQUINO42, 42}, {Fonts::AQUINO43, 43}, {Fonts::AQUINO44, 44}, {Fonts::AQUINO45, 45}, {Fonts::AQUINO46, 46}, {Fonts::AQUINO47, 47}, {Fonts::AQUINO48, 48}};

//Object init
Font::Font(string path, int size)
{
    this->setPath(path)->setSize(size);
    this->setFont(TTF_OpenFont(path.c_str(), size));

    //If we could not open font
    if(!this->getFont())
    {
        string m_error = TTF_GetError();

        Log::addEntry(1, "/!\\ Could not load font : " + path + "\n-> " + m_error);
        //TODO handle error to avoid loading a font that does not function
    }
    //Everything went fine
    else
    {
        Log::addEntry(3, "Font : " + path + " was loaded successfully");
    }
}

Font::~Font()
{
    if(this->getFont() != nullptr)
    {
        TTF_CloseFont(this->getFont());
    }
    Log::addEntry(3, "Font : " + this->getPath() + " was deleted successfully");
}

string Font::getPath()
{
    return this->m_path;
}

Font* Font::setPath(string path)
{
    this->m_path = path;
    return this;
}

int Font::getSize()
{
    return this->m_size;
}

Font* Font::setSize(int size)
{
    this->m_size = size;
    return this;
}

int Font::getMaxHeight()
{
    if(this->getFont() != NULL)
        return TTF_FontHeight(this->getFont());

    else
    {
        Log::addEntry(1, "/!\\ NULL font provided to getMaxHeight");
        return 0;
    }
}

int Font::getMaxHeight(Fonts font)
{
    if(Font::getFontFromMap(font) != NULL)
        return Font::getMaxHeight(Font::getFontFromMap(font));

    else
    {
        Log::addEntry(1, "/!\\ Got NULL font from map in getMaxHeight");
        return 0;
    }
}

int Font::getMaxHeight(Font* font)
{
    if(font != nullptr)
    {
        return TTF_FontHeight(font->getFont());
    }
    else
    {
        Log::addEntry(1, "/!\\ Got nullptr font in getMaxHeight");
        return 0;
    }
}

int Font::getLineSkip(Font* font)
{
    if(font != nullptr)
    {
        return TTF_FontLineSkip(font->getFont());
    }
    Log::addEntry(1, "/!\\ Got nullptr font in getLineSkip");
    return 0;
}

TTF_Font* Font::getFont()
{
    return this->m_font;
}

Font* Font::setFont(TTF_Font* font)
{
    this->m_font = font;
    return this;
}

Font* Font::getFontFromMap(Fonts font)
{
    if(Font::fontMap.count(font) > 0)
    {
        return Font::fontMap[font];
    }
    else
    {
        //error
    }

}

Font* Font::getFontFromMap(FontFamily family, int size)
{
    if(Font::fontMMap.count(family) > 0)
    {
        if(Font::fontMMap.at(family).count(size) > 0)
        {
            return Font::fontMMap.at(family).at(size);
        }
    }
    return nullptr;
}

static Fonts getFontEnum(FontFamily fontFamily, int size)
{
    map<int, Fonts> fontSizes;

    switch(fontFamily)
    {
        case FontFamily::ROBOTO :
            fontSizes = Font::robotoSizes;
            break;
        case FontFamily::AQUINO :
            fontSizes = Font::aquinoSizes;
            break;
        default :
            //error;
            break;
    }

    if(fontSizes.count(size) > 0)
    {
        return fontSizes.at(size);
    }
    //error
}

int Font::getBiggestFontToFit(FontFamily fontFamily, std::string text, int width, int height)
{
    int size;
    int smallestFont;

    switch(fontFamily)
    {
        case FontFamily::ROBOTO :
            size = Fonts::ROBOTOLAST;
            smallestFont = Fonts::ROBOTOFIRST;
            break;
        case FontFamily::AQUINO :
            size = Fonts::AQUINOLAST;
            smallestFont = Fonts::AQUINOFIRST;
            break;
        default :
            //error;
            break;
    }

    while(Font::getFontFromMap(fontFamily, size)->getMaxHeight() > height && size != smallestFont)
    {
        --size;
    }

    while(Text::getTextWidth(text, fontFamily, size) > width && size != smallestFont)
    {
        --size;
    }

    return size;
}

int Font::getBiggestFontForHeight(FontFamily fontFamily, std::string text, int height)
{
    int size;
    int smallestFont;

    switch(fontFamily)
    {
        case FontFamily::ROBOTO :
            size = Fonts::ROBOTOLAST;
            smallestFont = Fonts::ROBOTOFIRST;
            break;
        case FontFamily::AQUINO :
            size = Fonts::AQUINOLAST;
            smallestFont = Fonts::AQUINOFIRST;
            break;
        default :
            //error;
            break;
    }

    while(Font::getFontFromMap(fontFamily, size)->getMaxHeight() > height && size != smallestFont)
    {
        --size;
    }
    return size;
}

Fonts Font::getSmallerFont(FontFamily fontFamily, Fonts font)
{
 Fonts smallerFont;

 map<int, Fonts> fontSizes;
 map<Fonts, int> fontSizesReverser;

int smallestFont;

 switch(fontFamily)
 {
     case FontFamily::ROBOTO :
        fontSizes = Font::robotoSizes;
        fontSizesReverser = Font::robotoSizesReverser;
        smallestFont = Fonts::ROBOTOFIRST;
        break;
    case FontFamily::AQUINO :
        fontSizes = Font::aquinoSizes;
        fontSizesReverser = Font::aquinoSizesReverser;
        smallestFont = Fonts::AQUINOFIRST;
        break;
    default :
        //error
        break;
 }

 if(fontSizesReverser.count(font) > 0)
 {
     int size = fontSizesReverser[font];

     bool found = false;



     while(size >= smallestFont && !found)
     {
         --size;
         if(fontSizes.count(size) > 0)
         {
             smallerFont = fontSizes[size];
             found = true;
         }
         else
         {
             //error

         }
     }
 }
 else
 {
     cout << "Wrong font" << endl;
 }
 return smallerFont;
}

int Font::getSmallerFont(FontFamily fontFamily, int fontSize)
{
    map<int, Fonts> fontSizes;

    switch(fontFamily)
    {
        case FontFamily::ROBOTO :
           fontSizes = Font::robotoSizes;
           break;
       case FontFamily::AQUINO :
           fontSizes = Font::aquinoSizes;
           break;
       default :
           //error
           break;
    }

    if(fontSizes.count(fontSize) > 0)
    {
        return Font::getSmallerFont(fontFamily, fontSizes.at(fontSize));
    }

    //error
    return 0;
}
