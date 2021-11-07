using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class GameObject
    {
        public UInt32 Entity;
        public Transform transform;
        public string ScriptName;

        public GameObject(UInt32 entity, string name)
        {
            Entity = entity;
            ScriptName = name;
            transform = new Transform(Entity);
        }

        public T GetComponent<T>() where T : IScriptable
        {
            return (T)Activator.CreateInstance(typeof(T), new object[] { Entity });
        }

        public T GetBehavior<T>() where T : IScriptable
        {
            return getBehavior(Entity, typeof(T).Name) as T;
        }

        public bool enabled
        {
            set => setEnabled(Entity, ScriptName,  0);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setEnabled(UInt32 entity, string name, int value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static GameObject getBehavior(UInt32 entity, string behaviorName);
    }
}