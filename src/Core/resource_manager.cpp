#include <stdio.h>
#include "resource_manager.h"
#include "logger.h"
#include "Graphics/mesh.h"
#include "Graphics/shader.h"
#include "Graphics/texture.h"
#include "Graphics/model.h"
#include "Events/async.h"
#include "threadpool.h"
#include "application.h"

namespace Lynx {


ResourceManager::ResourceManager(ThreadPool* pool) : thpool(pool), lastId(0)
{
	#ifdef LYNX_MULTITHREAD
	EventManager::AddListener(AsyncTextureLoad, [this](const Event& ev){
		const AsyncTextureLoadEvent& event = static_cast<const AsyncTextureLoadEvent&>(ev);
		log_debug("b");
		LYNX_ASSERT(event.Tex, "Fuck");
		event.Tex->GetResourceID();
		log_debug("a");
		event.Tex->Generate();
    });
	#endif
}

ResourceManager::ResourceManager()
{

}

ResourceManager::~ResourceManager()
{
	Clear();
}

/**
 * @brief Clear all resources ( UNUSED )
 *
 */
void ResourceManager::Clear()
{
}

/**
 * @brief Updates the resource queue ( UNUSED )
 *
 */
void ResourceManager::Update(float dt)
{/*
#ifdef LYNX_MULTITHREAD
	if(texdata_queue.empty())
		return;

	TextureBase* tex;
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		tex = texdata_queue.front();
	}

	log_debug("Uploading texture %s to GPU", tex->GetResourcePath());

	tex->Generate();
		
	{
		std::unique_lock<std::mutex> lock(queue_mutex);
		texdata_queue.pop();
	}
	
#endif*/
}

const char* ResourceManager::getFileName(const char* path)
{
	std::string str(path);

	size_t s = str.rfind('/', str.length());
	if(s != str.npos) {
		return (str.substr(s+1, str.length() - s)).c_str();
	}

	return "";
}

/**
 * @brief Loads a shader resource
 *
 * @param vpath Vertex shader path
 * @param fpath Fragment shader path
 * @return std::shared_ptr<Shader>
 */
std::shared_ptr<Shader> ResourceManager::LoadShader(const char* vpath, const char* fpath)
{
	std::string name = std::string(vpath);
	const auto shader_found = GetResource<Shader>(vpath);
	if(shader_found != NULL)
		return shader_found;

	auto n_shader = std::make_shared<Shader>(vpath, fpath);
	resource_map[Resource::GetLastID()] = std::static_pointer_cast<Resource>(n_shader);
	
	return n_shader;
}

/**
 * @brief Loads a texture
 *
 * @param path Texture path
 * @param type Texture type
 * @return std::shared_ptr<TextureBase>
 */
std::shared_ptr<TextureBase> ResourceManager::LoadTexture(const char* path, TextureType type)
{
	std::shared_ptr<TextureBase> texture = GetResource<TextureBase>(path);
	if( texture ) {
		if( texture->IsValid() )
			return texture;
	}

	std::string name = std::string(path);
	
#ifdef LYNX_MULTITHREAD
	auto n_tex = std::make_shared<Texture>(type);
	TexObj* obj = new TexObj{n_tex, path};

	log_debug("Texture %s", obj->path.c_str());

	log_debug("Starting to load texture %s in async mode", path);
	thpool->PushJob([this](void* data){
		TexObj* tdata = (TexObj*)data;
		std::shared_ptr<TextureBase> tex = tdata->tex;

		log_debug("Processing texture %s", tdata->path.c_str());

		tex->LoadFromFile(tdata->path);

		EventManager::SendEvent(AsyncTextureLoadEvent(tex));

		log_debug("Added texture %s to GPU queue", tex->GetResourcePath().c_str());
		free(tdata);
		LYNX_ASSERT(tex, "Fuck");
		return;
	}, obj);
#else
	auto n_tex = std::make_shared<Texture>(path, type);
	n_tex->Generate();
#endif
	
	log_debug("New texture resource id : %ld", n_tex->GetResourceID());
	resource_map[Resource::GetLastID()] = std::static_pointer_cast<Resource>(n_tex);
	
	return n_tex;
}

/**
 * @brief Loads a mesh
 *
 * @param name
 * @param vertices
 * @param indices
 * @param type
 * @return std::shared_ptr<Mesh>
 */
std::shared_ptr<Mesh>
ResourceManager::LoadMesh(const char* name, std::vector<Vertex>* vertices,
	std::vector<unsigned int>* indices,
	MeshType type)
{
	std::string res_name = name;
	const auto found = GetResource<Mesh>(name);
	if(found != NULL)
		return found;

	auto n_mesh = std::make_shared<Mesh>(name, vertices, indices, type);
	resource_map[Resource::GetLastID()] = std::static_pointer_cast<Resource>(n_mesh);
	
	return n_mesh;
}

/**
 * @brief Finds a resource by its path
 *
 * @param path
 * @return std::shared_ptr<Resource>
 */
std::shared_ptr<Resource> ResourceManager::FindResourceByPath(const std::string& path)
{
	for(auto const& [k,v] : resource_map) {
		if(v->GetResourcePath() == path) {
			return v;
		}
	}
	return NULL;
}

}
