using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public struct Light
    {
        public UInt32 Entity;

        public Light(UInt32 entity)
        {
            Entity = entity;
        }

        public bool enabled
        {
            set => SetLightEnabled(Entity, value ? 1 : 0);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetLightEnabled(UInt32 ent, int state);
    }
}