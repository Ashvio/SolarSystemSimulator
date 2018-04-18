#include <GL/glew.h>
#include <debuggl.h>
#include <iostream>
#include "texture_to_render.h"



TextureToRender::TextureToRender()
{
}

TextureToRender::~TextureToRender()
{
	if (framebuffer < 0)
		return ;
	unbind();
	glDeleteFramebuffers(1, &framebuffer);
	glDeleteTextures(1, &renderedTexture);
	glDeleteRenderbuffers(1, &depthrenderbuffer);
}

void TextureToRender::create(int width, int height)
{
	w_ = width;
	h_ = height;
	// FIXME: Create the framebuffer object backed by a texture
	framebuffer = 0;
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	// The texture we're going to render to
	renderedTexture = 0;
	glGenTextures(1, &renderedTexture);

	// "Bind" the newly created texture : all future texture functions will modify this texture
	glBindTexture(GL_TEXTURE_2D, renderedTexture);

	// Give an empty image to OpenGL ( the last "0" )
	glTexImage2D(GL_TEXTURE_2D, 0,GL_RGB, w_, h_, 0,GL_RGB, GL_UNSIGNED_BYTE, 0);

	// Poor filtering. Needed !
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	depthrenderbuffer = 0;
	glGenRenderbuffers(1, &depthrenderbuffer);
	glBindRenderbuffer(GL_RENDERBUFFER, depthrenderbuffer);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 1024, 768);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthrenderbuffer);

		// Set "renderedTexture" as our colour attachement #0
	glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, renderedTexture, 0);

	// Set the list of draw buffers.
	GLenum DrawBuffers[1] = {GL_COLOR_ATTACHMENT0};
	glDrawBuffers(1, DrawBuffers); // "1" is the size of DrawBuffers
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cerr << "Failed to create framebuffer object as render target" << std::endl;
	} else {
		std::cerr << "Framebuffer ready" << std::endl;
	}
	unbind();
}

void TextureToRender::bind()
{
	// Render to our framebuffer
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glViewport(0,0,w_,h_); // Render on the whole framebuffer, complete from the lower left corner to the upper right
	// FIXME: bind the framebuffer object to GL_FRAMEBUFFER
}

void TextureToRender::unbind()
{
	
	// FIXME: Unbind current framebuffer object from the render target
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	//glViewport(960,0,1280,720); // Render on the whole framebuffer, complete from the lower left corner to the upper right
}
