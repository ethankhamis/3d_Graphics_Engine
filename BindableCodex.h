#pragma once
#include "Bindable.h"
#include "BindableCodex.h"
#include <memory>
#include <unordered_map>
#include "Converter.h"
#include <type_traits>

namespace Bind //Bind injection
{
	struct Codex
	{
	private:
		std::unordered_map<std::wstring, std::shared_ptr<Bindable>> allbinds;
	public:
		template<class T, typename...Args>
		static std::shared_ptr<T> Resolve(Graphics& gfx, Args&&...args) noexcept_unless
		{
			static_assert(std::is_base_of<Bindable, T>::value, "only resolves classes derived from Bindable");
			return Fetch().Resolve_<T>(gfx, std::forward<Args>(args)... );
		}
	private:
		template<class T, typename...Args>
		std::shared_ptr<T> Resolve_(Graphics& gfx, Args&&...args) noexcept_unless
		{
			const std::wstring key = T::ConstructUID(std::forward<Args>(args)...);
			const auto iterator_bind = allbinds.find(key);
			if (iterator_bind == allbinds.end())
			{
				std::shared_ptr<T> bind = std::make_shared<T>(gfx, std::forward<Args>(args)...);
				allbinds[key] = bind;
				return bind;
			}
			else
			{
				return  std::static_pointer_cast<T>( iterator_bind->second );
			}

			
		}
		static Codex& Fetch()
		{
			static Codex codex;
			return codex;
		}
	};
}