using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class PhysicsObject
    {
        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        extern static void AddForce();
    }
}