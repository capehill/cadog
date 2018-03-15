#ifndef __SDLGL_H
#define __SDLGL_H



bool hasVersion(int major, int minor);
bool hasExtension(const char *str);
bool getFormat(const SDL_PixelFormat *fmt, GLenum &format, GLenum &type, GLenum &internal);
GLuint loadTextureFromMemory(const void *data, int size, int width, int height);
GLuint loadTextureFromFP(FILE *fp, int size, int width, int height);
GLuint loadTexture(SDL_Surface *img);

#endif
