using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public static class Time
    {
        public static float deltaTime
        {
            get => getDeltaTime();
        }

        public static float fixedDeltaTime
        {
            get => getFixedDeltaTime();
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getDeltaTime();

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static float getFixedDeltaTime();
    }
}