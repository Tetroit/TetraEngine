#pragma once
#include <format>
#include <filesystem>
#include <fstream>
#include <tetrapc.h>

#include "../rendering/Texture2D.h"
#include "../rendering/Material.h"
#include "../utils/Types.h"
#include "../utils/UUID.h"
#include "nlohmann/json.hpp"

namespace TetraEngine {

	struct ResourceMetadata {
		UUID guid;
		std::optional<std::string> path;
		asset_type_id type;
	};
	struct MetadataLoader {
		static std::optional<ResourceMetadata> LoadMetadata(std::filesystem::path path) {
			if (std::filesystem::exists(path)) {
				std::ifstream fileStream(path);
				nlohmann::json jsonFile;
				fileStream >> jsonFile;

				ResourceMetadata meta;
				meta.path = path.string().substr(0, path.string().find_last_of('.'));
				if (jsonFile.contains("guid")) {
					meta.guid = UUID(jsonFile["guid"].get<std::string>());
				}
				if (jsonFile.contains("type")) {
					meta.type = jsonFile["type"];
				}
				return meta;
			}
			return std::nullopt;
		}
	};

	class ISharedContentStorage {
	public:
		virtual ~ISharedContentStorage() = default;
		virtual void* GetItem(uint id) = 0;
		virtual uint GetGen(uint id) = 0;
		virtual UUID GetUuid(uint id) = 0;
		virtual asset_type_id GetType() = 0;
		virtual std::string GetPath(uint id) = 0;
		virtual void SetPath(uint id, std::string path) = 0;
		virtual bool HasPath(uint id) = 0;
		virtual ResourceMetadata GetMetadata(uint id) = 0;
		virtual ResourceMetadata& GetMetadataRef(uint id) = 0;
		virtual void HandleError(std::string message) = 0;
	};

	template <typename T>
	class SharedContentStorage;

	template <typename T>
	class ResourceHandle {
		friend class ResourceHandleTypeErased;
		friend class SharedContentRegistry;
		uint id = -1;
		uint gen = -1;
		ISharedContentStorage* storage = nullptr;

	public:
		static ResourceHandle<T> Invalid() {
			return ResourceHandle<T>(-1, asset_type_id());
		}
		bool isValid();

		uint GetID() { return id; };
		UUID GetUUID() {return storage->GetUuid(id);}
		uint GetGen(){ return gen; }
		asset_type_id GetType() { return storage->GetType(); }
		ResourceMetadata GetMetadata() { return storage->GetMetadata(id); }
		ResourceMetadata& GetMetadataRef(){ return storage->GetMetadataRef(id); }
		ResourceHandle(SharedContentStorage<T>* storage, uint id, uint gen) : id(id), gen(gen), storage(storage) {}
		T* operator->();
	};

	class ResourceHandleTypeErased {
		friend class SharedContentRegistry;
	protected:
		asset_type_id type = 0;
		uint id = -1;
		uint gen = -1;
		ISharedContentStorage* storage;
		template<typename T>
		ResourceHandleTypeErased(ResourceHandle<T> src) : storage(src.storage), id(src.id), gen(src.gen), type(src.GetType()) {}
		ResourceHandleTypeErased(ISharedContentStorage* storage, asset_type_id type, uint id, uint gen) :
		storage(storage),
		type(type),
		id(id),
		gen(gen){}
	public:
		asset_type_id GetType() { return type; }
		uint GetId() { return id; }
		uint GetGen() { return gen; }
		template<typename T>
		std::optional<ResourceHandle<T>> TryCast() {
			if (type == TypeInfo<T>::id) {
				return ResourceHandle<T>(storage, id, gen);
			}
			return std::nullopt;
		}
		template<typename T>
		ResourceHandle<T> Cast() {
			return ResourceHandle<T>(storage, id, gen);
		}
	};

	template <typename T>
	class SharedContentStorage;


	template <typename T>
	class SharedContentStorage : public ISharedContentStorage {
		std::vector<std::optional<T>> resources;
		std::vector<ResourceMetadata> metadata;
		std::vector<uint> generations;
		std::vector<uint> freeIDs;
	public:

		template <typename... Args>
		ResourceHandle<T> CreateItem(Args&&... args) {
			size_t id = 0;
			UUID newGuid;
			if (!freeIDs.empty()) {
				id = freeIDs.back();
				freeIDs.pop_back();
				resources[id].emplace(std::forward<Args>(args)...);
				metadata[id].guid = GetUuid(id);
				metadata[id].path = "";
			}
			else {
				id = resources.size();
				generations.push_back(0);
				resources.emplace_back(std::forward<Args>(args)...);
				metadata.emplace_back(newGuid, "");
			}

			return ResourceHandle<T>(this, (uint)id, generations.at(id));
		}
		template <typename... Args>
		ResourceHandle<T> CreateItemAt(const std::filesystem::path& path, Args&&... args) {
			std::filesystem::path metaPath = path.string() + ".meta";
			size_t id = 0;
			UUID newGuid;

			if (!freeIDs.empty()) {
				id = freeIDs.back();
				freeIDs.pop_back();
				resources[id].emplace(std::forward<Args>(args)...);
			}
			else {
				id = resources.size();
				generations.push_back(0);
				resources.emplace_back(std::forward<Args>(args)...);
				metadata.emplace_back();
			}
			bool hasMetadata = std::filesystem::exists(metaPath);
			if (hasMetadata) {
				auto loadedMeta = MetadataLoader::LoadMetadata(metaPath);
				if (loadedMeta.has_value()) {
					newGuid = loadedMeta.value().guid;
				}
				else {
					std::cerr << "Couldn't retrieve metadata from " << metaPath << std::endl;
				}
			}
			else {
				newGuid = UUID::Next();
			}
			metadata[id].guid = newGuid;
			metadata[id].path = path.string();
			metadata[id].type = TypeInfo<T>::id;
			if (!hasMetadata) SaveMetadata(id);
			return ResourceHandle<T>(this, (uint)id, generations.at(id));
		}
		std::function<void(std::string)> errorCallback;
		void* GetItem(uint id) override
		{
			if (id >= resources.size()) {
				HandleError( std::format("SharedContentStorage id ({}) was out of range ({})", id, resources.size()));
				return nullptr;
			}
			if (!resources[id].has_value()) {
				HandleError(std::format("SharedContentStorage object at slot {} was null", id));
				return nullptr;
			}
			return &(*resources[id]);
		}
		uint GetGen(uint id) override {
			return generations[id];
		}
		UUID GetUuid(uint id) override {
			return metadata[id].guid;
		}
		asset_type_id GetType() override {
			return TypeInfo<T>::id;
		}
		std::string GetPath(uint id) override {
			auto& ptr = metadata[id].path;
			return ptr.has_value() ? ptr.value() : std::string();
		}
		bool HasPath(uint id) override {
			return metadata[id].path.has_value();
		}
		ResourceMetadata GetMetadata(uint id) override {
			return metadata[id];
		}
		ResourceMetadata& GetMetadataRef(uint id) override {
			return metadata[id];
		}
		void SetPath(uint id, std::string path) override {
			metadata[id].path = path;
		}
		void DeleteItem(ResourceHandle<T> handle) {
			uint id = handle.id;

			if (id >= resources.size())
				return;

			if (!resources[id].has_value())
				return;

			resources[id].reset();
			generations[id]++;
			freeIDs.push_back(id);
		}
		void SaveMetadata(uint id) {
			auto& meta = metadata[id];
			nlohmann::json jsonFile;
			jsonFile["type"] = TypeInfo<T>::id;
			jsonFile["guid"] = (meta.guid.ToString());
			std::filesystem::path path(meta.path.value());
			path += ".meta";
			if (!std::filesystem::exists(path.parent_path()))
				std::filesystem::create_directories(path.parent_path());
			std::ofstream fileStream(path);
			if (!fileStream) {
				std::cerr << "Failed to open file " << path.string() << std::endl;
				return;
			}
			fileStream << jsonFile.dump(4);
		};
		void HandleError(std::string message) override {
			if (errorCallback)
				errorCallback(message);
			else {
				std::cout << message << std::endl;
			}
		}
		template <typename Fn>
		requires std::invocable<Fn&, T&>
		void Foreach(Fn&& fn) {
			for (int i = 0; i < resources.size(); i++) {
				if (resources[i].has_value()) {
					fn(resources[i].value());
				}
			}
		}
	};

	class SharedContentRegistry
	{
		template <typename T>
		friend class ResourceHandle;
		std::unordered_map<asset_type_id, std::unique_ptr<ISharedContentStorage>> registry;
		std::unordered_map<UUID, ResourceHandleTypeErased, UUIDHasher> uuidMap;
		std::pair<void*, uint> GetItemDirect(asset_type_id type, uint id) {
			auto& storage = registry[type];
			void* data = storage->GetItem(id);
			uint gen = storage->GetGen(id);
			return { data, gen };
		}
		template <typename T>
		ISharedContentStorage& AddStorage() {
			auto [it, success] = registry.try_emplace(TypeInfo<T>::id, std::make_unique<SharedContentStorage<T>>());
			if (!success) {
				HandleError( std::format("Failed to create storage of type {}", TypeInfo<T>::id));
			}
			auto storage = it->second.get();
			auto casted = static_cast<SharedContentStorage<T>*>(storage);
			casted->errorCallback = [this](std::string message) {
				HandleError(message);
			};
			return *casted;
		}
		template <typename T>
		SharedContentStorage<T>& GetStorage() {
			auto it = registry.find(TypeInfo<T>::id);
			if (it == registry.end()) return AddStorage<T>();
			return *static_cast<SharedContentStorage<T>*>(it->second.get());
		}

	public:

		SharedContentRegistry() = default;
		void LoadAllAssets();
		void LoadFile(const std::filesystem::path& path);
		template <typename T>
		ResourceHandle<T> AddItem(T* item)
		{
			auto storageIt = registry.find(TypeInfo<T>::id);
			if (storageIt == registry.end()) {
				AddStorage<T>();
			}
			return storageIt->second->CreateItem(item);
		}
		template <typename T, typename... Args>
		requires std::constructible_from<T, Args...>
		ResourceHandle<T> CreateItem (Args&&... args) {
			auto storageIt = registry.find(TypeInfo<T>::id);
			if (storageIt == registry.end()) {
				storageIt = AddStorage<T>();
			}
			auto storagePtr = storageIt->second.get();
			auto casted = static_cast<SharedContentStorage<T>*>(storagePtr);
			return casted->CreateItem(std::forward<Args>(args)...);
		}
		template <typename T, typename... Args>
		requires std::constructible_from<T, Args...>
		ResourceHandle<T> CreateItemAt (const std::filesystem::path& filename, Args&&... args) {
			auto storageIt = registry.find(TypeInfo<T>::id);
			ISharedContentStorage* storagePtr = nullptr;
			if (storageIt == registry.end()) {
				storagePtr = &AddStorage<T>();
			}
			else {
				storagePtr = storageIt->second.get();
			}
			auto casted = static_cast<SharedContentStorage<T>*>(storagePtr);
			auto handle = casted->CreateItemAt(filename, std::forward<Args>(args)...);
			uuidMap.emplace(handle.GetUUID(), ResourceHandleTypeErased(handle));
			return handle;
		}
		template <typename T>
		void* Get(ResourceHandle<T> handle) {
			auto[data, gen] = GetItemDirect(TypeInfo<T>::id, handle.id);
			if (gen != handle.gen) {
				std::string msg = std::format(
					"Failed to get resource from SharedContentReg by handle. Handle gen was {}, expected {}",
					handle.gen,
					gen
				);
				HandleError(msg);
				return nullptr;
			}
		}
		template <typename T>
		ResourceHandle<T> Load(std::filesystem::path filename) {
			std::cerr << "Unknown type loading" << std::endl;
			return ResourceHandle<T>::Invalid();
		}
		void HandleError(const std::string& msg) {
			std::cerr << msg << std::endl;
		}

		std::vector<TypeMetadata> GetResourceTypes() {
			std::vector<TypeMetadata> types;
			types.reserve(registry.size());
			for (auto& [type, arr] : registry) {
				types.emplace_back(*TypeRegistry::Get(type));
			}
			return types;
		}
		template <typename Ty, typename Fn>
		requires std::invocable<Fn&, Ty&>
		void Foreach(Fn&& fn) {
			auto it = registry.find(TypeInfo<Ty>::id);
			if (it == registry.end()) {
				return;
			}
			auto& storage = it->second;
			auto* casted = static_cast<SharedContentStorage<Ty>*>(storage.get());
			casted->Foreach(std::forward<Fn>(fn));
		}
	};

	//------DEFINITIONS--------

	template<typename T>
	bool ResourceHandle<T>::isValid() {
		if (storage == nullptr) return false;
		return storage->GetGen(id) == gen;
	}

	template<typename T>
	T* ResourceHandle<T>::operator->() {
		void* data = storage->GetItem(*this);
		return static_cast<T*>(data);
	}

	//---------LOADING---------

	template<>
	inline ResourceHandle<Texture2D> SharedContentRegistry::Load(std::filesystem::path filename) {
		return CreateItemAt<Texture2D>(filename.string(), filename.string());
	}
}