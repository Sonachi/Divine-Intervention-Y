#include "FontManager.h"

FontManager* FontManager::instance;

FontManager::FontManager(){}

FontManager* FontManager::getInstance()
{
	if (!instance)
		instance = new FontManager();

	return instance;
}

FontManager::~FontManager()
{
	for (auto it = fonts.begin(); it != fonts.end(); it++)
	{
		TTF_CloseFont(it->second);
	}
}

TTF_Font* FontManager::GenerateFont(std::string _fontID, int _fontSize, std::string _path)
{
	//no duplicate fonts
	auto search = fonts.find(_fontID);
	if (search != fonts.end())
		return search->second;

	TTF_Font* font = TTF_OpenFont(_path.c_str(), _fontSize);

	fonts.insert(std::pair<std::string, TTF_Font*>(_fontID, font));

	return font;
}

TTF_Font* FontManager::GetFont(std::string _fontID)
{
	return fonts.find(_fontID)->second;
}