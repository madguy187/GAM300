#pragma once
#include "Serializer.h"
#include "Deserializer.h"
#include "Reflection/Variant/RefVariant.h"

namespace Eclipse
{
	class SerializationManager
	{
		class Backup
		{
			const char* backUpPath = "Data\\Temp\\RunningTemp.scn";
			TiXmlDocument _backup;

		public:
			void SaveBackup(Serializer&);
			void LoadBackup(Deserializer&);
			const char* GetBackUpPath();
		};

		Backup backup;

		class PostUpdate
		{
			std::map<Entity, Entity> oldToNewMap;
		public:
			Entity GetNew(const Entity& oldEnt);

			void RegisterForPostUpdate(World& w, const Entity& oldEnt, const Entity& newEnt);

			void RunPostUpdate();

			void Clear();

		} pUpdate;

		template <typename CompType>
		inline void SerializeComponent(World& w, const Entity& ent)
		{
			if (w.CheckComponent<CompType>(ent))
			{
				auto index = static_cast<unsigned int>(w.GetComponentType<CompType>());
				auto name = w.GetAllComponentNames()[index];
				auto& comp = w.GetComponent<CompType>(ent);
				sz.StartElement(name);
				SerializeComponentData(RefVariant{ comp });
				sz.CloseElement();
			}
		}

		template <typename CompType>
		inline bool DeserializeComponent(World& w, const Entity& ent)
		{
			auto index = static_cast<unsigned int>(w.GetComponentType<CompType>());
			auto name = w.GetAllComponentNames()[index];
			bool isSuccess = true;

			if (dsz.StartElement(name))
			{
				CompType comp;
				if (DeserializeComponentData(RefVariant{ comp }))
				{
					w.AddComponent(ent, comp);
				}
				else
				{
					isSuccess = false;
				}
				dsz.CloseElement();
			}

			return isSuccess;
		}

		void SerializeEntity(World& w, const Entity& ent, const size_t& counter);

		Entity DeserializeEntity(World& w, const size_t& counter, bool PrefabUse = false);

		void SerializeAllComponents(World& w, const Entity& ent);

		bool DeserializeAllComponents(World& w, const Entity& ent, bool PrefabUse = false);

		void SerializeAllEntity();

		void DeserializeAllEntity();

		void SaveFile(const char* fullpath);

		bool LoadFile(const char* fullpath);

		void SavePrefab(const EUUID& prefabID, const Entity& ent, bool IsFromMainWorld = false);

		EUUID LoadPrefab(Entity& dszEnt, bool IsFromMainWorld = false);

		void SavePrefabWorld(const std::set<Entity>& entities);

		void LoadPrefabCleanUp(World& world, const Entity& ent);

		void UpdateParentChild(World& world, const Entity& parentEnt, const Entity& childEnt, bool IsFromMainWorld = false);

	public:
		static Serializer sz;
		static Deserializer dsz;

		SerializationManager();

		~SerializationManager();

		void SaveDefaultEngineConfig();

		void SaveEngineConfig(const std::string& windowName = { "Unagi Studios : Eclipse" }, const int& windowWidth = 1850,
			const int& windowHeight = 970, const bool& editorState = true, const bool& isFullscreen = false);

		void LoadEngineConfig(std::string& windowName, int& windowWidth, int& windowHeight, bool& editorState, bool& isFullscreen);

		void SaveBackupFile();

		void LoadBackupFile();

		const char* GetBackUpPath();

		bool CheckBackUpPathExistence();

		void SavePrefabFile(const EUUID& prefabID, const Entity& ent, const char* path, bool IsFromMainWorld = false);

		EUUID LoadPrefabFile(Entity& dszEnt, const char* fullpath, bool IsFromMainWorld = false);

		void SaveSceneFile(const char* fullpath = "Data/Temp/UnsavedTemp.scn");

		void LoadSceneFile(const char* fullpath = "Data/Temp/UnsavedTemp.scn");

		void SaveLayersConfigFile(const std::unordered_map<int, std::string>& LayerList,
			const size_t& UserLayerSize, const char* fullpath = "Config/Layers.cfg");

		bool LoadLayersConfigFile(std::unordered_map<int, std::string>& LayerList, 
			size_t& UserLayerSize, const char* fullpath = "Config/Layers.cfg");

		void SavePrefabWorldFile(const std::set<Entity>& entities);

		template <typename T>
		inline static void	SerializeDataMember(const char* name, RefVariant refv)
		{
			sz.StartElement(name);
			sz.AddAttributeToElement("value", refv.ValueRegistry<RemTypeQual<T>::type>());
			sz.CloseElement();
		}

		template <typename T>
		inline static bool DeserializeDataMember(const char* name, RefVariant refv)
		{
			/*bool isSuccess = false;

			if (dsz.StartElement(name))
			{
				if constexpr (!std::is_pointer<T>::value)
					dsz.ReadAttributeFromElement("value", refv.ValueRegistry<RemTypeQual<T>::type>());
				dsz.CloseElement();
				isSuccess = true;
			}*/
			bool isSuccess = true;

			if (dsz.StartElement(name))
			{
				if constexpr (!std::is_pointer<T>::value)
					dsz.ReadAttributeFromElement("value", refv.ValueRegistry<RemTypeQual<T>::type>());
				dsz.CloseElement();
			}

			return isSuccess;
		}

		template <typename T>
		inline static bool CompareDataMembers(RefVariant lhs, RefVariant rhs)
		{
			if (lhs.ValueRegistry<RemTypeQual<T>::type>() == rhs.ValueRegistry<RemTypeQual<T>::type>())
				return true;

			return false;
		}

		inline static void SerializeComponentData(RefVariant data)
		{
			const MetaData* meta = data.Meta();

			if (meta->HasMembers())
			{
				const Member* mem = meta->Members();

				while (mem)
				{
					void* offsetData = PTR_ADD(data.Data(), mem->Offset());
					mem->Meta()->Serialize(mem->GetName().c_str(), RefVariant(mem->Meta(), offsetData));
					mem = mem->Next();
				}
			}
		}

		inline static bool DeserializeComponentData(RefVariant data)
		{
			bool IsSuccess = false;
			const MetaData* meta = data.Meta();

			if (meta->HasMembers())
			{
				const Member* mem = meta->Members();

				while (mem)
				{
					void* offsetData = PTR_ADD(data.Data(), mem->Offset());
					IsSuccess = mem->Meta()->Deserialize(mem->GetName().c_str(), RefVariant(mem->Meta(), offsetData));

					if (!IsSuccess)
						return false;

					mem = mem->Next();
				}
			}

			return true;
		}

		inline static bool CompareComponentData(RefVariant lhs, RefVariant rhs, bool isChild = false)
		{
			(void)isChild;
			bool IsSuccess = false;
			const MetaData* LHSmeta = lhs.Meta();
			const MetaData* RHSmeta = rhs.Meta();

			if (LHSmeta->HasMembers() && RHSmeta->HasMembers())
			{
				const Member* LHSmem = LHSmeta->Members();
				const Member* RHSmem = RHSmeta->Members();

				while (LHSmem && RHSmem)
				{
					void* LHSoffsetData = PTR_ADD(lhs.Data(), LHSmem->Offset());
					void* RHSoffsetData = PTR_ADD(rhs.Data(), RHSmem->Offset());

					IsSuccess = LHSmem->Meta()->Compare(RefVariant(LHSmem->Meta(), LHSoffsetData),
						RefVariant(RHSmem->Meta(), RHSoffsetData));

					if (!IsSuccess)
						return false;

					LHSmem = LHSmem->Next();
					RHSmem = RHSmem->Next();
				}
			}

			return true;
		}

		inline static  bool CompareComponentData(const EntityComponent& lhs, const EntityComponent& rhs, bool isChild = false)
		{
			(void)isChild;
			bool result = true;
			result = (lhs.Tag == rhs.Tag) && result;
			result = (lhs.LayerIndex == rhs.LayerIndex) && result;
			return result;
		}
		
		inline static  bool CompareComponentData(const TransformComponent& lhs, const TransformComponent& rhs, bool isChild = false)
		{
			bool result = true;
			if (isChild)
			{
				result = (lhs.position == rhs.position) && result;
			}
			result = (lhs.rotation == rhs.rotation) && result;
			result = (lhs.scale == rhs.scale) && result;
			return result;
		}
		
		inline static  bool CompareComponentData(const ParentComponent&, const ParentComponent&, bool isChild = false)
		{
			(void)isChild;
			return  true;
		}
		
		inline static  bool CompareComponentData(const PrefabComponent&, const PrefabComponent&, bool isChild = false)
		{
			(void)isChild;
			return true;
		}

		inline static  bool CompareComponentData(const ChildComponent& lhs, const ChildComponent& rhs, bool isChild = false)
		{
			(void)isChild;
			bool result = true;
			result = (lhs.PosOffset == rhs.PosOffset) && result;
			result = (lhs.RotOffset == rhs.RotOffset) && result;
			result = (lhs.ScaleOffset == rhs.ScaleOffset) && result;
			return result;
		}

		inline static  bool CompareComponentData(const AIComponent& lhs, const AIComponent& rhs, bool isChild = false)
		{
			(void)isChild;
			bool result = true;
			result = (lhs.MinDisttoChange == rhs.MinDisttoChange) && result;
			result = (lhs.patrolling == rhs.patrolling) && result;
			result = (lhs.PatrolSpeed == rhs.PatrolSpeed) && result;
			return result;
		}

		template <typename... T>
		void SerializeListedComponent(World& w, const Entity& ent, TypeList<T...>)
		{
			((SerializeComponent<T>(w, ent)), ...);
		}

		template <typename T>
		bool DeserializeListedComponent(World& w, const Entity& ent, TypeList<T>)
		{
			bool result = false;
			if (DeserializeComponent<T>(w, ent))
			{
				result = true;
			}
			return result; 
		}

		template <typename T, typename... Args>
		bool DeserializeListedComponent(World& w, const Entity& ent, TypeList<T, Args...>)
		{
			bool result = false;
			if (DeserializeComponent<T>(w, ent))
			{
				TypeList<Args...> remaining;
				result = DeserializeListedComponent(w, ent, remaining);
			}
			return result;
		}
	};
}