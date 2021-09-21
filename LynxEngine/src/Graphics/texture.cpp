#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "texture.h"
#include "Core/logger.h"
#include "rendererAPI.h"

namespace Lynx::Graphics {

	static int texcount = 0;

	Texture::Texture(const char* path) : path(path)
	{ 
		LoadFromFile(path);
		texture = RendererAPI::GenTexture();
		RendererAPI::BindTexture(texture);	
	}

	void Texture::use()
	{
		if(!IsValid())
			return;
		
		RendererAPI::UseTexture(id);
		RendererAPI::BindTexture(texture);
	}

	void Texture::LoadFromFile(const char* path)
	{
		log_debug("Loading texture %s", path);
		tex_data.data = stbi_load(path, &tex_data.width, &tex_data.height, &tex_data.channels, 0);
	}

	void Texture::Generate(int tid)
	{
		if(tex_data.data){
			// Wrapping / Filtering settings
			switch ( IRendererAPI::GetAPI() ){
				case API_OPENGL:
					RendererAPI::SetTextureParameter(GL_TEXTURE_WRAP_S, GL_REPEAT);	
					RendererAPI::SetTextureParameter(GL_TEXTURE_WRAP_T, GL_REPEAT);
					RendererAPI::SetTextureParameter(GL_TEXTURE_MIN_FILTER, GL_LINEAR);
					RendererAPI::SetTextureParameter(GL_TEXTURE_MAG_FILTER, GL_LINEAR);
					break;
			}
			RendererAPI::LoadTexture(tex_data.data, tex_data.width, tex_data.height, false);
			id = tid;
			log_debug("Texture id is %d", id);
		}else{
			log_error("Unable to load texture %s", path);
		}
		stbi_image_free(tex_data.data);
	}

	TextureData* loadTexture(const char* path)
	{
		TextureData* texdata = (TextureData*)malloc(sizeof(TextureData));

		int width, height, channels;
		unsigned char *data = stbi_load(path, &width, &height, &channels, 0);

		if(!data){
			log_error("Unable to load texture %s", path);
		}
		texdata->width = width;
		texdata->height = height;
		texdata->channels = channels;
		texdata->data = data;

		return texdata;
	}

	void textureData_free(TextureData* data)
	{
		stbi_image_free(data->data);
		free(data);
	}

}