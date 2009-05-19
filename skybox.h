#ifndef SKYBOX_H
#define SKYBOX_H

class SkyBox {

public: 
	SkyBox();
	void drawSkyBox(GLdouble*);

 private:
	GLuint loadTexture(char* name);
	GLuint textureId;
};

#endif
