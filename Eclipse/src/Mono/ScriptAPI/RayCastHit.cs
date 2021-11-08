using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public class RayCastHit
    {
        public Vector3 point;

        public RayCastHit(float x, float y, float z)
        {
            point = new Vector3(x, y, z);
        }
    }
}