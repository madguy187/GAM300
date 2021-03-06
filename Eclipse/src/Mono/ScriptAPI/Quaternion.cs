using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class Quaternion
    {
        static Quaternion()
        {
            identity = new Quaternion(1, 0, 0, 0);
        }

        public Quaternion(float _w, float _x, float _y, float _z)
        {
            w = _w;
            x = _x;
            y = _y;
            z = _z;
        }

        public float w;
        public float x;
        public float y;
        public float z;

        public static readonly Quaternion identity;

        public static Quaternion Euler(float x, float y, float z)
        {
            return GetEuler(x, y, z);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static Quaternion GetEuler(float x, float y, float z);

        public override string ToString() => $"{w} {x} {y} {z}";
    }
}