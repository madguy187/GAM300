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
            Console.WriteLine("Crash3");
            return GetRigidComponent(Entity) as T;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static RigidBodyComponent GetRigidComponent(UInt32 entity);
    }
}