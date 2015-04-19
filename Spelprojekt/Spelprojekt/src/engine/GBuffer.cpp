#include "GBuffer.h"

void Gbuffer::init(int x, int y, int nrTex, bool depth)
{

	if (renderQuad == 0)
	{
		genQuad();
	}

	glGenFramebuffers(1, &targetId);
	this->depth = depth;
	nrTextures = nrTex;

	generate(x, y);

	bind(GL_DRAW_FRAMEBUFFER);

	GLenum* DrawBuffers = new GLenum[nrTextures];
	
	for (size_t i = 0; i < nrTextures; i++)
	{
		if (i == 0 && depth)
		{
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, rTexture[i].getTargetId(), 0);
			DrawBuffers[i] = GL_NONE;
		}
		else
		{
			glFramebufferTexture2D(GL_DRAW_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, rTexture[i].getTargetId(), 0);
			DrawBuffers[i] = (GL_COLOR_ATTACHMENT0 + i);
		}
	}

	glDrawBuffers(nrTextures, DrawBuffers);

	delete[] DrawBuffers;

	GLenum Status = glCheckFramebufferStatus(GL_FRAMEBUFFER);

	if (Status != GL_FRAMEBUFFER_COMPLETE) {
		throw;
	}

	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, 0);

	if (!shaderPtr)
	{
		throw;
	}

	pos = new GLuint[nrTextures];

	if (depth)
	{
		pos[0] = 0;
	}
	pos[1] = glGetUniformLocation(*shaderPtr, "diffuse");
	pos[2] = glGetUniformLocation(*shaderPtr, "normal");
	pos[3] = glGetUniformLocation(*shaderPtr, "world");

	unifromCamPos = glGetUniformLocation(*shaderPtr, "lightPos");
	unifromPlayerPos = glGetUniformLocation(*shaderPtr, "playerPos");

	cameraPos = 0;
	playerPos = 0;

	//gui
	uniformGUItexture = glGetUniformLocation(*shaderGuiPtr, "diffuse");
	//defaults 0
	glProgramUniform1i(uniformGUItexture, uniformGUItexture, 0);
	uniformGUIModel = glGetUniformLocation(*shaderGuiPtr, "modelMatrix");
}

Gbuffer::~Gbuffer()
{
	delete[] rTexture;
	glDeleteBuffers(1, &targetId);
}

void Gbuffer::resize(int x, int y)
{
	for (size_t i = 0; i < nrTextures; i++)
	{
		rTexture[i].resize(x, y);
	}
}

void Gbuffer::bind(GLuint target)
{
	glBindFramebuffer(target, targetId);
}

void Gbuffer::render(glm::vec3* campos, const GUI* gui, const ContentManager* content, bool renderGui)
{
	// bind shader
	glUseProgram(*shaderPtr);

	// bind buffer
	glBindBuffer(GL_ARRAY_BUFFER, renderQuad);
	glBindVertexArray(renderVao);

	// bind textures
	for (size_t i = 1; i < nrTextures; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, rTexture[i].getTargetId());
		glProgramUniform1i(*shaderPtr, pos[i], i);
	}

	glProgramUniform3f(*shaderPtr, unifromCamPos, campos->x, campos->y, 4);
	glProgramUniform3f(*shaderPtr, unifromPlayerPos, playerPos[0], playerPos[1], 4);
	
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	if (renderGui)
	{
		glUseProgram(*shaderGuiPtr);

		int size = gui->readSize();
		ScreenItem** items = gui->getItems();
		//bind tex blit
		content->bindGUIvert();
		
		int id = 0;
		int lastid = -1;
		for (int n = 0; n < size; n++)
		{
			if (items[n]->visable())
			{
				id = items[n]->bindWorldMat(shaderGuiPtr, &uniformGUIModel);
				if (id != lastid)
					content->bindGUItex(id);
				glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
				lastid = id;
			}
		}
	}

}

void Gbuffer::generate(int x, int y)
{
	rTexture = new RenderTarget[nrTextures];
	for (size_t i = 0; i < nrTextures; i++)
	{
		if (i == 0 && depth)
		{
			rTexture[i].init(x, y, 0, true);
		}
		else
		{
			rTexture[i].init(x, y, 0, false);
		}
		
	}
}