#pragma once
#include <vector>
#include "Math.cpp"
#include <type_traits>
#include <utility>
#include <cassert>
#include "TypeDecl.h"
#include "Graphics.h"
#include "debugdefs.h"

namespace DynamicVertex {
	typedef colours::BGRAColour BGRAColour;

	struct VertexLayout
	{
		enum MemberType
		{
			Position2D,
			Position3D,
			Float3Colour,
			Float4Colour,
			Texture2D,
			BGRAColour,
			Normal,

			Count,
		};
		template<MemberType> struct Map;
		template<> struct Map<Position2D>
		{
			using SysType = float2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const wchar_t* id = L"POS2";
		};
		template<> struct Map<Position3D>
		{
			using SysType = float3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Position";
			static constexpr const wchar_t* id = L"POS3";
		};
		template<> struct Map<Texture2D>
		{
			using SysType = float2;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32_FLOAT;
			static constexpr const char* semantic = "Texcoord";
			static constexpr const wchar_t* id = L"TEX2";
		};
		template<> struct Map<Float3Colour>
		{
			using SysType = float3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Colour";
			static constexpr const wchar_t* id = L"COL3";
		};
		template<> struct Map<Float4Colour>
		{
			using SysType = float4;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32A32_FLOAT;
			static constexpr const char* semantic = "Colour";
			static constexpr const wchar_t* id = L"COL4";
		};
		template<> struct Map<Normal>
		{
			using SysType = float3;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R32G32B32_FLOAT;
			static constexpr const char* semantic = "Normal";
			static constexpr const wchar_t* id = L"NOR";
		};
		template<> struct Map<BGRAColour>
		{
			using SysType = DynamicVertex::BGRAColour;
			static constexpr DXGI_FORMAT dxgiFormat = DXGI_FORMAT_R8G8B8A8_UNORM;
			static constexpr const char* semantic = "Colour";
			static constexpr const wchar_t* id = L"COL8";
		};

		struct Member
		{
		public:
			Member(MemberType type, unsigned int offset)
				:
				memberType(type),
				offset(offset)
			{}
			unsigned int FetchOffsetAfterCurrent() const noexcept_unless
			{
				return (offset + size());
			}
			unsigned int FetchOffset() const
			{
				return offset;
			}
			unsigned int size() const noexcept_unless
			{
				return size_BYTES(memberType);
			}
			const wchar_t* FetchID() const noexcept
			{
				switch (memberType)
				{
				case Position2D:
					return Map<Position2D>::id;
				case Texture2D:
					return Map<Texture2D>::id;
				case Position3D:
					return Map<Position3D>::id;
				case Float3Colour:
					return Map<Float3Colour>::id;
				case Normal:
					return Map<Normal>::id;
				case Float4Colour:
					return Map<Float4Colour>::id;
				case BGRAColour:
					return Map<BGRAColour>::id;
				}
				assert(L"Unrecognised element type called" && false);
				return L"Unrecognised element";
				
			}
			static constexpr unsigned int size_BYTES(MemberType type) noexcept_unless
			{
				switch (type)
				{
				case Position2D:
					return sizeof(Map<Position2D>::SysType);
				case Texture2D:
					return sizeof(Map<Texture2D>::SysType);
				case Position3D:
					return sizeof(Map<Position3D>::SysType);
				case Float3Colour:
					return sizeof(Map<Float3Colour>::SysType);
				case Normal:
					return sizeof(Map<Normal>::SysType);
				case Float4Colour:
					return sizeof(Map<Float4Colour>::SysType);
				case BGRAColour:
					return sizeof(Map<BGRAColour>::SysType);
				}
				assert("element type not identified" && false);
				return 0u;
			}

			D3D11_INPUT_ELEMENT_DESC FetchDescription() const noexcept_unless
			{
				switch (memberType)
				{
				case Position2D:
					return GenerateD3DDesc<Position2D>(FetchOffset());
				case Position3D:
					return GenerateD3DDesc<Position3D>(FetchOffset());
				case Texture2D:
					return GenerateD3DDesc<Texture2D>(FetchOffset());
				case Float3Colour:
					return GenerateD3DDesc<Float3Colour>(FetchOffset());
				case Float4Colour:
					return GenerateD3DDesc <Float4Colour> (FetchOffset());
				case Normal:
					return GenerateD3DDesc<Normal>(FetchOffset());
				case BGRAColour:
					return GenerateD3DDesc<BGRAColour>(FetchOffset());
				}
				assert("element type is invalid" && false);
				return { "INVALID",0,DXGI_FORMAT_UNKNOWN,0,0,D3D11_INPUT_PER_VERTEX_DATA,0 };
			}
		private:
			template<MemberType memtype>
			static constexpr D3D11_INPUT_ELEMENT_DESC GenerateD3DDesc(uint32_t offset) noexcept
			{
				return{ Map<memtype>::semantic,0,Map<memtype>::dxgiFormat,0,static_cast<uint32_t>(offset), D3D11_INPUT_PER_VERTEX_DATA,0 };
			}
		public:
			MemberType FetchType() const noexcept
			{
				return memberType;
			}
		private:
			MemberType memberType;
			unsigned int offset;
		};
	public:
		template<MemberType elem_t>
		const Member& Store() const noexcept_unless
		{
			for (const VertexLayout::Member& e : elements)
			{
				if (e.FetchType() == elem_t)
				{
					return e;
				}
			}
			assert("Could not resolve element type" && false);
			return elements.front();
		}
		const Member& ResolveByIndex(unsigned int idx) const noexcept_unless
		{
			return elements[idx];
		}
		VertexLayout& Emplace_Back(MemberType memtype) noexcept_unless
		{
			elements.emplace_back(memtype, size());
			return *this;
		}
		unsigned int size() const noexcept_unless
		{
			if (elements.empty())
			{
				return 0u;
			}
			else
			{
				return elements.back().FetchOffsetAfterCurrent();
			}
		}
		unsigned int FetchMemberCount() const noexcept
		{
			return elements.size();
		}
		std::vector<D3D11_INPUT_ELEMENT_DESC> FetchD3DLayout() const noexcept_unless
		{
			std::vector<D3D11_INPUT_ELEMENT_DESC> description;
			description.reserve(FetchMemberCount());
			for (const auto& e : elements)
			{
				description.emplace_back(e.FetchDescription());
			}

			return description;
		}
		std::wstring FetchID() const noexcept_unless
		{
			std::wstring id;
			for (const auto& elem : elements)
			{
				id += elem.FetchID();
			}return id;
		}
	private:
		std::vector<Member> elements;
	};

	struct Vertex //vertex accessor
	{
	private:
		friend struct VertexBuffer;
	public:
		template<VertexLayout::MemberType elem_t>
		auto& Attribute() noexcept_unless;
		template<typename T>
		void SetAttributeByIndex(unsigned int idx, T&& val) noexcept_unless;
	protected:
		Vertex(const VertexLayout& layout, char* pVertexData) noexcept_unless
			:
			pVertexData(pVertexData),
			layout(layout)
		{
			assert(pVertexData != nullptr);
		}
	private:
		// helper to reduce code duplication in SetAttributeByIndex
		template<VertexLayout::MemberType DestinationLayout_t, typename Source_t>
		void ApplyNewAttribute(char* pAttribute, Source_t&& val) noexcept_unless
		{
			using Destination = typename VertexLayout::Map<DestinationLayout_t>::SysType;
			if constexpr (std::is_assignable<Destination, Source_t>::value)
			{
				*reinterpret_cast<Destination*>(pAttribute) = val;
			}
			else
			{
				assert("mismatch of Parameter attribute type" && false);
			}
		}
		template<typename First, typename ...Others>
		void SetAttributeByIndex(unsigned int idx, First&& first, Others&&... others) noexcept_unless
		{
			//template recursion
			SetAttributeByIndex(idx, std::forward<First>(first));
			SetAttributeByIndex(idx + 1, std::forward<Others>(others)...);
		}
	private:
		const VertexLayout& layout;
		char* pVertexData = nullptr;
	};

	struct ConstantVertex
	{
		ConstantVertex(const Vertex& _vertex) noexcept_unless
			:vertex(_vertex)
		{
		}
		template<VertexLayout::MemberType Elem_t>
		const auto& Attribute() const noexcept_unless
		{
			return const_cast<Vertex&>(vertex).Attribute<Elem_t>();
		}
	private:
		Vertex vertex;
	};

	struct VertexBuffer
	{
	public:
		VertexBuffer(VertexLayout layout, UINT size = NULLUNSIGNED) noexcept_unless
			: layout(std::move(layout)) {
			AdjustBufferLength(size);
		}

		void AdjustBufferLength(UINT size_)
		{
			const UINT current_size = VertexBuffer::size();
			if (current_size < size_) {
				int new_size = buffer.size() + layout.size() * ( size_ - current_size);
				buffer.resize(new_size); 
			}

		}
		const VertexLayout& FetchLayout() const noexcept
		{
			return layout;
		}
		const char* FetchData() const noexcept_unless
		{
			return buffer.data();
		}
		unsigned int size() const noexcept_unless
		{
			return buffer.size() / layout.size();
		}
		uint32_t size_bytes() const noexcept_unless
		{
			return buffer.size();
		}
		template<typename ...Args>
		void Emplace_Back(Args&&... arguments) noexcept_unless
		{
			assert(sizeof...(arguments) == layout.FetchMemberCount() &&
				"Argument count not matching with number of elements in vertex");
			buffer.resize(buffer.size() + layout.size());
			FetchBack().SetAttributeByIndex(0u, std::forward<Args>(arguments)...);
		}
		Vertex FetchBack() noexcept_unless
		{
			assert(buffer.size() != 0u);

			return Vertex{ layout, buffer.data() + buffer.size() - layout.size() };
		}
		Vertex FetchFront() noexcept_unless
		{
			assert(buffer.size() != 0u);
			return Vertex{ layout, buffer.data() };
		}
		Vertex operator[](unsigned int idx) noexcept_unless
		{
			assert(idx < size());
			return Vertex{ layout, buffer.data() + layout.size() * idx };
		}
		ConstantVertex FetchBack() const noexcept_unless
		{
			return const_cast<VertexBuffer*>(this)->FetchBack();
		}
		ConstantVertex FetchFront() const noexcept_unless
		{
			return const_cast<VertexBuffer*>(this)->FetchFront();
		}
		ConstantVertex operator[](unsigned int idx) const noexcept_unless
		{
			return const_cast<VertexBuffer&>(*this)[idx];
		}
	private:
		std::vector<char> buffer;
		VertexLayout layout;
	};


	template<VertexLayout::MemberType elem_t>
	inline auto& Vertex::Attribute() noexcept_unless
	{
		auto pAttribute = pVertexData + layout.Store<elem_t>().FetchOffset();
		return *reinterpret_cast<typename VertexLayout::Map<elem_t>::SysType*>(pAttribute);
	}

	template<typename T>
	inline void Vertex::SetAttributeByIndex(unsigned int idx, T&& val)/*universal ref*/ noexcept_unless
	{
		const auto& member = layout.ResolveByIndex(idx);
		auto pAttribute = pVertexData + member.FetchOffset();
		// find type and apply it depending on its case
		switch (member.FetchType())
		{
		case VertexLayout::Position2D:
			ApplyNewAttribute<VertexLayout::Position2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Texture2D:
			ApplyNewAttribute<VertexLayout::Texture2D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Position3D:
			ApplyNewAttribute<VertexLayout::Position3D>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float3Colour:
			ApplyNewAttribute<VertexLayout::Float3Colour>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Normal:
			ApplyNewAttribute<VertexLayout::Normal>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::Float4Colour:
			ApplyNewAttribute<VertexLayout::Float4Colour>(pAttribute, std::forward<T>(val));
			break;
		case VertexLayout::BGRAColour:
			ApplyNewAttribute<VertexLayout::BGRAColour>(pAttribute, std::forward<T>(val));
			break;
		default:
			assert("element type not found" && false);
		}
	}
}