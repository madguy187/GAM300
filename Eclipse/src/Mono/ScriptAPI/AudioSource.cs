using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public struct AudioSource
    {
        UInt32 Entity;

        public AudioSource(UInt32 entity)
        {
            Entity = entity;
        }

        public void Play()
        {
            PlayAudio(Entity);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void PlayAudio(UInt32 ent);
    }
}