using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class Light
    {
        UInt32 Entity;

        public Light(UInt32 entity)
        {
            Entity = entity;
        }

        public bool enabled
        {
            set => SetLightEnabled(value ? 1 : 0);
        }

        public static Vector3 operator *(Vector3 a, Vector3 b)
        {
            return new Vector3(a.x * b.x, a.y * b.y, a.z * b.z);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetLightEnabled(int state);
    }
}