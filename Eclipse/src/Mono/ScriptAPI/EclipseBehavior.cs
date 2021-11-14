using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public class EclipseBehavior : IScriptable
    {
        private UInt32 gc_handle;
        public GameObject gameObject;
        public Transform transform;

        public UInt32 Entity
        {
            get => gameObject.Entity;
            set => gameObject.Entity = value;
        }

        protected void InitBehavior(UInt32 handle, UInt32 entity, string name)
        {
            gameObject = new GameObject(entity, name);
            gc_handle = handle;
            transform = gameObject.transform;
        }

        public T GetComponent<T>() where T : IScriptable
        {
            if (typeof(T).IsSubclassOf(typeof(EclipseBehavior)))
              return gameObject.GetBehavior<T>();
            else
              return gameObject.GetComponent<T>();
        }

        public void Invoke(string funcName, float time)
        {
            InvokeFunc(Entity, gameObject.ScriptName, funcName, time);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static GameObject CreateSpotLight(Vector3 pos, Vector3 direction);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static GameObject Find(string entName);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void InvokeFunc(UInt32 entity, string scriptName, string funcName, float time);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static GameObject CreatePrefab(string prefabName, Vector3 pos, Vector3 rot);
    }
}