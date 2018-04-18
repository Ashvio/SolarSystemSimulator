#ifndef TEXTURE_TO_RENDER_H
#define TEXTURE_TO_RENDER_H

class TextureToRender {
public:
	TextureToRender();
	~TextureToRender();
	void create(int width, int height);
	void bind();
	void unbind();
	int getTexture() const { return renderedTexture; }
private:
	int w_, h_;
	unsigned int framebuffer = -1;
	unsigned int renderedTexture = -1;
	unsigned int depthrenderbuffer = -1;
};

#endif
