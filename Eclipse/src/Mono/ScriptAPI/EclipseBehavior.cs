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

        protected void InitBehavior(UInt32 handle, UInt32 entity)
        {
            gameObject = new GameObject(entity);
            gc_handle = handle;
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