using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class Vector3
    {
        static Vector3()
        {
            zero = new Vector3(0, 0, 0);
        }

        public Vector3(float _x, float _y, float _z)
        {
            x = _x;
            y = _y;
            z = _z;
        }

        public float x;
        public float y;
        public float z;

        public static readonly Vector3 zero;
    }
}