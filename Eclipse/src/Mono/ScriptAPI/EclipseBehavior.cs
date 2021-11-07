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

        protected void InitBehavior(UInt32 handle, UInt32 entity, string name)
        {
            Console.WriteLine(name);
            gameObject = new GameObject(entity, name);
            gc_handle = handle;
            transform = gameObject.transform;
        }

        public UInt32 Entity
        {
            get => gameObject.Entity;
            set => gameObject.Entity = value;
        }

        public T GetComponent<T>() where T : IScriptable
        {
            return gameObject.GetComponent<T>();
        }
    }
}