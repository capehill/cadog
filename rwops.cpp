#include <SDL/SDL.h>
#include <stdlib.h>
#include <stdio.h>


struct rw_fp {
	FILE *fp;
	int begin, size, pos;
	bool autoclose;
};

int rw_fp_seek(SDL_RWops *ctx, int offset, int whence) {
	struct rw_fp *data = (struct rw_fp*) ctx->hidden.unknown.data1;
	switch (whence) {
	case SEEK_SET:
		data->pos = offset;
		break;
	case SEEK_CUR:
		data->pos += offset;
		break;
	case SEEK_END:
		data->pos = data->size+offset;
		break;
	default:
		return -1;
		break;
	}
	if (data->pos<0) data->pos = 0;
	if (data->pos>data->size) data->pos = data->size;
	if (fseek(data->fp,data->begin+data->pos,SEEK_SET)==0)
		return data->pos;
//	SDL_Error(SDL_EFSEEK);
	return -1;
}

int rw_fp_read(SDL_RWops *ctx, void *ptr, int size, int maxnum) {
	struct rw_fp *data = (struct rw_fp*) ctx->hidden.unknown.data1;
	int n = maxnum;
	int n2 = (data->size-data->pos)/size;
	if (n2<n) n = n2;
//	if (data->pos == data->size) return -1;
	n = fread(ptr,size,n,data->fp);
	data->pos += n*size;
//	if (n==0 && ferror(data->fp))
//		SDL_Error(SDL_EFREAD);
	return n;
}

int rw_fp_write(SDL_RWops *ctx, const void *ptr, int size, int num) {
	struct rw_fp *data = (struct rw_fp*) ctx->hidden.unknown.data1;
	int n = num;
	int n2 = (data->size-data->pos)/size;
	if (n2<n) n = n2;
//	if (data->pos == data->size) return -1;
	n = fwrite(ptr,size,n,data->fp);
	data->pos += n*size;
//	if (n==0 && ferror(data->fp))
//		SDL_Error(SDL_EFWRITE);
	return n;
}

int rw_fp_close(SDL_RWops *ctx) {
	struct rw_fp *data = (struct rw_fp*) ctx->hidden.unknown.data1;
	if (data->autoclose)
		fclose(data->fp);
	free(data);
	free(ctx);
	return 0;
}


SDL_RWops *rwFromSizedFP(FILE *in, int size, bool autoclose = true) {
	SDL_RWops *rw = (SDL_RWops*) malloc(sizeof(SDL_RWops));
	if (rw==NULL) return NULL;
	rw->hidden.unknown.data1 = malloc(sizeof(struct rw_fp));
	if (rw->hidden.unknown.data1==NULL) {
		free(rw);
		return NULL;
	}
	struct rw_fp *data = (struct rw_fp*) rw->hidden.unknown.data1;
	data->fp = in;
	data->begin = ftell(in);
	data->size = size;
	data->pos = 0;
	data->autoclose = autoclose;
	rw->seek = rw_fp_seek;
	rw->read = rw_fp_read;
	rw->write = rw_fp_write;
	rw->close = rw_fp_close;
	return rw;
}

