using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public static class Input
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool GetKeyDown(KeyCode Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static bool GetKey(KeyCode Key);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static float GetAxis(string Axis);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static float GetAxisRaw(string Axis);

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        public extern static float LockCamera(int state);
    }
}