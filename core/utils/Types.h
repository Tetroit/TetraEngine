#pragma once
#include "tetrapc.h"

#define TETRA_REGISTER_TYPE(T, STR) \
template<> struct TypeInfo<T> { \
	static constexpr const char* name = STR; \
	static constexpr asset_type_id id = fnv1a_64(STR); \
};

namespace TetraEngine {

	constexpr asset_type_id fnv1a_64(const char* str)
	{
		asset_type_id hash = 1469598103934665603ull;

		while (*str)
		{
			hash ^= (asset_type_id)(*str++);
			hash *= 1099511628211ull;
		}

		return hash;
	}

	template<typename T>
	struct TypeInfo {
		static constexpr const char* name = "unknown";
		static constexpr asset_type_id id = 0;
	};

	class Texture2D;
	TETRA_REGISTER_TYPE(Texture2D, "Texture2D")
	class Material;
	TETRA_REGISTER_TYPE(Material, "Material")
	class Shader;
	TETRA_REGISTER_TYPE(Shader, "Shader")


	struct TypeMetadata {
		asset_type_id id;
		size_t size;
		const char* name;
	};

	class TypeRegistry {
		std::unordered_map<asset_type_id, TypeMetadata> map;

	public:
		template <typename T>
		void Register() {
			size_t CTId = TypeInfo<T>::id;
			if (CTId == 0) {
				size_t id = fnv1a_64(typeid(T).name());
				map.emplace(id, TypeMetadata{
					id, sizeof(T), typeid(T).name()
				});
			}
			else {
				map.emplace(CTId, TypeMetadata{
					CTId,
					sizeof(T),
					TypeInfo<T>::name
				});
			}
		}
		[[nodiscard]] const TypeMetadata* Get_(asset_type_id id) const {
			auto it = map.find(id);
			if (it == map.end())
				return nullptr;
			return &it->second;
		}
		static const TypeMetadata* Get(asset_type_id id);
	};
}

