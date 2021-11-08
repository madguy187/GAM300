using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public static class Physics
    {
        public static bool Raycast(Vector3 origin, Vector3 dir)
        {
            return RaycastCheck(origin, dir);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static bool RaycastCheck(Vector3 origin, Vector3 dir);
    }
}