using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public static class Cursor
    {
        public static CursorLockMode lockState
        {
            set => setState((int)value);
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static void setState(int state);
    }
}