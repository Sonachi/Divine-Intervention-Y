#include "TextSprite.h"
#include "ModelManager.h"

TextSprite::TextSprite(std::string _text, TTF_Font* _font, Colour _textColor)
: Texture(TTF_RenderText_Blended(_font, _text.c_str(), _textColor), "TTGE_TEXTSPRITE")
{
	//Self Register
	mapAddress = ModelManager::nextTextureID;
	ModelManager::getInstance()->textures.insert(std::pair<int, Texture*>(ModelManager::nextTextureID, this));

	ModelManager::nextTextureID++;
}

void TextSprite::ReCreateTexture(std::string _text, TTF_Font* _font, Colour _textColor)
{

	if (surface)
		SDL_FreeSurface(surface);

	surface = TTF_RenderText_Blended(_font, _text.c_str(), _textColor);

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