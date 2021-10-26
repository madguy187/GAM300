using System;
using System.Runtime.CompilerServices;
using System.Collections.Generic;
using System.Collections;

namespace Eclipse
{
    public class EclipseBehavior : IScriptable
    {
        public GameObject gameObject;
        private UInt32 gc_handle;

        protected void InitBehavior(UInt32 handle, UInt32 entity)
        {
            Console.WriteLine("InitBehavior");
            gameObject = new GameObject(entity);
            gc_handle = handle;
            GetRigidComponent();
        }

        public UInt32 Entity
        {
            get => gameObject.Entity;
            set => gameObject.Entity = value;
        }

        public void GetRigidComponent()
        {
            Console.WriteLine("GetRigidComponent");
            Console.WriteLine(gc_handle);
            Console.WriteLine(Entity);
        }

        public T GetComponent<T>() where T : IScriptable
        {
            Console.WriteLine("Crash1");
            //gameObject.PrintSomething();
            //Console.WriteLine("Crash2");

            Console.WriteLine(Entity);
            Console.WriteLine("Crash2");

            

            return gameObject.GetComponent<T>();
        }

        [MethodImplAttribute(MethodImplOptions.InternalCall)]
        private extern static GameObject GetGameObject(UInt32 entity);
    }
}