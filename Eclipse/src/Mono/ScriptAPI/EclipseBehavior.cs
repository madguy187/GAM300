using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class EclipseBehavior : IScriptable
    {
        public GameObject gameObject;
        UInt32 gc_handle;

        protected void InitBehavior(UInt32 handle, UInt32 entity)
        {
            gameObject = new GameObject();
            gc_handle = handle;
            Entity = entity;
        }

        public UInt32 Entity
        {
            get => gameObject.Entity;
            set => gameObject.Entity = value;
        }
    }
}