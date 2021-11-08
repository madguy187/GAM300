using System;
using System.Runtime.CompilerServices;
using System.Numerics;

namespace Eclipse
{
    public struct Transform
    {
        UInt32 Entity;
        //Vector3 position;
        //Quaternion rotation;

        public Transform(UInt32 entity)
        {
            Entity = entity;
        }

        public void Rotate(Vector3 angle)
        {
            RotateEuler(Entity, angle.x, angle.y, angle.z);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static void RotateEuler(UInt32 ent, float x, float y, float z);

        //[MethodImplAttribute(MethodImplOptions.InternalCall)]
        //public extern static Vector3 GetScale(Entity ent);
    }
}