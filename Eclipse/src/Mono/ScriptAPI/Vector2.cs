using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class Vector2
    {
        static Vector2()
        {
            zero = new Vector2(0, 0);
            forward = new Vector2(0, 0);
            up = new Vector2(0, 1);
        }

        public Vector2(float _x, float _y)
        {
            x = _x;
            y = _y;
        }

        public float x;
        public float y;

        public static readonly Vector2 zero;
        public static readonly Vector2 forward;
        public static readonly Vector2 up;

        public float magnitude
        {
            get => x * x + y * y;
        }

        public static Vector2 operator *(Vector2 a, Vector2 b)
        {
            return new Vector2(a.x * b.x, a.y * b.y);
        }

        public override string ToString() => $"{x} {y}";
    }
}