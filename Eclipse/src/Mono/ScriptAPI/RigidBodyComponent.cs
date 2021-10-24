using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class RigidBodyComponent : IScriptable
    {
        public UInt32 Entity;

        public void InitComponent(UInt32 entity)
        {
            Entity = entity;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void Add_Force(UInt32 entity, float x, float y, float z);

        public void AddForce(float x, float y, float z)
        {
            Add_Force(Entity, x, y, z);
        }
    }
}