using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public class RaycastHit
    {
        public Vector3 point;
        public UInt32 Entity;

        public RaycastHit(UInt32 ent, float x, float y, float z)
        {
            point = new Vector3(x, y, z);
            Entity = ent;
        }
    }
}