#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#if defined(__APPLE__) && defined(__MACH__)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#else
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glext.h>
#endif
#include <stdio.h>
#include <string.h>
#include "sdlgl.h"
#include "rwops.h"

bool hasVersion(int major, int minor) {
	const char *version = (const char*) glGetString(GL_VERSION);
	int v1, v2;
	if (sscanf(version,"%d.%d",&v1,&v2)!=2) return false;
	if (v1<major) return false;
	if (v2<minor) return false;
	return true;
}

bool hasExtension(const char *str) {
	const char *extensions = (const char*) glGetString(GL_EXTENSIONS);
	int len = strlen(str);
	const char *ptr;
	while ((ptr=strstr(extensions,str))!=NULL) {
		if ((ptr==extensions || *(ptr-1)==' ') && (*(ptr+len)==' ' || *(ptr+len)=='\0'))
			return true;
		extensions = ptr+len;
	}
	return false;
}

bool getFormat(const SDL_PixelFormat *fmt, GLenum &format, GLenum &type, GLenum &internal) {
	bool pp = hasExtension("GL_EXT_packed_pixels");
	bool bgra = hasExtension("GL_EXT_bgra");
	bool abgr = hasExtension("GL_EXT_abgr");
	bool ver12 = hasVersion(1,2) || (hasExtension("GL_APPLE_packed_pixels") && bgra);

//printf("%d, %d, %d, %d\n", pp, bgra, abgr, ver12);

	const Uint8 &bpp = fmt->BitsPerPixel;
	const Uint32 &Rmask = fmt->Rmask;
	const Uint32 &Gmask = fmt->Gmask;
	const Uint32 &Bmask = fmt->Bmask;
	const Uint32 &Amask = fmt->Amask;
	if (bpp==32) {
printf("%d\n", __LINE__);
		internal = GL_RGB8;
		if (Rmask==0x00FF0000 && Gmask==0x0000FF00 && Bmask==0x000000FF) {
printf("%d\n", __LINE__);
			if (!ver12) return false;
			format = GL_BGRA;
			type = GL_UNSIGNED_INT_8_8_8_8_REV;
			if (Amask==0xFF000000) internal = GL_RGBA8;
printf("%d\n", __LINE__);
		} else if (Rmask==0x000000FF && Gmask==0x0000FF00 && Bmask==0x00FF0000) {
printf("%d\n", __LINE__);
			format = GL_RGBA;
			type = GL_UNSIGNED_INT_8_8_8_8_REV;
			if (Amask==0xFF000000) internal = GL_RGBA8;
			if (!ver12) {
printf("%d\n", __LINE__);
				if (pp && abgr) {
printf("%d\n", __LINE__);
					type = GL_UNSIGNED_INT_8_8_8_8;	// GL_UNSIGNED_INT_8_8_8_8_EXT
					format = GL_ABGR_EXT;
				} else return false;
			}
		} else if (Rmask==0x0000FF00 && Gmask==0x00FF0000 && Bmask==0xFF000000) {
printf("%d\n", __LINE__);
			format = GL_BGRA; // == GL_BGRA_EXT
			type = GL_UNSIGNED_INT_8_8_8_8; // == GL_UNSIGNED_INT_8_8_8_EXT
			if (Amask==0x000000FF) {
				printf("RGBA8\n");
				internal = GL_RGBA8;
			}
printf("%d\n", __LINE__);
			if (!ver12 && !(pp && bgra)) return false;
		} else if (Rmask==0xFF000000 && Gmask==0x00FF0000 && Bmask==0x0000FF00) {
printf("%d\n", __LINE__);
			format = GL_RGBA;
			type = GL_UNSIGNED_INT_8_8_8_8; // == GL_UNSIGNED_INT_8_8_8_8_EXT
			if (Amask==0x000000FF) internal = GL_RGBA8;
			if (!ver12 && !pp) return false;
		} else return false;
printf("%d\n", __LINE__);
	} else if (bpp==24) {
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
		if (Rmask==0xFF0000 && Gmask==0x00FF00 && Bmask==0x0000FF)
			format = GL_BGR;
		else if (Rmask==0x0000FF && Gmask==0x00FF00 && Bmask==0xFF0000)
			format = GL_RGB;
		else return false;
#else
		if (Rmask==0xFF0000 && Gmask==0x00FF00 && Bmask==0x0000FF)
			format = GL_RGB;
		else if (Rmask==0x0000FF && Gmask==0x00FF00 && Bmask==0xFF0000)
			format = GL_BGR;
		else return false;
#endif
		if (format==GL_BGR) {
			if (!ver12) {
				if (bgra) format = GL_BGR_EXT;
				else return false;
			}
		}
		type = GL_UNSIGNED_BYTE;
		internal = GL_RGB8;
	} else if (bpp==16) {
		internal = GL_RGB8;
		if (Rmask==0xF800 && Gmask==0x07C0 && Bmask==0x003E && Amask==0x0001) {
			format = GL_RGBA;
			type = GL_UNSIGNED_SHORT_5_5_5_1; // == GL_UNSIGNED_SHORT_5_5_5_1_EXT
			internal = GL_RGBA8;
			if (!ver12 && !pp) return false;
		} else if (Rmask==0x003E && Gmask==0x07C0 && Bmask==0xF800 && Amask==0x0001) {
			format = GL_BGRA; // == GL_BGRA_EXT
			type = GL_UNSIGNED_SHORT_5_5_5_1; // == GL_UNSIGNED_SHORT_5_5_5_1_EXT
			internal = GL_RGBA8;
			if (!ver12 && !(pp && bgra)) return false;
		} else if (Rmask==0x7C00 && Gmask==0x03E0 && Bmask==0x001F && Amask==0x8000) {
			if (!ver12) return false;
			format = GL_BGRA;
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			internal = GL_RGBA8;
		} else if (Rmask==0x001F && Gmask==0x03E0 && Bmask==0x7C00 && Amask==0x8000) {
			if (!ver12) return false;
			format = GL_RGBA;
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
			internal = GL_RGBA8;
		} else if (Rmask==0xF800 && Gmask==0x07E0 && Bmask==0x001F) {
			if (!ver12) return false;
			format = GL_RGB;
			type = GL_UNSIGNED_SHORT_5_6_5;
		} else if (Rmask==0x001F && Gmask==0x07E0 && Bmask==0xF800) {
			if (!ver12) return false;
			format = GL_RGB;
			type = GL_UNSIGNED_SHORT_5_6_5_REV;
		} else return false;
	} else if (bpp==15) {
		internal = GL_RGB8;
		if (Rmask==0x7C00 && Gmask==0x03E0 && Bmask==0x001F) {
			if (!ver12) return false;
			format = GL_BGRA;
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		} else if (Rmask==0x001F && Gmask==0x03E0 && Bmask==0x7C00) {
			if (!ver12) return false;
			format = GL_RGBA;
			type = GL_UNSIGNED_SHORT_1_5_5_5_REV;
		} else if (Rmask==0xF800 && Gmask==0x07C0 && Bmask==0x003E) {
			format = GL_RGBA;
			type = GL_UNSIGNED_SHORT_5_5_5_1; // == GL_UNSIGNED_SHORT_5_5_5_1_EXT
			if (!ver12 && !pp) return false;
		} else if (Rmask==0x003E && Gmask==0x07C0 && Bmask==0xF800) {
			format = GL_BGRA; // == GL_BGRA_EXT
			type = GL_UNSIGNED_SHORT_5_5_5_1; // == GL_UNSIGNED_SHORT_5_5_5_1_EXT
			if (!ver12 && !(pp && bgra)) return false;
		} else return false;
	} else {
		return false;
	}
	return true;
}

GLuint loadTextureScaled(SDL_Surface *img, int width, int height) {
	if (img==NULL) return 0;
	if (width>0 && height>0) {
printf("Here\n");
		SDL_Surface *img2 = SDL_CreateRGBSurface(SDL_SWSURFACE,width,height,img->format->BitsPerPixel,img->format->Rmask,img->format->Gmask,img->format->Bmask,img->format->Amask);
		if (width==img->w/2 && height==img->h/2) {
			Uint8 *inptr = (Uint8*) img->pixels;
			Uint8 *outptr = (Uint8*) img2->pixels;
			for (int y = 0; y < img2->h; y++) {
				for (int x = 0; x < img2->w; x++) {
					Uint32 r, g, b, a;
					r = g = b = a = 0;
					Uint8 cr, cg, cb, ca;
					Uint32 pixel;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
#define shiftPixel()			pixel >>= (32-img->format->BitsPerPixel);
#else
#define shiftPixel()
#endif

#define handlePixel(x,y)								\
					pixel = 0;					\
					memcpy(&pixel,inptr+img->pitch*(y)+img->format->BytesPerPixel*(x),img->format->BytesPerPixel);		\
					shiftPixel();					\
					SDL_GetRGBA(pixel,img->format,&cr,&cg,&cb,&ca);	\
					r += ca*cr; g += ca*cg; b += ca*cb; a += ca;

					handlePixel(x*2,y*2);
					handlePixel(x*2+1,y*2);
					handlePixel(x*2,y*2+1);
					handlePixel(x*2+1,y*2+1);
					if (a>0) {
						r /= a;
						g /= a;
						b /= a;
					}
					a >>= 2;
					pixel = SDL_MapRGBA(img2->format,r,g,b,a);
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
					pixel <<= (32-img2->format->BitsPerPixel);
#endif
					memcpy(outptr,&pixel,img2->format->BytesPerPixel);
					outptr += img2->format->BytesPerPixel;
				}
				outptr += img2->pitch - img2->format->BytesPerPixel*img2->w;
			}
		} else {
			Uint8 *inptr = (Uint8*) img->pixels;
			Uint8 *outptr = (Uint8*) img2->pixels;
			for (int y = 0; y < img2->h; y++) {
				int iny = img->h*y/img2->h;
				for (int x = 0; x < img2->w; x++) {
					int inx = img->w*x/img2->w;
					memcpy(outptr,inptr+img->pitch*iny+img->format->BytesPerPixel*inx,img->format->BytesPerPixel);
					outptr += img2->format->BytesPerPixel;
				}
				outptr += img2->pitch - img2->format->BytesPerPixel*img2->w;
			}
		}
		SDL_FreeSurface(img);
		return loadTexture(img2);
	} else {
		return loadTexture(img);
	}
}

GLuint loadTextureFromMemory(const void *data, int size, int width, int height) {
	SDL_RWops *rw = SDL_RWFromConstMem(data,size);
	SDL_Surface *img = IMG_Load_RW(rw,0);
	if (img==NULL) {
		SDL_RWseek(rw,0,SEEK_SET);
		img = IMG_LoadTGA_RW(rw);
	}
	SDL_RWclose(rw);
	return loadTextureScaled(img,width,height);
}


GLuint loadTextureFromFP(FILE *fp, int size, int width, int height) {
	SDL_RWops *rw = rwFromSizedFP(fp,size,false);
	SDL_Surface *img = IMG_Load_RW(rw,0);
	if (img==NULL) {
		SDL_RWseek(rw,0,SEEK_SET);
		img = IMG_LoadTGA_RW(rw);
	}
	SDL_RWclose(rw);
	return loadTextureScaled(img,width,height);
}

GLuint loadTexture(SDL_Surface *img) {

	GLuint texture;
	glGenTextures(1,&texture);
	glBindTexture(GL_TEXTURE_2D,texture);

	GLenum internal, format, type;

printf("%d * %d * %d (%d), A %X, R %X, G %X B %X\n",
	img->w, img->h,
	img->format->BitsPerPixel,
	img->format->BytesPerPixel,
	img->format->Amask, 
	img->format->Rmask,
	img->format->Gmask,
	img->format->Bmask);

#if 0
	if (!getFormat(img->format,format,type,internal))
#else
#endif
	{
		SDL_PixelFormat fmt = {
			NULL, 32, 4,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN
			0, 8, 16, 24,
			0, 0, 0, 0,
			0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000,
#else
			24, 16, 8, 0,
			0, 0, 0, 0,
			0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF,
#endif
			0, 255 };
		SDL_Surface *img2 = SDL_ConvertSurface(img,&fmt,SDL_SWSURFACE);
		if(!img2)
		{
			printf("NULL\n");
		}
 
		SDL_FreeSurface(img);
		img = img2;
		internal = GL_RGBA;
		format = GL_RGBA;
		type = GL_UNSIGNED_BYTE;
	}

printf("internal %d, format %d, type %d\n", internal, format, type);

	if (SDL_MUSTLOCK(img)) SDL_LockSurface(img);
	glTexImage2D(GL_TEXTURE_2D,0,internal,img->w,img->h,0,format,type,img->pixels);
	GLenum err;
	if ((err = glGetError())!=0) printf("%s\n",(char*)gluErrorString(err));

	if (SDL_MUSTLOCK(img)) SDL_UnlockSurface(img);
	SDL_FreeSurface(img);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	return texture;
}



