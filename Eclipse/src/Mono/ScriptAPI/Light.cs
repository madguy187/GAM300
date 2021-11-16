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

        public float Intensity
        {
            set => SetIntensity(Entity, value);
            get => GetIntensity(Entity);
        }

        public Vector3 Direction
        {
            set => SetDirection(Entity, value);
        }

        public float Decrease
        {
            set => DecreaseIntensity(Entity, value);
        }

        public float Increase
        {
            set => IncreaseIntensity(Entity, value);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetLightEnabled(UInt32 ent, int state);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetIntensity(UInt32 ent, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void SetDirection(UInt32 ent, Vector3 value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void IncreaseIntensity(UInt32 ent, float value);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void DecreaseIntensity(UInt32 ent, float value);
        
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float GetIntensity(UInt32 ent);
    }
}