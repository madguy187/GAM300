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
            Console.WriteLine(name);
            gameObject = new GameObject(entity, name);
            gc_handle = handle;
            transform = gameObject.transform;
        }

        public T GetComponent<T>() where T : IScriptable
        {
            return gameObject.GetComponent<T>();
        }

        public void Invoke(string funcName, float time)
        {
            InvokeFunc(Entity, gameObject.ScriptName, funcName, time);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void InvokeFunc(UInt32 entity, string scriptName, string funcName, float time);
    }
}