using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class RigidBody : IScriptable
    {
        public UInt32 Entity;

        public RigidBody(UInt32 entity)
        {
            Entity = entity;
        }
        
        public void AddForce(float x, float y, float z)
        {
            Add_Force(Entity, x, y, z);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void Add_Force(UInt32 entity, float x, float y, float z);
    }
}