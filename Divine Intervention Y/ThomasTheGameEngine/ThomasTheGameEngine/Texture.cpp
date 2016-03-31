#include "Texture.h"
#include <iostream>

Texture::Texture(SDL_Surface* _surface, std::string id_)
: width(_surface->w), height(_surface->h), surface(_surface), ID(id_)
{
	dataType = TextureDataType::UnsignedByte;

	SDL_PixelFormat *fmt;
	fmt = _surface->format;

	if ((int)fmt->BitsPerPixel == 8)
	{
		SDL_Surface* _formattedSurf = SDL_ConvertSurfaceFormat(_surface, SDL_PIXELFORMAT_RGBA8888, 0);
		SDL_Surface* temp = _surface;
		_surface = _formattedSurf;

		SDL_FreeSurface(temp);
	}

	pixelData = _surface->pixels;
}

Texture::Texture(void* _pixelData, UINT32 _width, UINT32 _height, UINT32 _depth, std::string id_)
: pixelData(_pixelData), width(_width), height(_height), depth(_depth), ID(id_)
{
	dataType = TextureDataType::UnsignedByte_3D;
}

Texture::Texture(void *_pixelData, UINT32 _width, UINT32 _height, std::string id_)
: pixelData(_pixelData), width(_width), height(_height), ID(id_)
{
	dataType = TextureDataType::Float;
}

Texture::~Texture()
{
	if (surface)
		SDL_FreeSurface(surface);
	else
		delete pixelData;
}

