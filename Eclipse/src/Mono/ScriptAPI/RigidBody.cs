using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class RigidBody : IScriptable
    {
        public RigidBody(UInt32 entity)
        {
            Entity = entity;
        }

        public UInt32 Entity;

        public float mass
        {
            get => getMass(Entity);
        }

        public float x
        {
            get => getX(Entity);
        }

        public float y
        {
            get => getY(Entity);
        }

        public float z
        {
            get => getZ(Entity);
        }
        
        public void AddForce(float x, float y, float z)
        {
            Add_Force(Entity, x, y, z);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void Add_Force(UInt32 entity, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getMass(UInt32 entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getX(UInt32 entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getY(UInt32 entity);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getZ(UInt32 entity);
    }
}