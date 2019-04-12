#include "CubeMap.h"


CubeMap::CubeMap() {
}

CubeMap::CubeMap(std::vector<std::string> faces) {
	this->LoadCubeMap(faces);
}


CubeMap::~CubeMap() {
	glDeleteTextures(1, &this->textureId);
}

void CubeMap::LoadCubeMap(std::vector<std::string> faces) {
	glGenTextures(1, &(this->textureId));
	glBindTexture(GL_TEXTURE_CUBE_MAP, this->textureId);
	std::cout << "Generating Cube Map." << std::endl;
	for (unsigned int i = 0; i < faces.size(); i++) {
		const char* filename = (faces[i]).c_str();
		FREE_IMAGE_FORMAT fileFormat = FreeImage_GetFileType(filename, 0);
		if (fileFormat == -1) {
			std::cout << "Couldn't find file -> " << (faces[i]) << " - Aborting..." << std::endl;
			return;
			//exit(-1);
		}

		if (fileFormat == FIF_UNKNOWN) {
			std::cout << "Couldn't determine file format - trying with file extension..." << std::endl;

			fileFormat = FreeImage_GetFIFFromFilename(filename);
			if (!FreeImage_FIFSupportsReading(fileFormat)) {
				std::cout << "Image format cannot be read!" << std::endl;
				return;
			}
		}

		FIBITMAP* bitmap = FreeImage_Load(fileFormat, filename);
		int bitsPerPixel = FreeImage_GetBPP(bitmap);

		FIBITMAP* bitmap32 = NULL;
		//if image has 32 bytes, don't convert->use it
		if (bitsPerPixel == 32) {
			std::cout << "Source image has " << bitsPerPixel << "bits per pixel. Skipping conversion..." << std::endl;
			bitmap32 = bitmap;
		}
		else {
			std::cout << "Source image has " << bitsPerPixel << "bits per pixel. Converting to 32-bit colour..." << std::endl;
			bitmap32 = FreeImage_ConvertTo32Bits(bitmap);
		}

		int imageW = FreeImage_GetWidth(bitmap32);
		int imageH = FreeImage_GetHeight(bitmap32);

		GLubyte* textureData = FreeImage_GetBits(bitmap32);

		if (textureData) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, imageW, imageH, 0, GL_BGRA, GL_UNSIGNED_BYTE,
			             textureData);
		}

		FreeImage_Unload(bitmap32);
		if (bitsPerPixel != 32) {
			FreeImage_Unload(bitmap);
		}

	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
