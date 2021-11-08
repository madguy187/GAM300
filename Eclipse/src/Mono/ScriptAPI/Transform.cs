using System;
using System.Runtime.CompilerServices;
using System.Numerics;

namespace Eclipse
{
    public struct Transform
    {
        public Transform(UInt32 entity)
        {
            Entity = entity;
        }

        UInt32 Entity;

        public Vector3 position
        {
            get => GetTransform(Entity);
            set => SetTransform(Entity, value);
        }
        
        public Quaternion rotation
        {
            get => GetRotation(Entity);
            set => SetRotation(Entity, value);
        }

        public void Rotate(Vector3 angle)
        {
            RotateEuler(Entity, angle.x, angle.y, angle.z);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateEuler(UInt32 ent, float x, float y, float z);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Vector3 GetTransform(UInt32 ent);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetTransform(UInt32 ent, Vector3 pos);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Quaternion GetRotation(UInt32 ent);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetRotation(UInt32 ent, Quaternion rot);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern static Vector3 GetScale(Entity ent);
    }
}