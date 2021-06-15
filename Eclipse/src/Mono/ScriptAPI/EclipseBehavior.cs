using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class EclipseBehavior : IScriptable
    {
        public GameObject gameObject;

        public UInt32 Entity
        {
            get { gameObject.Entity; }
            set { gameObject.Entity = value; }
        }
    }
}