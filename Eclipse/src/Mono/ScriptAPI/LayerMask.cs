using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public class LayerMask
    {
        public int mask;

        public LayerMask(string binaryString)
        {
            mask = Convert.ToInt32(binaryString, 2);
        }
    }
}