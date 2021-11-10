using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public static class Physics
    {
        public static bool Raycast(Vector3 origin, Vector3 dir, out RaycastHit hit, float dist = 0, int mask = 1)
        {
            hit = RaycastCheck(origin, dir, dist, Convert.ToString(mask, 2));
            if (hit == null) return false;

            return true;
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static RaycastHit RaycastCheck(Vector3 origin, Vector3 dir, float dist, string mask);
    }
}