#pragma once
#include "isBinded.h"
#include "isBinded_Storage.h"
#include <memory>
#include <unordered_map>
#include "Converter.h"
#include <type_traits>

namespace Bind //Bind injection
{
	struct Repository
	{
	private:
		std::unordered_map<std::wstring, std::shared_ptr<isBinded>> allbinds;
	public:
		template<class T, typename...Args>
		static std::shared_ptr<T> Store(Graphics& gfx, Args&&...args) noexcept_unless
		{
			//check that all attributes are predecessors to type isBinded
			static_assert(std::is_base_of<isBinded, T>::value, "only stores classes derived from isBinded");
			//forward the arguments (similar to move) to the Store_ function
			return Fetch().Store_<T>(gfx, std::forward<Args>(args)... );
		}
	private:
		template<class T, typename...Args> // plural type
		std::shared_ptr<T> Store_(Graphics& gfx, Args&&...args) noexcept_unless // 
		{
			const std::wstring key = T::ConstructUID(std::forward<Args>(args)...);/*
																				  find key of the first argument
																				  to index bind
																				  */
			const auto iterator_bind = allbinds.find(key); //stores the current first argument to be added to the hash map
			bool isfinal_key = iterator_bind == allbinds.end();
			if (isfinal_key) // if the iterator is at the final element (meaning the bind does not currently exist in the repository)
			{
				std::shared_ptr<T> bind = std::make_shared<T>(gfx, std::forward<Args>(args)...); /*
																								 create bind using the arguments passed in
																								 i.e., binds to be stored by the repo for rendering
																								 */
				allbinds[key] = bind; // add bind to hash map
				return bind; // return bind
			}
			else
			{
				// create a shared instance of the bind-derived entity
				return  std::static_pointer_cast<T>( iterator_bind->second );
			}

			
		}
		static Repository& Fetch()
		{
			static Repository repo;
			return repo; // return repo entity
		}
	};
}