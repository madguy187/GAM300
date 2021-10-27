using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class GameObject
    {
        public UInt32 Entity;

        public GameObject(UInt32 entity)
        {
            Entity = entity;
        }

        public T GetComponent<T>() where T : IScriptable
        {
            return (T)Activator.CreateInstance(typeof(T), new object[] { Entity });
        }
    }
}