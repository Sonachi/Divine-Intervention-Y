//Author:	Mathieu Violette
//Date:		12/01/2015

#ifndef FONT_MANAGER_H
#define FONT_MANAGER_H

#include <map>
#include <SDL_ttf.h>

class FontManager
{
public:
	static FontManager* instance;
	static FontManager* getInstance();

	~FontManager();

	TTF_Font* GenerateFont(std::string _fontID, int _fontSize, std::string _path);

	TTF_Font* GetFont(std::string _fontID);

private:
	FontManager();

	std::map<std::string, TTF_Font*> fonts;
};

#endif