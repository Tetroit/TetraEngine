#pragma once

namespace TetraEngine {

	typedef size_t type_id;

	template <typename T>
	struct TypeInfo
	{
		static constexpr type_id id = 0;
		static constexpr const char* name = "Empty";
	};

	class Texture2D;
	template<>
	struct TypeInfo<Texture2D>
	{
		static constexpr type_id id = 1;
		static constexpr const char* name = "Texture2D";
	};

	class Material;
	template<>
	struct TypeInfo<Material>
	{
		static constexpr type_id id = 2;
		static constexpr const char* name = "Material";
	};

	class ViewportCamera;
	template<>
	struct TypeInfo<ViewportCamera>
	{
		static constexpr type_id id = 3;
		static constexpr const char* name = "Camera";
	};
}

