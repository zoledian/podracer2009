
#ifdef __cplusplus
extern "C" {
#endif 

#ifndef readjpeg_h
#define readjpeg_h

int read_JPEG_file(char * filename, char** pixels, int* width, int* height);

#endif

#ifdef __cplusplus
 }
 #endif 
