#pragma once
#include "Global.h"
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

		class ParentChildPostUpdate
		{
			std::map<Entity, Entity> oldToNewMap;
		public:
			Entity GetNew(const Entity& oldEnt);

			void RegisterForPostUpdate(World& w, const Entity& oldEnt, const Entity& newEnt);

			void PostUpdate();

			void Clear();

		} PCPostUpdate;

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

		void SavePrefab(const EUUID& prefabID, const Entity& ent);

		EUUID LoadPrefab(Entity& dszEnt, bool IsFromMainWorld = false);

		void SavePrefabWorld(const std::set<Entity>& entities);

		void LoadPrefabCleanUp(World& world, const Entity& ent);

		void UpdateParentChild(World& world, const Entity& parentEnt, const Entity& childEnt);

	public:
		static Serializer sz;
		static Deserializer dsz;

		SerializationManager();

		~SerializationManager();

		void SaveBackupFile();

		void LoadBackupFile();

		const char* GetBackUpPath();

		bool CheckBackUpPathExistence();

		void SavePrefabFile(const EUUID& prefabID, const Entity& ent, const char* path);

		EUUID LoadPrefabFile(Entity& dszEnt, const char* fullpath, bool IsFromMainWorld = false);

		void SaveSceneFile(const char* fullpath = "Data/Temp/UnsavedTemp.scn");

		void LoadSceneFile(const char* fullpath = "Data/Temp/UnsavedTemp.scn");

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
			bool isSuccess = false;

			if (dsz.StartElement(name))
			{
				if constexpr (!std::is_pointer<T>::value)
					dsz.ReadAttributeFromElement("value", refv.ValueRegistry<RemTypeQual<T>::type>());
				dsz.CloseElement();
				isSuccess = true;
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

		inline static bool CompareComponentData(RefVariant lhs, RefVariant rhs)
		{
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

		inline bool CompareComponentData(const EntityComponent& lhs, const EntityComponent& rhs)
		{
			bool result = true;
			result = (lhs.Tag == rhs.Tag);
			result = (lhs.IsAChild == rhs.IsAChild);
			return result;
		}
		
		inline bool CompareComponentData(const TransformComponent& lhs, const TransformComponent& rhs)
		{
			bool result = true;
			result = (lhs.rotation == rhs.rotation);
			result = (lhs.scale == rhs.scale);
			return result;
		}
		
		inline bool CompareComponentData(const ParentComponent& lhs, const ParentComponent& rhs)
		{
			bool result = true;
			result = (lhs.model == rhs.model);
			return result;
		}

		inline bool CompareComponentData(const ChildComponent& lhs, const ChildComponent& rhs)
		{
			bool result = true;
			result = (lhs.PosOffset == rhs.PosOffset);
			result = (lhs.RotOffset == rhs.RotOffset);
			result = (lhs.ScaleOffset == rhs.ScaleOffset);
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