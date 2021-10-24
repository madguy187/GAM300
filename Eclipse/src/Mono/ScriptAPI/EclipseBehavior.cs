using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;

namespace Eclipse
{
    public class EclipseBehavior : IScriptable
    {
        public GameObject gameObject;
        private UInt32 gc_handle;

        protected void InitBehavior(UInt32 handle, UInt32 entity)
        {
            gameObject = new GameObject(entity);
            gc_handle = handle;
            //Entity = entity;
            GetRigidComponent();
        }

        public UInt32 Entity
        {
            get => gameObject.Entity;
            set => gameObject.Entity = value;
        }

        public void GetRigidComponent()
        {
            Console.WriteLine("Crash1");
            Console.WriteLine(Entity);
        }

        public T GetComponent<T>() where T : IScriptable
        {
            Console.WriteLine("Crash1");
            if (gameObject == null)
            {
              Console.WriteLine("empty gameObject");
            }

            return gameObject.GetComponent<T>();
        }
    }
}