#include "TextSprite.h"
#include "ModelManager.h"

MultiLineTextSprite::MultiLineTextSprite(std::string _text, TTF_Font* _font, Colour _textColor, Uint32 wrapLength_)
: Texture(TTF_RenderText_Blended_Wrapped(_font, _text.c_str(), _textColor, CompareMinimumWordLength(_text, _font, wrapLength_)), "TTGE_MULTILINE_TEXTSPRITE")
{
	//Self Register
	mapAddress = ModelManager::nextTextureID;
	ModelManager::getInstance()->textures.insert(std::pair<int, Texture*>(ModelManager::nextTextureID, this));

	ModelManager::nextTextureID++;
}

void MultiLineTextSprite::ReCreateTexture(std::string _text, TTF_Font* _font, Colour _textColor, Uint32 wrapLength_)
{

	if (surface)
		SDL_FreeSurface(surface);

	surface = TTF_RenderText_Blended_Wrapped(_font, _text.c_str(), _textColor, wrapLength_);

	width = surface->w;
	height = surface->h;
	dataType = TextureDataType::UnsignedByte;

	SDL_PixelFormat *fmt;
	fmt = surface->format;

	if ((int)fmt->BitsPerPixel == 8)
	{
		SDL_Surface* _formattedSurf = SDL_ConvertSurfaceFormat(surface, SDL_PIXELFORMAT_RGBA8888, 0);
		SDL_Surface* temp = surface;
		surface = _formattedSurf;
		SDL_FreeSurface(temp);
	}

	pixelData = surface->pixels;

	glDeleteTextures(1, &address);
	glGenTextures(1, &address);
	glBindTexture(GL_TEXTURE_2D, address);

	switch (dataType)
	{
	case Texture::TextureDataType::Float:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_FLOAT, pixelData);
		break;
	case Texture::TextureDataType::UnsignedByte:
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, pixelData);
		break;
	}

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
}

Uint32 MultiLineTextSprite::CompareMinimumWordLength(std::string text_, TTF_Font* font_, Uint32 compareLength_)
{
	Uint32 wrapLength = compareLength_;
	//Uint32 longestWord = 0;
	std::vector<std::string> words;
	std::string text = text_;

	//Find Words
	for (int i = 0; i < text.size(); i++)
	{
		if (text[i] == ' ')
		{
			if (i > 0) // add word
			{
				words.push_back(text.substr(0, i));
				text = text.substr(i, text.size() - i);
				i = 0;
			}
			else //Trim space
			{
				if (text.size() == 1)
					break;
				else
				{
					text = text.substr(1, text.size()-1);
					i = 0;
				}
			}
		}
		else if (i == text.size() - 1) //last index and no space
		{
			words.push_back(text);
		}
	}

	//Create surfaces and get width
	for (int i = 0; i < words.size(); i++)
	{
		SDL_Surface *surface = TTF_RenderText_Blended(font_, words[i].c_str(), Colour::Black());
		wrapLength = surface->w > wrapLength ? surface->w : wrapLength;
		SDL_FreeSurface(surface);
	}

	return wrapLength;
}