using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public static class Physics
    {
        public static bool Raycast(Vector3 origin, Vector3 dir, out RayCastHit hit)
        {
            hit = RaycastCheck(origin, dir);
            if (hit == null) return false;

            return true;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static RayCastHit RaycastCheck(Vector3 origin, Vector3 dir);
    }
}