#include "Texture.h"
#include <FreeImage.h>
#include <iostream>



Texture::Texture()
{
}



Texture::~Texture()
{
}

void Texture::LoadTexture(std::string filenameString, GLenum minificationFilter, GLenum magnificationFilter)
{
	const char* filename = filenameString.c_str();
	FREE_IMAGE_FORMAT fileFormat = FreeImage_GetFileType(filename, 0);

	if (fileFormat == -1)
	{
		std::cout << "Couldn't find file -> " << filenameString << " - Aborting..." << std::endl;
		exit(-1);
	}

	if (fileFormat == FIF_UNKNOWN)
	{
		std::cout << "Couldn't determine file format - trying with file extension..." << std::endl;

		fileFormat = FreeImage_GetFIFFromFilename(filename);
		if (!FreeImage_FIFSupportsReading(fileFormat))
		{
			std::cout << "Image format cannot be read!" << std::endl;
			exit(-1);
		}
	}

	FIBITMAP* bitmap = FreeImage_Load(fileFormat, filename);
	int bitsPerPixel = FreeImage_GetBPP(bitmap);

	FIBITMAP* bitmap32 = NULL;
	//if image has 32 bytes, don't convert->use it
	if (bitsPerPixel == 32)
	{
		std::cout << "Source image has " << bitsPerPixel << "bits per pixel. Skipping conversion..." << std::endl;
		bitmap32 = bitmap;
	}
	else
	{
		std::cout << "Source image has " << bitsPerPixel << "bits per pixel. Converting to 32-bit colour..." << std::endl;
		bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
	}

	int imageW = FreeImage_GetWidth(bitmap32);
	int imageH = FreeImage_GetHeight(bitmap32);

	GLubyte* textureData = FreeImage_GetBits(bitmap32);
	GLuint tempTextureId;
	glGenTextures(1, &tempTextureId);
	glBindTexture(GL_TEXTURE_2D, tempTextureId);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, imageW, imageH, 0, GL_BGRA, GL_UNSIGNED_BYTE, textureData);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minificationFilter);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magnificationFilter);

	if (minificationFilter == GL_LINEAR_MIPMAP_LINEAR ||
		minificationFilter == GL_LINEAR_MIPMAP_NEAREST ||
		minificationFilter == GL_NEAREST_MIPMAP_LINEAR ||
		minificationFilter == GL_NEAREST_MIPMAP_NEAREST)
	{
		glGenerateMipmap(GL_TEXTURE_2D);
	}

	GLenum glError = glGetError();
	if (glError)
	{
		std::cout << "There was an error loading the texture: " << filenameString << std::endl;

		switch (glError)
		{
		case GL_INVALID_ENUM:
			std::cout << "Invalid enum." << std::endl;
			break;

		case GL_INVALID_VALUE:
			std::cout << "Invalid value." << std::endl;
			break;

		case GL_INVALID_OPERATION:
			std::cout << "Invalid operation." << std::endl;

		default:
			std::cout << "Unrecognised GLenum." << std::endl;
			break;
		}
	}


	FreeImage_Unload(bitmap32);
	if (bitsPerPixel != 32)
	{
		FreeImage_Unload(bitmap);
	}

	this->TextureId = tempTextureId;
	std::cout << "Texture Loading has ended. The TextureId is " << TextureId << std::endl;
}

